#include "service.h"
#include <sys/time.h>
#include <arpa/inet.h>
#include <unordered_map>
#include <fstream>
#include "server.h"
#include "handler/handler.h"
#include "protocol.h"
#include "common.h"
#include "ae/anet.h"
#include "offhub/offhub_proxy.h"
#include "flag.h"
#include "push_declare.h"
#include "buffer.h"

namespace im {

class BaseHandler;

typedef struct handler_t {
    handler_t (const char* n, BaseHandler* b) 
        : name(n), proc(b) {}
    std::string name;
    BaseHandler* proc; 
} handler_t;

std::unordered_map<std::string, handler_t*> g_handler_map;

void load_white_list(const char* path) {
    std::ifstream ifs(path);
    if (!ifs.is_open()) {
        LOG_ERROR << "open white list conf file error: " << path; 
        return;
    }
    std::string line;
    while (getline(ifs, line)) {
        if (line.size() > 0 && line[0] != '#') {
            LOG_INFO << "load white list:" << line;
            g_server.white_list.insert(line);
        }
    }
    ifs.close();
}

void init_service() {
#define ADD_HANDLER(name, handler) do { \
    g_handler_map[name]  = new handler_t(name, new handler()); \
} while (0);
    ADD_HANDLER(HI_CMD, HiHandler);
    ADD_HANDLER(CONNECT_CMD, ConnectHandler);
    ADD_HANDLER(NOTIFY_MSG_CMD, ClientPushResponseHandler);
    ADD_HANDLER(SEND_MSG_CMD, SendMessageHandler);
    ADD_HANDLER(DROP_CONN_CMD, DropConnectHandler);
    Protocol::init();
    if (FLAGS_white_list_conf.size() > 0) {
        load_white_list(FLAGS_white_list_conf.c_str()); 
    }
}

int service2(client_t* c) {
    Msg& request = c->request;
    auto ite = g_handler_map.find(request.name());
    if (g_handler_map.end() == ite) {
        LOG_ERROR << "service: no matching handler, method["<< request.name() <<"]";
        return 1; 
    }
    handler_t* handler = ite->second;
    handler->proc->handle(c);
    return 0;
}

void service(client_t* c) {
    int ret = -1;
    char* p = NULL;
    while (true) {
        struct timeval tv_begin, tv_end;
        gettimeofday(&tv_begin, NULL);
        c->last_active_time = tv_begin.tv_sec;
        if (NULL == c->protocol) {
            ret = Protocol::read_header(c->reader, &(c->header));
            if (Protocol::kNotReady == ret) {
                break; 
            } else if (Protocol::kError == ret) {
                LOG_ERROR << "service: parse header error, client_id["
                    << c->id << "] conn_id[" 
                    << c->conn_id << "] ip["
                    << c->ip << "]";
                if (PERSIST == c->status) {
                    free_client(c, ERROR); 
                }
                return;
            }
            LOG_DEBUG << "proto size[" << c->header.proto_size << "]";
            c->protocol = Protocol::get_protocol(c->header.version);
            if (NULL == c->protocol) {
                LOG_ERROR << "service: get protocol error, version["
                    << c->header.version << "] data_size["
                    << c->header.proto_size << "]" ;
                c->reader->mark_garbage(c->header.proto_size);
                break;
            }
        } 
        ret = c->protocol->decode(c->header, 
                                  c->reader,
                                  &(c->request));
        if (Protocol::kNotReady == ret) {
            LOG_DEBUG << "protocol decode not ready";
            break;
        } else if (Protocol::kError == ret) {
            LOG_ERROR << "service: decode error, conn_id["
                << c->conn_id <<"] client_id[" << c->id << "]"; 
            free_client(c, ERROR);
            return;
        }
        ret = service2(c);
        if (ret) {
            LOG_ERROR << "service: handle request error, conn_id["
                << c->conn_id << "] client_id[" << c->id <<"]"; 
            return; 
        }
        if (c->response.content().size() > 0) {
            c->response.set_mid(c->request.mid());
            c->response.set_name(c->request.name());
            ret = c->protocol->encode(c->response, c->writer);
            if (Protocol::kError == ret) {
                LOG_ERROR << "service: encode response error, conn_id["
                    << c->conn_id << "] client_id["
                    << c->id << "]";
                free_client(c, ERROR);
                return; 
            }
        }
        gettimeofday(&tv_end, NULL);
        LOG_INFO << "service: cost[" << TIME_DIFF(tv_begin, tv_end)
            << "] version[" <<  (int)(c->header.version)
            << "] id[" << c->request.mid()
            << "] method[" << c->request.name()
            << "] input[" << c->header.proto_size
            << "] output[" << c->response.content().size() 
            << "] conn_id[" << c->conn_id 
            << "] client_id[" << c->id <<"]";
        c->protocol = NULL;
    }
    c->reader->adapt();
    return;
}

