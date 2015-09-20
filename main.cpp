#include "server.h"

int main (int argc, char** argv) {
    im::init_server();
    im::start_server();
    return 0;
}
