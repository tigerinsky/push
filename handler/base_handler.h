#ifndef PUSH_HANDLER_BASE_HANDLER_H
#define PUSH_HANDLER_BASE_HANDLER_H

#include "server.h"

namespace im {

class BaseHandler {
public:
    BaseHandler() {}
    virtual ~BaseHandler() {}

public:
    virtual void handle(client_t* c) = 0;
};

}

#endif
