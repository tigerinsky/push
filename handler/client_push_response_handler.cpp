#include "client_push_response_handler.h"
#include "client_push.pb.h"
#include "common.h"

namespace im {

void ClientPushResponseHandler::handle(client_t* c) {
    client::NotifyMessageResponse response;
    c->response.clear();
    if (!response.ParseFromArray(c->request.req_proto, c->request.req_proto_size)) {
        LOG_ERROR << "client push response handler parse error";
        return;
    }
    if (OK != response.err_code()) {
        LOG_ERROR << "client push response error, conn_id[" << c->conn_id  
            <<"] mid[" << c->request.id << "]";
        return;
    }
    LOG_DEBUG << "client return notify response: mid[" << c->request.id << "]";
    //
}

}
