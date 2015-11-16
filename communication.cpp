#include "communication.h"
#include <arpa/inet.h> 
#include "ae/anet.h"
#include "common.h"
#include "protocol.h"

namespace im {

Version_1_Protocol s_protocol;

int send(int fd, 
         const std::string& name,
         const ::google::protobuf::Message& message,
         uint64_t mid) {
    SocketWriter writer(fd);
    Version_1_Protocol protocol;
    Msg msg;
    message.SerializeToString(msg.mutable_content());
    msg.set_mid(mid);
    msg.set_name(name);
    int ret = protocol.encode(msg, &writer);
    if (ret) {
        LOG_WARN << "encode request error, method["
            << name << "] data[" << message.DebugString()<< "]";
        return 1;
    }
    assert(0 == writer.nonblock_write(true));
    return 0;
}

int receive(int fd, Msg* msg) {
    char buf[102400];
    int ret = -1;
    message_header_t* header = (message_header_t*)buf;
    ret = read(fd, buf, sizeof(*header));
    if (ret <=  0) {
        LOG_WARN << "read header error, ret["<<ret<<"] errno["<<errno<<"]";
        return 1;
    }
    int magic_num = ntohl(header->magic_num);
    if (magic_num != kMagicNum) {
        LOG_WARN << "magic num error: " << magic_num; 
        return 1;
    }
    int size  = ntohl(header->proto_size);
    ret = read(fd, buf + sizeof(*header), size);
    if (ret != size) {
        LOG_WARN << "read data error, ret["<<ret<<"] errno["<<errno<<"]";
        return 1;
    }
    if (!msg->ParseFromArray(buf + sizeof(*header), size)) {
        LOG_WARN << "decode message error, ret["<<ret<<"]";
        return 2;
    }
    return 0;
}

}
