#include "buffer.h"
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include "google/protobuf/message.h"
#include "common.h"

namespace im {

SocketReader::SocketReader(int buf_size, int fd) {
    _buf_capacity = buf_size; 
    _data_size = 0;
    _fd = fd;
    assert(_buf = (char*)malloc(buf_size));
    _consume_size = 0;
    _garbage_size = 0;
}

SocketReader::~SocketReader() {
    free(_buf);
}

int SocketReader::nonblock_read() {
    if (_data_size >= _buf_capacity) {
        return kBufferOverflow; 
    }
    int ret = read(_fd, _buf + _data_size, _buf_capacity - _data_size); 
    if (0 == ret) {
        return kPeerClosed; 
    }
    if (0 > ret) {
        if (EAGAIN == errno) {
            return kOk;
        } else {
            return kError; 
        }
    }
    _data_size += ret;
    LOG_DEBUG << "in nonblock_read ret[" << ret << "] _data_size[" << _data_size << "]";
    if (_garbage_size > 0) {
        if (_garbage_size > _data_size) {
            _data_size = 0; 
            _garbage_size -= _data_size;
        } else {
            memcpy(_buf, _buf + _garbage_size, _data_size - _garbage_size); 
            _data_size -= _garbage_size;
            _garbage_size = 0;
        }
    }
    return kOk;
}

bool SocketReader::consume(int size, char** p) {
    LOG_DEBUG << "_data_size[" << _data_size << "] _consume_size[" << _consume_size << "] size[" << size << "]";
    if (_data_size - _consume_size < size) {
        return false; 
    }
    *p =  _buf + _consume_size;
    _consume_size += size;
    return true; 
}

void SocketReader::mark_garbage(int size) {
    _garbage_size = size; 
}

void SocketReader::adapt() {
    if (_consume_size > 0) {
        int left_size = _data_size - _consume_size;
        if (_garbage_size >= left_size) {
            _data_size = 0; 
            _consume_size = 0;
            _garbage_size -= left_size;
        } else {
            _data_size = left_size - _garbage_size;
            memcpy(_buf, _buf + _consume_size + _garbage_size, _data_size);
            _consume_size = 0;
            _garbage_size = 0;
        }
    }     
}

SocketWriter::SocketWriter(int fd) {
    _fd = fd;
    _has_write = 0;
}

SocketWriter::~SocketWriter() {}

int SocketWriter::nonblock_write(bool flush) {
    int ret = -1;
    if (_buf.size() <= _has_write) {
        return kOk; 
    } 
    while (true) {
        ret = write(_fd, 
                    _buf.c_str() + _has_write, 
                    _buf.size() - _has_write); 
        LOG_DEBUG << "write:" + ret;
        if (ret < 0) {
            if (EAGAIN == errno) {
                if (flush) {
                    continue;
                } else {
                    return kOk; 
                }
            } else {
                return kError;
            }
        }
        _has_write += ret;
        if (_has_write == _buf.size()) {
            break; 
        }
    }
    _has_write = 0;
    _buf.clear();
    return 0;
}

void SocketWriter::add_data(void* data, int len) {
    _buf.append((char*)data, len);
}

int SocketWriter::add_proto(const ::google::protobuf::Message& proto) {
    int old_size = _buf.size();
    int proto_size = proto.ByteSize();
    _buf.resize(_buf.size() + proto_size);
    if (proto.SerializeToArray(
                (char*)(_buf.c_str() + old_size),
                proto_size)) {
        return 0; 
    } else {
        return 1; 
    }
}

}
