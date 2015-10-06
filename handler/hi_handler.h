#ifndef PUSH_HANDLER_HI_HANDLER_H
#define PUSH_HANDLER_HI_HANDLER_H

#include "base_handler.h"

namespace im {

class HiHandler : public BaseHandler {
public:
    HiHandler() {}
    virtual ~HiHandler() {}

public:
    void handle(client_t* client);
};

}

#endif
