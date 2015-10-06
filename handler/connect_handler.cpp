#include "connect_handler.h"
#include "connect.pb.h"
#include "common.h"

namespace im {

static bool is_valid(const client::ConnectRequest& request) {
    if (0 != strncmp(request.token().c_str(), "seiya", 5)) {
        return false;
    }
    return true;
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
        c->is_verified = true;
        c->conn_id = _request.conn_id();
        g_server.client_map[_request.conn_id()] = c;
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
