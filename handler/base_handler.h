#ifndef PUSH_HANDLER_BASE_HANDLER_H
#define PUSH_HANDLER_BASE_HANDLER_H

#include "google/protobuf/message.h"

namespace im {

class BaseHandler {
public:
    BaseHandler() {}
    virtual ~BaseHandler() {}

public:
    virtual void handle(const char* input_msg, int input_size,
                        const ::google::protobuf::Message** message) = 0;
};

}

#endif
