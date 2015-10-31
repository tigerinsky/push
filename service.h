#ifndef PUSH_SERVICE_H
#define PUSH_SERVICE_H

#include "protocol.h"

namespace im {

#define HI_CMD "hi"
#define CONNECT_CMD "connect"
#define NOTIFY_MSG_CMD "notify_message"
#define SEND_MSG_CMD "send_message"
#define DROP_CONN_CMD "drop_connect"
#define RE_CONN_CMD "re_connect"

extern Protocol g_protocol;

struct client_t;

void init_service();

int service(client_t* client);

void check_alive();

void send_message();

}

#endif
