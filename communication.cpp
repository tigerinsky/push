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
    return writer.write(1000) == SocketWriter::kOk ? 0 : -1;
}

int receive(int fd, Msg* msg) {
    char buf[1024000];
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
    int has_read = 0;
    while (has_read != size) {
        ret = read(fd, buf + sizeof(*header) + has_read, size - has_read);
        if (ret <= 0) {
            LOG_WARN << "read data error, ret["<<ret<<"] errno["<<errno<<"]";
            return 1;
        }
        has_read += ret;
    }
    if (!msg->ParseFromArray(buf + sizeof(*header), size)) {
        LOG_WARN << "decode message error, ret["<<ret<<"]";
        return 2;
    }
    return 0;
}

}
