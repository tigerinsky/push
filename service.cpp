#include "service.h"
#include <sys/time.h>
#include <unordered_map>
#include "server.h"
#include "handler/handler.h"
#include "protocol.h"
#include "common.h"
#include "flag.h"

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

void init_service() {
#define ADD_HANDLER(name, handler) do { \
    g_handler_map[name]  = new handler_t(name, new handler()); \
} while (0);
    ADD_HANDLER(HI_CMD, HiHandler);
    ADD_HANDLER(CONNECT_CMD, ConnectHandler);
    ADD_HANDLER(NOTIFY_MSG_CMD, ClientPushResponseHandler);
}

int service(client_t* c) {
    int ret = -1;
    struct timeval tv_begin, tv_end;
    gettimeofday(&tv_begin, NULL);
    ret = g_protocol.decode(c->input_buf, c->input_size, &(c->request));
    if (ret) {
        LOG_ERROR << "service: decode request error, ret["<< ret <<"]";
        return 1; 
    }
    message_t& request = c->request;
    if (FLAGS_enable_connect_verify) {
        if (!c->is_verified && 0 != strcmp(request.method, CONNECT_CMD)) {
            LOG_ERROR << "service: client not verify, method[" << request.method << "]";
            return 2; 
        }
    }
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

}
