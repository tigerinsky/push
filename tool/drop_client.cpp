#include <arpa/inet.h>
#include "ae/anet.h"
#include "google/gflags.h"
#include "drop_connect.pb.h"
#include "communication.h"
#include "common.h"

DEFINE_string(h, "127.0.0.1", "");
DEFINE_int32(p, 8888, "");
DEFINE_uint64(conn_id, 0, "");

using namespace im;

int main (int argc , char** argv) {
    int ret = -1;
    char err[1024];
    ::google::ParseCommandLineFlags(&argc, &argv, true);
    int fd = anetTcpConnect(err, (char*)(FLAGS_h.c_str()), FLAGS_p);
    if (ANET_ERR == fd) {
        LOG_ERROR << "connect error, host["<<FLAGS_h<<"] port["<<FLAGS_p<<"]"; 
        return 1;
    }
    anetEnableTcpNoDelay(NULL, fd); 
    router::DropConnectRequest request;
    request.set_conn_id(FLAGS_conn_id);
    if (send(fd, "drop_connect", request)) {
        LOG_WARN << "send reconn request error"; 
        return 1;
    }
    Msg msg;
    if (receive(fd, &msg)) {
        LOG_WARN << "read response error"; 
        return 1;
    }
    router::DropConnectResponse response;
    if (!response.ParseFromArray(msg.content().c_str(), 
                                 msg.content().size())) {
        LOG_WARN << "parse response errro";
        return 1;
    }
    LOG_INFO << "resp: " << response.DebugString();
    return 0;
}
