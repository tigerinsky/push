#ifndef PUSH_HANDLER_HI_HANDLER_H
#define PUSH_HANDLER_HI_HANDLER_H

#include "base_handler.h"
#include "hi.pb.h"

namespace im {

class HiHandler : public BaseHandler {
public:
    HiHandler() {}
    virtual ~HiHandler() {}

public:
    void handle(const char* input_msg, int input_size,
                const ::google::protobuf::Message** message);

private:
    connect::HiRequest _request;
    connect::HiResponse _response;
};

}

#endif
