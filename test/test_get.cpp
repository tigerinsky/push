#include <arpa/inet.h>
#include "hi.pb.h"
#include "msg.pb.h"

using namespace im;

int main (int argc, char** argv) {
    char buf[1024]; 
    int size = fread(buf, 1, 17, stdin);
    printf("read:%d\n", size);
    printf("version: %d\n", buf[0]);
    int* p1 = (int*)(buf + 1);
    printf("size:%d\n", ntohl(*p1));
    
    Msg message;
    message.ParseFromArray(buf + 5, size - 5);
    printf("msg:[%s]\n", message.DebugString().c_str());

    client::HiRequest hi;
    hi.ParseFromString(message.content());
    printf("hi:[%s]\n", hi.DebugString().c_str());
    return 0;
}
