#include "glog/logging.h"
#include "server.h"
#include "flag.h"
#include <signal.h>
#include "push_declare.h"

namespace im {

DEFINE_int32(port, 9999, "server port");
DEFINE_string(offhub_host, "127.0.0.1", "offhub service host");
DEFINE_int32(offhub_port, 9027, "offhub service port");
DEFINE_int32(max_fd_num, 10000, "service max fd num");
DEFINE_bool(enable_conn_notify, true, "whether notify offhub for conn notify");
DEFINE_bool(enable_token_verify, true, "whether turn on token verify");
DEFINE_bool(enable_sign_verify, true, "whether turn on token verify");
DEFINE_bool(enable_profiling, false, "whether turn on profiling");

}

pthread_t g_pid;

void stop(int) {
    monitor::end_monitor();
    im::stop_server();
}

int main (int argc, char** argv) {
    ::google::ParseCommandLineFlags(&argc, &argv, true);
    ::google::InitGoogleLogging(argv[0]);
    signal(SIGINT, stop);
    signal(SIGTERM, stop);
    ADD_STAT_COLLECTOR(im::get_server_info);
    g_pid = monitor::start_monitor();
    im::init_server();
    im::start_server();
    return 0;
}
