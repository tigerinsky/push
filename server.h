#ifndef PUSH_SERVER_H
#define PUSH_SERVER_H

#include <unordered_map>
#include <string>
#include "common.h"
#include "protocol.h"

struct aeEventLoop;

namespace im {

const int kServerCronInterval = 1;
const int kMaxAcceptPerCall = 1000;
const int kNetIPStrLen = 46;
const int kProtoIOBufLen = 1024 * 16;

typedef struct client_t {
    uint32_t id;
    std::string conn_id;
    bool is_verified;
    int fd;
    int input_size;
    char input_buf[kProtoIOBufLen];
    message_t request;
    std::string response;
    std::string output_buf;

    client_t() {
        id = 0;
        is_verified = false;
        fd = -1; 
        resp_proto = NULL;
    }
} client_t;

typedef struct server_t {
    aeEventLoop* loop;
    std::unordered_map<uint64_t, client_t*> client_map;
    char err_msg[1024];
    uint32_t next_client_id;

    server_t() {
        loop = NULL; 
        next_client_id = 1;
    }
} server_t;
extern server_t g_server;


void init_server();
void start_server();


}

#endif
