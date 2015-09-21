#ifndef PUSH_HANDLER_CONNECT_HANDLER_H
#define PUSH_HANDLER_CONNECT_HANDLER_H

#include "base_handler.h"
#include "connect.pb.h"

namespace im {

class ConnectHandler : public BaseHandler {
public:
    ConnectHandler() {}
    virtual ~ConnectHandler() {}

public:
    void handle(const char* input_msg, int input_size,
                const ::google::protobuf::Message** message);

private:
    connect::ConnectRequest _request;
    connect::ConnectResponse _response;
};

}

#endif
