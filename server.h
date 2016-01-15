#ifndef PUSH_SERVER_H
#define PUSH_SERVER_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <queue>
#include "common.h"
#include "protocol.h"
#include "push_message.pb.h"
#include "msg.pb.h"

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

class SocketReader;

typedef struct client_t {
    uint32_t id;
    uint64_t conn_id;
    char ip[kNetIPStrLen];
    int fd;
    SocketReader* reader;
    SocketWriter* writer;
    Protocol* protocol;
    message_header_t header;
    im::Msg request;
    im::Msg response;
    long last_active_time;
    Status status;
    bool in_white_list;

    client_t() {
        id = 0;
        conn_id = 0;
        status = NONE_PERSIST;
        fd = -1; 
        protocol = NULL;
        last_active_time = 0;
        reader = NULL;
        writer = NULL;
        in_white_list = false;
    }

    ~client_t() {
        if (reader) {
            delete reader; 
        } 
        if (writer) {
            delete writer; 
        }
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
    std::unordered_set<std::string> white_list;

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
