#ifndef PUSH_HANDLER_SEND_MESSAGE_HANDLER_H
#define PUSH_HANDLER_SEND_MESSAGE_HANDLER_H

#include "base_handler.h"

namespace im {

class SendMessageHandler : public BaseHandler {
public:
    SendMessageHandler() {}
    virtual ~SendMessageHandler() {}

public:
    void handle(client_t* client);
};

}

#endif
