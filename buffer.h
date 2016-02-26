#ifndef PUSH_BUFFER_H
#define PUSH_BUFFER_H

#include <string>

namespace google {
namespace protobuf {
class Message;
}
}

namespace im {

class SocketReader {
public:
    static const int kOk = 0;
    static const int kPeerClosed = 1;
    static const int kError = 2;
//    static const int kBufferOverflow = 3;

public:
    SocketReader(int buf_size, int fd);
    virtual ~SocketReader();

public:
    int nonblock_read();
    bool consume(int size, char** p);
    // 标记要废弃掉之后的若干字节
    void mark_garbage(int size);
    void adapt();

private:
    char* _buf;
    int _buf_capacity;
    int _data_size;
    int _consume_size;
    int _fd;
    int _garbage_size;
};

class SocketWriter {
public:
    static const int kOk = 0;
    static const int kError = 1;
    static const int kLeftSome = 2;
    static const int kMaxWriteSizeOnce = 40960;

public:
    explicit SocketWriter(int fd);
    virtual ~SocketWriter();

public:
    int nonblock_write(); 
    int write(uint32_t timeout_ms);
    void add_data(void* data, int len); 
    int add_proto(const ::google::protobuf::Message& proto);
    bool has_data() {
        return _buf.size() - _has_write > 0; 
    }
    int left() {
        return _buf.size() - _has_write; 
    }

private:
    std::string _buf;
    int _has_write;
    int _fd;
};

}

#endif