void check_alive() {
    if (0 == g_server.client_num) {
        return; 
    }
    if (NULL == g_server.check_alive_ptr) {
        g_server.check_alive_ptr = &(g_server.client_list); 
    }
    long now = time(NULL);
    client_node_t* next_ptr = g_server.check_alive_ptr->next;
    for (int i = 0; i < kCheckAliveNum; ++i) {
        if (NULL == next_ptr) {
            g_server.check_alive_ptr = NULL; 
            break;
        }
        client_t* c = next_ptr->client; 
        long diff = now - c->last_active_time;
        if (diff < kKeepAliveSec / 10) {
            break;
        } else if (ERROR == c->status) {
            g_server.check_alive_ptr->next = next_ptr->next;
            delete next_ptr;
            --g_server.client_num;
            if (FLAGS_enable_conn_notify
                    && g_server.client_map.find(c->conn_id) == g_server.client_map.end()) {
                g_offhub_proxy->conn_off_notify(c->conn_id);
            }
            STAT_COLLECT_COUNT(error_release);
            LOG_INFO << "error release conn[" << c->conn_id << "] client_id["
                << c->id << "]";
            free_client(c); 
        } else if (diff > kKeepAliveSec && PERSIST == c->status) {
            g_server.check_alive_ptr->next = next_ptr->next;
            delete next_ptr;
            --g_server.client_num;
            if (FLAGS_enable_conn_notify) {
                auto ite = g_server.client_map.find(c->conn_id);
                if (ite == g_server.client_map.end()
                        || ite->second->id == c->id) {
                    g_offhub_proxy->conn_off_notify(c->conn_id);
                }
            }
            LOG_INFO << "timeout release conn[" 
                << c->conn_id << "] last_active_time["
                << c->last_active_time << "] client_id["
                << c->id <<"]";
            free_client(c, OFFLINE);
        } else {
            g_server.check_alive_ptr = next_ptr;
        }
        next_ptr = g_server.check_alive_ptr->next;
    }
}

void send_message() {
    int ret = -1;
    if (0 == g_server.msg_queue.size()) {
        return; 
    }
    struct timeval tv_begin, tv_end;
    gettimeofday(&tv_begin, NULL);
    int old_size = g_server.msg_queue.size();
    int success = 0;
    Protocol* protocol = Protocol::get_protocol(1);
    int failed = 0;
    LOG_DEBUG << "msg in queue:" << g_server.msg_queue.size();
    run_within_time (30) {
        push_msg_t* msg = NULL;
        while (true) {
            if (0 == g_server.msg_queue.size()) {
                goto end; 
            }
            msg = g_server.msg_queue.front(); 
            if (msg->send != msg->request.conn_id_list_size()) {
                break;
            } 
            g_server.msg_queue.pop();
            delete msg;
        }     
        LOG_DEBUG << "do: mid[" << msg->request.mid() <<"] size["<< msg->request.conn_id_list_size() <<"] send["<< msg->send <<"]";
        uint64_t conn_id = msg->request.conn_id_list(msg->send++);
        auto ite = g_server.client_map.find(conn_id);
        if (ite == g_server.client_map.end()) {
            LOG_ERROR << "send msg: no suitable conn found, conn_id["
                << conn_id << "] mid[" << msg->request.mid() << "]"; 
            ++failed;
            if (FLAGS_enable_conn_notify) {
                g_offhub_proxy->conn_not_exist_notify(conn_id);
            }
            continue;
        }
        client_t* c = ite->second;
        c->response.set_mid(msg->request.mid());
        c->response.set_name(NOTIFY_MSG_CMD);
        c->response.set_allocated_content(&(msg->data));
        ret = protocol->encode(c->response, c->writer);
        if (0 != ret) {
            LOG_ERROR << "send msg: encode msg error, conn_id["
                << conn_id << "] client_id[" << c->id << "]mid[" 
                << msg->request.mid() << "]"; 
            ++failed;
            c->response.release_content();
            continue;
        }
        LOG_DEBUG << "left:" << c->writer->left();
        ret = c->writer->nonblock_write(true);
        LOG_DEBUG << "left2:" << c->writer->left();
        if (ret != SocketWriter::kOk) {
            LOG_ERROR << "send msg: write msg error, ret[" 
                << ret << "] errno[" << errno << "] conn_id["
                << c->conn_id << "] id[" << c->id << "]";
            ++failed;
            free_client(c, ERROR);
            c->response.release_content();
            continue;
        }
        c->response.release_content();
        ++success;
        LOG_INFO << "send msg: finish, conn_id["
            << conn_id << "] mid[" << msg->request.mid() << "]";
    }
end:
    gettimeofday(&tv_end, NULL);
    STAT_COLLECT(send_fail, failed)
    STAT_COLLECT(send_success, success)
    STAT_COLLECT(all_send_fail, failed)
    STAT_COLLECT(all_send_success, success)
    LOG_INFO << "send msg stat: total["
        << (success + failed) << "] cost["
        << TIME_DIFF(tv_begin, tv_end) << "] succ["
        << success << "] fail[" << failed << "]";
}

}
