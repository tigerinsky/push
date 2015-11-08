#include "service.h"
#include <sys/time.h>
#include <unordered_map>
#include "server.h"
#include "handler/handler.h"
#include "protocol.h"
#include "common.h"
#include "ae/anet.h"
#include "offhub/offhub_proxy.h"
#include "flag.h"
#include "push_declare.h"

namespace im {

class BaseHandler;

typedef struct handler_t {
    handler_t (const char* n, BaseHandler* b) 
        : name(n), proc(b) {}
    std::string name;
    BaseHandler* proc; 
} handler_t;

std::unordered_map<std::string, handler_t*> g_handler_map;
Protocol g_protocol;

void init_service() {
#define ADD_HANDLER(name, handler) do { \
    g_handler_map[name]  = new handler_t(name, new handler()); \
} while (0);
    ADD_HANDLER(HI_CMD, HiHandler);
    ADD_HANDLER(CONNECT_CMD, ConnectHandler);
    ADD_HANDLER(NOTIFY_MSG_CMD, ClientPushResponseHandler);
    ADD_HANDLER(SEND_MSG_CMD, SendMessageHandler);
    ADD_HANDLER(DROP_CONN_CMD, DropConnectHandler);
}

int service2(client_t* c) {
    int ret = -1;
    message_t& request = c->request;
    auto ite = g_handler_map.find(request.method);
    if (g_handler_map.end() == ite) {
        LOG_ERROR << "service: no matching handler, method["<< request.method <<"]";
        return 1; 
    }
    handler_t* handler = ite->second;
    handler->proc->handle(c);
    return 0;
}

int service(client_t* c) {
    int ret = -1;
    char* p_input = c->input_buf;
    int left_size = c->input_size;
    while (left_size > sizeof(message_header_t)) {
        struct timeval tv_begin, tv_end;
        gettimeofday(&tv_begin, NULL);
        c->last_active_time = tv_begin.tv_sec;
        ret = g_protocol.decode(p_input, left_size, &(c->request)); 
        if (0 > ret) {
            LOG_ERROR << "service: decode request error, ret[" << ret << "]"; 
            return 1;
        }
        p_input += ret;
        left_size -= ret;
        ret = service2(c);
        if (ret) {
            LOG_ERROR << "service: handle request error, ret[" << ret << "]"; 
            return 1; 
        }
        if (c->response.size() > 0) {
            ret = g_protocol.encode(c->request.method, 
                                    c->response, 
                                    &(c->output_buf), 
                                    c->request.id);
            if (ret) {
                LOG_ERROR << "service: encode response error, ret["<< ret <<"]";
                return 4; 
            }
        }
        gettimeofday(&tv_end, NULL);
        LOG_INFO << "service: cost[" << TIME_DIFF(tv_begin, tv_end)
            << "] version[" <<  (int)(c->request.version)
            << "] id[" << c->request.id
            << "] method[" << c->request.method 
            << "] input[" << c->input_size
            << "] output[" << c->output_buf.size() 
            << "] conn_id[" << c->conn_id 
            << "] client_id[" << c->id <<"]";
    } 
    return 0;
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
            LOG_INFO << "error release conn[" << c->conn_id << "] client_id["
                << c->id << "]";
            STAT_COLLECT(free_connect, 1);
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
            STAT_COLLECT(free_connect, 1);
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
    int failed = 0;
    LOG_DEBUG << "msg in queue:" << g_server.msg_queue.size();
    run_within_time (30) {
        push_msg_t* msg = NULL;
        while (true) {
            if (0 == g_server.msg_queue.size()) {
                return; 
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
        c->output_buf.clear();
        ret = g_protocol.encode(NOTIFY_MSG_CMD, 
                                msg->data, 
                                &(c->output_buf), 
                                msg->request.mid());
        if (0 != ret) {
            LOG_ERROR << "send msg: encode msg error, conn_id["
                << conn_id << "] client_id[" << c->id << "]mid[" 
                << msg->request.mid() << "]"; 
            ++failed;
            continue;
        }
        ret = anetWrite(c->fd, (char*)(c->output_buf.c_str()), c->output_buf.size());
        if (ret != c->output_buf.size()) {
            LOG_ERROR << "send msg: write msg error, ret[" 
                << ret << "] errno[" << errno << "] conn_id["
                << c->conn_id << "] id[" << c->id << "]";
            ++failed;
            free_client(c, ERROR);
            continue;
        }
        ++success;
        LOG_INFO << "send msg: finish, conn_id["
            << conn_id << "] mid[" << msg->request.mid() << "]";
    }
    gettimeofday(&tv_end, NULL);
    STAT_COLLECT(cost, TIME_DIFF(tv_begin, tv_end))
    STAT_COLLECT(send_success, success)
    STAT_COLLECT(send_fail, failed)
    LOG_INFO << "send msg stat: total["
        << (success + failed) << "] cost["
        << TIME_DIFF(tv_begin, tv_end) << "] succ["
        << success << "] fail[" << failed << "]";
}

}
