#include <arpa/inet.h> 
#include "ae/anet.h"
#include "google/gflags.h"
#include "common.h"
#include "communication.h"
#include "hi.pb.h"
#include "client_push.pb.h"
#include "connect.pb.h"
#include "reconnect.pb.h"

DEFINE_string(h, "127.0.0.1", "");
DEFINE_int32(p, 8888, "");
DEFINE_string(conn_id, "1", "");
DEFINE_string(token, "nvshenzuipiaoliang", "");

using namespace im;

int g_fd;
pthread_t g_hi_thread;

int connect(const std::string& conn_id, const std::string& token) {
    client::ConnectRequest conn_request;
    client::ConnectResponse conn_response;
    conn_request.set_conn_id(conn_id);
    conn_request.set_token(token);
    if (send(g_fd, "connect", conn_request)) {
        LOG_WARN <<"send conn request error";
        return 1;
    }
    Msg response;
    if (receive(g_fd, &response)) {
        LOG_WARN << "read conn response error";
        return 2;
    }
    if (!conn_response.ParseFromArray(response.content().c_str(), 
                                      response.content().size())) {
        LOG_WARN << "parse conn response error"; 
        return 3;
    }
    if (conn_response.err_code() != OK) {
        LOG_WARN << "build conn fail, return_code["<<conn_response.err_code()<<"]"; 
        return 4;
    }
    LOG_INFO << "build conn success, conn_id["<< conn_id <<"] token["<<token<<"]";
    return 0;
}

void* hi(void*) {
    int ret = -1;
    while (true) {
        sleep(5); 
        client::HiRequest hi_request;
        hi_request.set_msg("hi");
        ret = send(g_fd, "hi", hi_request); 
        if (ret) {
            LOG_WARN << "send hi request error"; 
        } else {
            LOG_INFO << "send hi["<<hi_request.msg() <<"] "; 
        }
    }
}

int main (int argc, char** argv) {
    int ret = -1;
    ::google::ParseCommandLineFlags(&argc, &argv, true);
    char err[1024]; 

    ret = anetTcpConnect(err, (char*)(FLAGS_h.c_str()), FLAGS_p);
    if (ANET_ERR == ret) {
        LOG_ERROR << "connect error, host["<<FLAGS_h<<"] port["<<FLAGS_p<<"]"; 
        return 1;
    }
    g_fd = ret;
    anetEnableTcpNoDelay(NULL, g_fd); 

    if (connect(FLAGS_conn_id, FLAGS_token)) {
        LOG_ERROR << "build connect error, conn_id["
            << FLAGS_conn_id << "] token[" << FLAGS_token << "]"; 
        return 1;
    }

    if (pthread_create(&g_hi_thread, NULL, hi, NULL)) {
        LOG_ERROR << "create hi thread error, errno["<< errno <<"]"; 
        return 2;
    }
    char buf[10240];
    Msg message;
    while (true) {
        ret = receive(g_fd, &message);
        if (ret) {
            LOG_ERROR << "read error";
            break; 
        }
        if (0 == strcmp(message.name().c_str(), "hi")) {
            client::HiResponse hi_response; 
            if (!hi_response.ParseFromArray(message.content().c_str(), 
                                            message.content().size())) {
                LOG_ERROR << "parse hi response error";            
                break;
            }
            LOG_INFO << "receive hi response, msg["<<hi_response.msg()<<"]";
        } else if (0 == strcmp(message.name().c_str(), "notify_message")) {
            client::NotifyMessageRequest notify_req;
            if (!notify_req.ParseFromArray(message.content().c_str(),
                                           message.content().size())) {
                LOG_ERROR << "parse notify req error"; 
                break;
            }
            LOG_INFO << "receive message: id["
                << message.mid() <<"] data["<<notify_req.DebugString()<<"]";
            client::NotifyMessageResponse response;
            response.set_err_code(OK);
            if (send(g_fd, "notify_message", response, message.mid())) {
                LOG_ERROR << "send notify ack error, mid["<< message.mid() << "]"; 
                break;
            } 
        } else if (0 == strcmp(message.name().c_str(), "re_connect")) {
            client::ReConnectRequest re_conn_req; 
            if (!re_conn_req.ParseFromArray(message.content().c_str(),
                                            message.content().size())) {
                LOG_ERROR << "parse reconn req error"; 
                break;
            }
            LOG_INFO << "receive reconn: " << re_conn_req.DebugString();
        } else {
            LOG_WARN << "read unknown data, method["<<message.content().c_str()<<"]"; 
        }
    }

    return 0;
}
