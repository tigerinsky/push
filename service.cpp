#include "service.h"
#include <sys/time.h>
#include <unordered_map>
#include "server.h"
#include "handler/handler.h"
#include "protocol.h"
#include "common.h"

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
    ADD_HANDLER("hi", HiHandler);
    ADD_HANDLER("connect", ConnectHandler);
}

int service(client_t* c) {
    int ret = -1;
    struct timeval tv_begin, tv_end;
    gettimeofday(&tv_begin, NULL);
    ret = g_protocol.decode(c->req_buf, c->req_size, &(c->request));
    if (ret) {
        LOG_ERROR << "service: decode request error, ret["<< ret <<"]";
        return 1; 
    }
    message_t& request = c->request;
    auto ite = g_handler_map.find(request.method);
    if (g_handler_map.end() == ite) {
        LOG_ERROR << "service: no matching handler, method["<< request.method <<"]";
        return 2; 
    }
    handler_t* handler = ite->second;
    const ::google::protobuf::Message* response = NULL;
    handler->proc->handle(request.req_proto, request.req_proto_size, &response);
    ret = g_protocol.encode(handler->name, response, &(c->response), request.id);
    if (ret) {
        LOG_ERROR << "service: encode response error, ret["<< ret <<"]";
        return 3; 
    }
    gettimeofday(&tv_end, NULL);
    LOG_INFO << "service: cost[" << TIME_DIFF(tv_begin, tv_end)
        << "] version[" <<  (int)(request.version)
        << "] id[" << request.id
        << "] method[" << request.method 
        << "] input[" << c->req_size
        << "] output[" << c->response.size() << "]";
    return 0;
}

}
