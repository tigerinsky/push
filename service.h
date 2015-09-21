#ifndef PUSH_SERVICE_H
#define PUSH_SERVICE_H

#include <string>

namespace im {

struct client_t;
class BaseHandler;

typedef struct handler_t {
    std::string name;
    BaseHandler* proc; 
} handler_t;

void init_service();

int service(client_t* client);

}

#endif
