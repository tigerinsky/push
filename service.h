#ifndef PUSH_SERVICE_H
#define PUSH_SERVICE_H

namespace im {

struct client_t;

void init_service();

int service(client_t* client);

void check_alive();

void send_message();

}

#endif
