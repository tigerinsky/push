#include "service.h"
#include <sys/time.h>
#include <unordered_map>
#include "server.h"
#include "handler/handler.h"
#include "protocol.h"
#include "common.h"
#include "ae/anet.h"

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

#define HI_CMD "hi"
#define CONNECT_CMD "connect"
#define NOTIFY_MSG_CMD "notify_message"
#define SEND_MSG_CMD "send_message"

void init_service() {
#define ADD_HANDLER(name, handler) do { \
    g_handler_map[name]  = new handler_t(name, new handler()); \
} while (0);
    ADD_HANDLER(HI_CMD, HiHandler);
    ADD_HANDLER(CONNECT_CMD, ConnectHandler);
    ADD_HANDLER(NOTIFY_MSG_CMD, ClientPushResponseHandler);
    ADD_HANDLER(SEND_MSG_CMD, SendMessageHandler);
}

int service(client_t* c) {
    int ret = -1;
    struct timeval tv_begin, tv_end;
    gettimeofday(&tv_begin, NULL);
    c->last_active_time = tv_begin.tv_sec;
    ret = g_protocol.decode(c->input_buf, c->input_size, &(c->request));
    if (ret) {
        LOG_ERROR << "service: decode request error, ret["<< ret <<"]";
        return 1; 
    }
    message_t& request = c->request;
    auto ite = g_handler_map.find(request.method);
    if (g_handler_map.end() == ite) {
        LOG_ERROR << "service: no matching handler, method["<< request.method <<"]";
        return 3; 
    }
    handler_t* handler = ite->second;
    handler->proc->handle(c);
    if (c->response.size() > 0) {
        ret = g_protocol.encode(handler->name, 
                                c->response, 
                                &(c->output_buf), 
                                request.id);
        if (ret) {
            LOG_ERROR << "service: encode response error, ret["<< ret <<"]";
            return 4; 
        }
    } else {
        c->output_buf.clear(); 
    }
    gettimeofday(&tv_end, NULL);
    LOG_INFO << "service: cost[" << TIME_DIFF(tv_begin, tv_end)
        << "] version[" <<  (int)(request.version)
        << "] id[" << request.id
        << "] method[" << request.method 
        << "] input[" << c->input_size
        << "] output[" << c->output_buf.size() << "]";
    return 0;
}

void check_alive() {
    if (!g_server.persistent_clients.empty()) {
        return; 
    }
    if (g_server.persistent_clients.end() == g_server.check_alive_ptr) {
        g_server.check_alive_ptr = g_server.persistent_clients.before_begin(); 
    }
    long now = time(NULL);
    auto next_ptr = g_server.check_alive_ptr;
    ++next_ptr;
    for (int i = 0; i < kCheckAliveNum; ++i) {
        if (g_server.persistent_clients.end() == next_ptr) {
            g_server.check_alive_ptr = g_server.persistent_clients.end(); 
            break;
        }
        client_t* c = *(next_ptr); 
        if (now - c->last_active_time > kKeepAliveSec && PERSIST == c->status) {
            next_ptr = g_server.persistent_clients.erase_after(g_server.check_alive_ptr);
            free_client(c);
            delete c;
            LOG_INFO << "timeout release conn[" << c->conn_id << "]";
        } else if (DEAD == c->status) {
            next_ptr = g_server.persistent_clients.erase_after(g_server.check_alive_ptr);
            free_client(c); 
            LOG_INFO << "error release conn[" << c->conn_id << "]";
        } else {
            ++g_server.check_alive_ptr; 
            ++next_ptr;
        }
    }
}

void send_message() {
    int ret = -1;
    if (0 == g_server.msg_queue.size()) {
        return; 
    }
    run_within_time (10) {
        push_msg_t* msg = NULL;
        while (true) {
            msg = g_server.msg_queue.front(); 
            if (NULL == msg) {
                return; 
            }
            if (msg->send != msg->request.conn_id_list_size()) {
                break;
            } 
            g_server.msg_queue.pop();
            delete msg;
        }     
        uint64_t conn_id = msg->request.conn_id_list(msg->send++);
        auto ite = g_server.client_map.find(conn_id);
        if (ite == g_server.client_map.end()) {
            LOG_ERROR << "send msg: no suitable conn found, conn_id["
                << conn_id << "] mid[" << msg->request.mid() << "]"; 
            continue;
        }
        client_t* c = ite->second;
        ret = g_protocol.encode(NOTIFY_MSG_CMD, 
                                msg->data, 
                                &(c->output_buf), 
                                msg->request.mid());
        if (0 != ret) {
            LOG_ERROR << "send msg: encode msg error, conn_id["
                << conn_id << "] mid[" << msg->request.mid() << "]"; 
            continue;
        }
        ret = anetWrite(c->fd, (char*)(c->output_buf.c_str()), c->output_buf.size());
        if (ret != c->output_buf.size()) {
            LOG_ERROR << "send msg: write msg error, ret[" 
                << ret << "] errno[" << errno << "]";
            continue;
        }
        LOG_INFO << "send msg: finish, conn_id["
            << conn_id << "] mid[" << msg->request.mid() << "]";
    }
}

}
