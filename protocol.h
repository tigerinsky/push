#ifndef PUSH_PROTOCOL_H
#define PUSH_PROTOCOL_H

#include <unordered_map>
#include "msg.pb.h"
#include "buffer.h"

namespace im {

const int kMagicNum = 1234567891;

#pragma pack(1)
typedef struct message_header_t {
    char version; 
    int magic_num;
    int proto_size;
} message_header_t;
#pragma pack()

class Protocol {
public:
    Protocol() {}
    virtual ~Protocol() {}
    virtual char get_version() = 0;
    virtual int decode(const message_header_t& header,
                       SocketReader* reader,
                       im::Msg* msg) = 0;
    virtual int encode(const im::Msg& response,
                       SocketWriter* writer) = 0;

public:
    static const int kOk = 0;
    static const int kNotReady = 1;
    static const int kError = 1;

public:
    static void init();
    static Protocol* get_protocol(char version);
    static int read_header(SocketReader* reader, 
                           message_header_t* header);

private:
    static std::unordered_map<int, Protocol*> _s_protocol_map; 
};

class Version_1_Protocol : public Protocol {
public:
    Version_1_Protocol() {}
    virtual ~Version_1_Protocol() {}

public:
    char get_version() { 
        return 1;
    }

    int decode(const message_header_t& header,
               SocketReader* reader, 
               im::Msg* msg);

    int encode(const im::Msg& response,
               SocketWriter* writer);
};

} 

#endif
