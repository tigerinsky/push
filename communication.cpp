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
    buf.clear();
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

int receive(int fd, message_t* message) {
    static char buf[102400];
    int ret = -1;
    message_header_t* header = (message_header_t*)buf;
    ret = read(fd, buf, sizeof(*header));
    if (ret <=  0) {
        LOG_WARN << "read header error, ret["<<ret<<"] errno["<<errno<<"]";
        return 1;
    }
    int size  = ntohl(header->proto_size);
    ret = read(fd, buf + sizeof(*header), size);
    if (ret != size) {
        LOG_WARN << "read data error, ret["<<ret<<"] errno["<<errno<<"]";
        return 1;
    }
    ret = s_protocol.decode(buf, size + sizeof(*header), message);
    if (ret < 0) {
        LOG_WARN << "decode message error, ret["<<ret<<"]";
        return 2;
    }
    return 0;
}

}
