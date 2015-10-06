#include <arpa/inet.h>
#include "connect.pb.h"
#include "msg.pb.h"

using namespace im;

int main (int argc, char** argv) {
    client::ConnectRequest conn; 
    conn.set_conn_id(strtoll(argv[1], NULL, 10)); 
    conn.set_token(argv[2]);
    Msg message; 
    message.set_name("connect");
    conn.SerializeToString(message.mutable_content());
    std::string s;
    message.SerializeToString(&s);
    char buf[1024]; 
    char* p = buf;
    *p = 1;
    p++;
    int size = s.size();
    int* p1 = (int*)p;
    *p1 = htonl(s.size());
    p += 4;
    memcpy(p, s.c_str(), s.size());
    fwrite(buf, 5 + size, 1, stdout);

    return 0;
}
