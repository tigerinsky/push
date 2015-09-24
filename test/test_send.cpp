#include <arpa/inet.h>
#include "hi.pb.h"
#include "msg.pb.h"

using namespace im;

int main (int argc, char** argv) {
    char* msg = argv[1];
    char buf[1024]; 
    char* p = buf;
    *p = 1;
    p++;


    connect::HiRequest hi;
    hi.set_msg("hi");
    Msg message;
    message.set_name("hi");
    hi.SerializeToString(message.mutable_data());
    std::string s;
    message.SerializeToString(&s);

    int size = s.size();
    
    int* p1 = (int*)p;
    *p1 = htonl(s.size());
    p += 4;
    memcpy(p, s.c_str(), s.size());

    fwrite(buf, 5 + size, 1, stdout);
    return 0;
}
