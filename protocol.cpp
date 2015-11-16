#include "protocol.h"
#include <arpa/inet.h>
#include "common.h"
#include "buffer.h"

namespace im {

int Version_1_Protocol::encode(const im::Msg& response,
                               SocketWriter* writer) {
    int proto_size = response.ByteSize();
    message_header_t resp_header;
    resp_header.version = get_version();
    resp_header.magic_num = htonl(kMagicNum);
    resp_header.proto_size = htonl(proto_size);
    writer->add_data((char*)(&resp_header), sizeof(resp_header));
    if(writer->add_proto(response)) {
        return kError; 
    }
    return kOk;
}

int Version_1_Protocol::decode(const message_header_t& header, 
                               SocketReader* reader, 
                               im::Msg* msg) {
    char* p = NULL;
    if (!reader->consume(header.proto_size, &p)) {
        return kNotReady; 
    }
    msg->Clear();
    if (!msg->ParseFromArray(p, header.proto_size)) {
        return kError;
    }
    return kOk;
}

std::unordered_map<int, Protocol*> Protocol::_s_protocol_map; 

Protocol* Protocol::get_protocol(char version) {
    auto ite = _s_protocol_map.find(version);
    return ite == _s_protocol_map.end() ? NULL : ite->second;
}

void Protocol::init() {
#define ADD_PROTOCOL(name) do { \
    Protocol* p = new name (); \
    _s_protocol_map[p->get_version()] = p; \
} while (0);
    ADD_PROTOCOL(Version_1_Protocol);
}

int Protocol::read_header(SocketReader* reader, 
                          message_header_t* header) {
    message_header_t* h = NULL;
    if (!reader->consume(sizeof(*h), (char**)(&h))) {
        return kNotReady;
    }
    header->version = h->version;
    header->magic_num = ntohl(h->magic_num);
    header->proto_size = ntohl(h->proto_size);
    if (header->magic_num != kMagicNum) {
        LOG_ERROR<<"magic num not right";
        return kError; 
    }
    return kOk;
}

}
