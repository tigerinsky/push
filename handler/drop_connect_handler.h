#ifndef PUSH_HANDLER_DROP_CONNECT_HANDLER_H
#define PUSH_HANDLER_DROP_CONNECT_HANDLER_H

#include "base_handler.h"

namespace im {

class DropConnectHandler : public BaseHandler {
public:
    DropConnectHandler() {}
    virtual ~DropConnectHandler() {}

public:
    void handle(client_t* client);
};

}

#endif
