#include "service.h"
#include <unordered_map>
#include "server.h"
#include "handler/handler.h"

namespace im {

handler_t g_handler_list[] = {
    {"hi", new HiHandler},
    {"connect", new ConnectHandler},
};

std::unordered_map<std::string, handler_t*> g_handler_map;

static int parse_request(const char* req_buf, int req_size, request_t* request) {
    if (NULL == request) {
        return 0; 
    }
#define READ_INT(p, value, left) do { \
    if (left < sizeof(int)) return 1; \
    *value = *((int*)p); \
    p += sizeof(int); \
    left -= sizeof(int); \
} while (0);
    int left = req_size;
    char* p = const_cast<char*>(req_buf);
    READ_INT(p, &(request->version), left);
    request->method = p;
    while (left && '\0' != *p) { 
        ++p; 
        --left;
    }
    if (*p != '\0') return 1;
    ++p; 
    READ_INT(p, &(request->req_proto_size), left);
    if (left != request->req_proto_size) {
        return 1;
    }
    request->req_proto = p; 
    return 0;
}

static int generate_response(const handler_t* handler, 
                             const ::google::protobuf::Message* response,
                             std::string* buf) {
    static const int version = 1;
    buf->clear();
    int proto_size = response->ByteSize();
    int size = 4 + handler->name.size() + 1 + 4 + proto_size; 
    buf->reserve(size);
    buf->append((char*)(&version), sizeof(version)); 
    buf->append(handler->name.c_str(), handler->name.size() + 1);
    buf->append((char*)(&proto_size), sizeof(proto_size));
    return response->SerializeToString(buf);
}

void init_service() {
    for (int i = 0; i < sizeof(g_handler_list) / sizeof(handler_t); ++i) {
        handler_t* h = g_handler_list + i; 
        g_handler_map[h->name] = h;
    }
}

int service(client_t* c) {
    if (parse_request(c->req_buf, c->req_size, &(c->request))) {
        return 1; 
    }
    request_t& request = c->request;
    auto ite = g_handler_map.find(request.method);
    if (g_handler_map.end() == ite) {
        return 2; 
    }
    handler_t* handler = ite->second;
    const ::google::protobuf::Message* response = NULL;
    handler->proc->handle(c->req_buf, c->req_size, &response);
    generate_response(handler, response, &(c->response));
}

}
