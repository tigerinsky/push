#include "send_message_handler.h"
#include "push_message.pb.h"
#include "client_push.pb.h"
#include "common.h"

namespace im {

void SendMessageHandler::handle(client_t* c) {
    push::SendMessageResponse response;
    client::NotifyMessageRequest notify_msg;
    c->response.clear();
    push_msg_t* msg = new(std::nothrow) push_msg_t;
    if (NULL == msg) {
        LOG_ERROR << "parse send message request error";
        response.set_err_code(INTERNAL_ERR);
        goto end;
    }
    if (!msg->request.ParseFromArray(c->request.req_proto, 
                                     c->request.req_proto_size)) {
        LOG_ERROR << "parse send message request error";
        response.set_err_code(ILLEGAL_REQUEST);
        goto end; 
    }
    notify_msg.set_allocated_msg(msg->request.mutable_msg());
    if (!notify_msg.SerializeToString(&(msg->data))) {
        LOG_ERROR << "serialize send message data error";
        response.set_err_code(INTERNAL_ERR);
        goto end; 
    }
    g_server.msg_queue.push(msg);
    response.set_err_code(OK);
end:
    notify_msg.release_msg();
    if (!response.SerializeToString(&(c->response))) {
        c->response.clear(); 
    }
}

}

