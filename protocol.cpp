#include "protocol.h"
#include <arpa/inet.h>
#include "common.h"

namespace im {

int Protocol::encode(const std::string& name,
                     const ::google::protobuf::Message* message,
                     std::string* buf, 
                     int id) {
    buf->clear();
    _encode_msg.Clear();
    _encode_msg.set_id(id);
    _encode_msg.set_name(name);
    if (!message->SerializeToString(_encode_msg.mutable_data())) {
        return 1; 
    }
    int proto_size = _encode_msg.ByteSize();
    message_header_t header;
    header.version = kVersion;
    header.proto_size = htonl(proto_size);
    int size = sizeof(message_header_t) + proto_size; 
    buf->reserve(size);
    buf->append((char*)(&header), sizeof(header));
    buf->resize(size);
    if(!_encode_msg.SerializeToArray((char*)(buf->c_str() + sizeof(header)),
                               proto_size)) {
        return 2;
    }
    return 0;
}

int Protocol::decode(const char* input, int size, message_t* message) {
    if (size < sizeof(message_header_t)) {
        return 1; 
    }
    const char* p = input;
    message_header_t* header = (message_header_t*)p;
    header->proto_size = ntohl(header->proto_size);
    LOG_DEBUG << "decode: package size[" << size 
        << "] version[" << (int)header->version
        << "] proto_size[" << header->proto_size << "]";
    if (header->version != kVersion) {
        return 2; 
    }
    if (size != sizeof(message_header_t) + header->proto_size) {
        return 3; 
    }
    p += sizeof(message_header_t);
    _decode_msg.Clear();
    if (!_decode_msg.ParseFromArray(p, header->proto_size)) {
        return 4;
    }
    message->version = header->version;
    message->id = _decode_msg.id();
    message->method = _decode_msg.name().c_str();
    message->req_proto_size = _decode_msg.data().size();
    message->req_proto = _decode_msg.data().c_str();
    return 0;
}

}
