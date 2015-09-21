#ifndef PUSH_SERVER_H
#define PUSH_SERVER_H

#include <unordered_map>
#include <string>

struct aeEventLoop;

namespace im {

const int kServerCronInterval = 1;
const int kMaxAcceptPerCall = 1000;
const int kNetIPStrLen = 46;
const int kProtoIOBufLen = 1024 * 16;

typedef struct request_t {
    int version;
    char* method;
    int req_proto_size;
    char* req_proto;
} request_t;

typedef struct client_t {
    uint32_t id;
    int fd;
    int req_size;
    char req_buf[kProtoIOBufLen];
    request_t request;
    std::string response;

    client_t() {
        id = 0;
        fd = -1; 
    }
} client_t;

typedef struct server_t {
    aeEventLoop* loop;
    std::unordered_map<uint32_t, client_t*> client_map;
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
