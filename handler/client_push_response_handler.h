#ifndef PUSH_HANDLER_CLIENT_PUSH_RESPONSE_HANDLER_H
#define PUSH_HANDLER_CLIENT_PUSH_RESPONSE_HANDLER_H

#include "base_handler.h"

namespace im {

class ClientPushResponseHandler : public BaseHandler {
public:
    ClientPushResponseHandler() {}
    virtual ~ClientPushResponseHandler() {}

public:
    void handle(client_t* client);

};

}

#endif
