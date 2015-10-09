#include "communication.h"
#include <arpa/inet.h> 
#include "ae/anet.h"
#include "common.h"
#include "protocol.h"

namespace im {

Protocol s_protocol;

int send(int fd, 
         const std::string& name,
         const ::google::protobuf::Message& message,
         uint64_t mid) {
    static std::string data;
    static std::string buf;
    message.SerializeToString(&data);
    int ret = s_protocol.encode(name, data, &buf, mid);
    if (ret) {
        LOG_WARN << "encode request error, method["
            << name << "] data[" << message.DebugString()<< "]";
        return 1;
    }
    ret = anetWrite(fd, (char*)(buf.c_str()), buf.size());
    if (ret != buf.size()) {
        LOG_WARN << "write request error, ret[" << ret << "] method["
            << name << "] data[" << message.DebugString()<< "]";
        return 2; 
    }
    return 0;
}

int read(int fd, message_t* message) {
    static char buf[10240];
    int ret = -1;
    message_header_t* header = (message_header_t*)buf;
    ret = anetRead(fd, buf, sizeof(*header));
    if (ret !=  sizeof(*header)) {
        LOG_WARN << "read header error, ret["<<ret<<"] errno["<<errno<<"]";
        return 1;
    }
    int proto_size = ntohl(header->proto_size);
    ret = anetRead(fd, buf + sizeof(*header), proto_size);
    if (ret != proto_size) {
        LOG_WARN << "read data error, ret["<<ret<<"] errno["<<errno<<"]";
        return 1;
    }
    ret = s_protocol.decode(buf, proto_size + sizeof(*header), message);
    if (ret) {
        LOG_WARN << "decode message error, ret["<<ret<<"]";
        return 2;
    }
    return 0;
}

}
