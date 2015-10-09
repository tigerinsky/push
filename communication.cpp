#include "communication.h"
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
    ret = anetRead(fd, buf, sizeof(buf));
    if (ret <= 0) {
        LOG_WARN << "read error, ret["<<ret<<"]";
        return 1;
    }
    ret = s_protocol.decode(buf, ret, message);
    if (ret) {
        LOG_WARN << "decode message error, ret["<<ret<<"]";
        return 2;
    }
    return 0;
}

}
