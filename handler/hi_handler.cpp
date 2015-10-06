#include "hi_handler.h"
#include "hi.pb.h"

namespace im {

void HiHandler::handle(client_t* c) {

    client::HiRequest request;
    client::HiResponse response;
    c->response.clear();
    if (request.ParseFromArray(c->request.req_proto, c->request.req_proto_size)) {
        response.set_msg(request.msg());
        if (!response.SerializeToString(&(c->response))) {
            c->response.clear(); 
        }
    }
}

}

