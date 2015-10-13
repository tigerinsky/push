#ifndef PUSH_HANDLER_CONNECT_HANDLER_H
#define PUSH_HANDLER_CONNECT_HANDLER_H

#include "base_handler.h"

namespace im {

class ConnectHandler : public BaseHandler {
public:
    ConnectHandler() {}
    virtual ~ConnectHandler() {}

public:
    void handle(client_t* client);

};

}

#endif
