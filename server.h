#ifndef PUSH_SERVER_H
#define PUSH_SERVER_H

#include <unordered_map>
#include <string>
#include <queue>
#include "common.h"
#include "protocol.h"
#include "push_message.pb.h"

struct aeEventLoop;

namespace im {

const int kServerShortCronInterval = 5;
const int kServerCronInterval = 20;
const int kMaxAcceptPerCall = 1000;
const int kNetIPStrLen = 46;
const int kProtoIOBufLen = 1024 * 16;
const int kCheckAliveNum = 300;
const int kKeepAliveSec = 240;

enum Status {
    ERROR, 
    OFFLINE,
    PERSIST,
    NONE_PERSIST,
    STATUS_END
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
    Status status;

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

typedef struct client_node_t {
    client_t* client;
    client_node_t* next;

    client_node_t() : next(NULL), client(NULL) {}
    client_node_t(client_t* c, client_node_t* n) 
        : client(c), next(n) {}
} client_node_t;

typedef struct server_t {
    bool stop;
    aeEventLoop* loop;
    long cronloops;
    std::unordered_map<uint64_t, client_t*> client_map;
    int client_num;
    client_node_t client_list;
    client_node_t* check_alive_ptr;
    std::queue<push_msg_t*> msg_queue;
    char err_msg[1024];
    uint32_t next_client_id;

    server_t() {
        stop = false;
        loop = NULL; 
        cronloops = 0;
        client_num = 0;
        next_client_id = 1;
        check_alive_ptr = NULL;
    }
} server_t;
extern server_t g_server;

void init_server();
void start_server();
void stop_server();
client_t* create_client();
void free_client(client_t* c, Status status=STATUS_END);
void get_server_info(std::string* info);

}

#endif
