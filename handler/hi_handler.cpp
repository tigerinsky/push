#include "hi_handler.h"
#include "hi.pb.h"

namespace im {

void HiHandler::handle(client_t* c) {
    client::HiRequest request;
    client::HiResponse response;
    c->response.Clear();
    if (c->status != PERSIST) {
        return; 
    }
    if (request.ParseFromArray(c->request.content().c_str(), 
                               c->request.content().size())) {
        response.set_msg(request.msg());
        if (!response.SerializeToString(c->response.mutable_content())) {
            c->response.Clear(); 
        }
    }
}

}

