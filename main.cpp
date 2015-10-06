#include "glog/logging.h"
#include "server.h"
#include "flag.h"

namespace im {

DEFINE_bool(enable_connect_verify, true, "whether open conn verify");

}


int main (int argc, char** argv) {
    ::google::ParseCommandLineFlags(&argc, &argv, true);
    ::google::InitGoogleLogging(argv[0]);
    im::init_server();
    im::start_server();
    return 0;
}
