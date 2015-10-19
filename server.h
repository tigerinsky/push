#ifndef PUSH_SERVER_H
#define PUSH_SERVER_H

#include <unordered_map>
#include <forward_list>
#include <string>
#include <queue>
#include "common.h"
#include "protocol.h"
#include "push_message.pb.h"

struct aeEventLoop;

namespace im {

const int kServerCronInterval = 1;
const int kMaxAcceptPerCall = 1000;
const int kNetIPStrLen = 46;
const int kProtoIOBufLen = 1024 * 16;
const int kCheckAliveNum = 1000;
const int kKeepAliveSec = 240;

enum {
    DEAD,
    PERSIST,
    NONE_PERSIST,
};

typedef struct client_t {
    uint32_t id;
    uint64_t conn_id;
    int fd;
    int input_size;
    char input_buf[kProtoIOBufLen];
    message_t request;
    std::string response;
    std::string output_buf;
    long last_active_time;
    uint8_t status;

    client_t() {
        id = 0;
        status = NONE_PERSIST;
        fd = -1; 
        last_active_time = 0;
    }
} client_t;

typedef struct push_msg_t {
    int send;
    push::SendMessageRequest request;
    std::string data;
    
    push_msg_t () : send(0) {}
} push_msg_t;

typedef struct server_t {
    aeEventLoop* loop;
    std::unordered_map<uint64_t, client_t*> client_map;
    std::forward_list<client_t*> persistent_clients;
    std::forward_list<client_t*>::iterator check_alive_ptr;
    std::queue<push_msg_t*> msg_queue;
    char err_msg[1024];
    uint32_t next_client_id;

    server_t() {
        loop = NULL; 
        next_client_id = 1;
        check_alive_ptr = persistent_clients.end();
    }
} server_t;
extern server_t g_server;


void init_server();
void start_server();
client_t* create_client();
void free_client(client_t* c);

}

#endif
