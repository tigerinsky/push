#include "glog/logging.h"
#include "google/gflags.h"
#include "server.h"

int main (int argc, char** argv) {
    ::google::ParseCommandLineFlags(&argc, &argv, true);
    ::google::InitGoogleLogging(argv[0]);
    im::init_server();
    im::start_server();
    return 0;
}
