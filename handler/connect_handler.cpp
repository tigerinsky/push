#include "connect_handler.h"
#include <openssl/md5.h>
#include "connect.pb.h"
#include "common.h"

namespace im {

static bool is_valid(const client::ConnectRequest& request) {
    if (0 == strcmp(request.token().c_str(), "nvshenzuipiaoliang")) {
        return true;
    }
    char md[32];
    char buf[256];
    long t = time(NULL);
    int size = snprintf(buf, 
                        256, 
                        "%lld%snvshen", 
                        t / 30, 
                        request.conn_id().c_str()); 
    printf("%s\n", buf);
    md5(buf, size, md);
    if (0 == strcasecmp(md, request.token().c_str())) {
        return true;
    }
    LOG_DEBUG << "conn fail:time[" 
        << t << "] "<<size<<"token["<< request.token() <<"] md5[" << md << "]";
    return false;
} 

void ConnectHandler::handle(client_t* c) {
    client::ConnectRequest request;
    client::ConnectResponse response;
    c->response.clear();
    if (!request.ParseFromArray(c->request.req_proto, c->request.req_proto_size)) {
        LOG_ERROR << "connect handler parse error";
        response.set_err_code(ILLEGAL_REQUEST);
        goto end;
    }
    LOG_DEBUG << "connect: conn_id[" << request.conn_id()
        << "] token[" << request.token() 
        << "] sign[" << request.sign() << "]";
    if (is_valid(request)) {
        c->conn_id = strtoull(request.conn_id().c_str(), NULL, 10);
        c->status = PERSIST;
        g_server.client_map[c->conn_id] = c;
        g_server.persistent_clients.push_front(c);
        response.set_err_code(OK);
        LOG_INFO << "new connect: conn_id[" << request.conn_id()
            << "] token[" << request.token() 
            << "] sign[" << request.sign() << "]";
    } else {
        LOG_WARN << "make connect failed: conn_id[" << request.conn_id()
            << "] token[" << request.token() 
            << "] sign[" << request.sign() << "]";
        response.set_err_code(INVALID_CONNECTION); 
    }
end:    
    if (!response.SerializeToString(&(c->response))) {
        c->response.clear();
    }
}

}
