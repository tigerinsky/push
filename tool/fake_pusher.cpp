#include <arpa/inet.h>
#include "ae/anet.h"
#include "google/gflags.h"
#include "push_message.pb.h"
#include "common/data.pb.h"
#include "communication.h"
#include "common.h"

DEFINE_string(h, "127.0.0.1", "");
DEFINE_int32(p, 8888, "");
DEFINE_uint64(to, 1, "");
DEFINE_uint64(mid, 1, "");
DEFINE_string(msg, "hi", "");

using namespace im;

int main (int argc, char** argv) {
    int ret = -1;
    ::google::ParseCommandLineFlags(&argc, &argv, true);
    char err[1024]; 

    ret = anetTcpConnect(err, (char*)(FLAGS_h.c_str()), FLAGS_p);
    if (ANET_ERR == ret) {
        LOG_ERROR << "connect error, host["<<FLAGS_h<<"] port["<<FLAGS_p<<"]"; 
        return 1;
    }
    int fd = ret;
    anetEnableTcpNoDelay(NULL, fd); 
    common::IMMessage msg;
    msg.set_type(common::TEXT);
    msg.set_message_id(FLAGS_mid);
    msg.mutable_text()->set_content(FLAGS_msg);
    std::string data;
    msg.SerializeToString(&data);
    push::SendMessageRequest request;
    request.add_conn_id_list(FLAGS_to);
    request.set_mid(FLAGS_mid);
    common::NotifyMessage* notify_msg = request.mutable_msg();
    notify_msg->set_channel_type(common::MEETING_MAIN);
    notify_msg->set_data(data);
    ret = send(fd, "send_message", request, FLAGS_mid);
    if (ret) {
        LOG_ERROR << "send message error"; 
        return 1;
    }
    LOG_INFO << "send message, id["<<FLAGS_mid<<"] msg["
        <<FLAGS_msg<<"] to["<<FLAGS_to<<"]";
    message_t resp_msg;
    ret = receive(fd, &resp_msg);
    if (ret) {
        LOG_ERROR << "read reasponse from server error"; 
        return 1;
    }
    push::SendMessageResponse response;
    if (!response.ParseFromArray(resp_msg.req_proto, resp_msg.req_proto_size)) {
        LOG_ERROR << "parse response error";    
        return 1;
    }
    if (response.err_code() != OK) {
        LOG_ERROR << "reponse error, err_code["<<response.err_code()<<"]"; 
        return 1;
    }
    LOG_INFO << "response ok";
    return 0;
}

