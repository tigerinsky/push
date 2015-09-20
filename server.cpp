#include "server.h"
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "glog/logging.h"
#include "ae/ae.h"
#include "ae/anet.h"
#include "common.h"

namespace im {

server_t g_server;

static int server_cron(aeEventLoop* loop, long long id, void* data) {
//    printf("%s\n", (char*)data);
    return kServerCronInterval;
}


void init_server() {
    g_server.loop = aeCreateEventLoop(1024);
    aeCreateTimeEvent(g_server.loop, 
                      kServerCronInterval, 
                      server_cron, 
                      NULL, 
                      NULL);
}

static client_t* create_client() {
    client_t* c = new(std::nothrow) client_t;
    if (NULL == c) {
        return NULL; 
    }
    c->id = g_server.next_client_id++;
    c->fd = -1;
    return c;
}

static void free_client(client_t* c) {
    if (c->fd > 0) {
        aeDeleteFileEvent(g_server.loop, c->fd, AE_READABLE);
        aeDeleteFileEvent(g_server.loop, c->fd, AE_WRITABLE);
        close(c->fd);
    }
}

static void send_reply_to_client(aeEventLoop* loop, int fd, void *data, int mask) {
    client_t* c = (client_t*)data;
    int ret = -1;
    int nwrite = 0;
    while (true) {
        ret = write(fd, c->resp_buf + nwrite, c->resp_size - nwrite); 
        if (ret < 0) {
            if (EAGAIN == errno) {
                return;  
            } else {
                //err 
                free_client(c);
                return;
            }
        }
        nwrite += ret;
        if (nwrite == c->resp_size) {
            break; 
        }
    }
    c->resp_size = 0;
    aeDeleteFileEvent(g_server.loop, c->fd, AE_WRITABLE);
}

static int parse_request(const char* req_buf, int req_size, request_t* request) {
    if (NULL == request) {
        return 0; 
    }
#define READ_INT(p, value, left) do { \
    if (left < sizeof(int)) return 1; \
    *value = *((int*)p); \
    p += sizeof(int); \
    left -= sizeof(int); \
} while (0);
    int left = req_size;
    char* p = const_cast<char*>(req_buf);
    READ_INT(p, &(request->version), left);
    request->method = p;
    while (left && '\0' != *p) { 
        ++p; 
        --left;
    }
    if (*p != '\0') return 1;
    ++p; 
    READ_INT(p, &(request->req_proto_size), left);
    if (left != request->req_proto_size) {
        return 1;
    }
    request->req_proto = p; 
    return 0;
}

static void request_handler(aeEventLoop* loop, int fd, void* data, int mask) {
    client_t* c = (client_t*)data;
    int ret = read(fd, c->req_buf, kProtoIOBufLen);
    if (0 == ret) {
        return;
    } else if (ret < 0) {
        if (EAGAIN == errno) {
            return; 
        } else {
        //  err 
            free_client(c);
            return; 
        }
    }
    c->req_size = ret;
    ret = parse_request(c->req_buf, c->req_size, &(c->request));
    if (0 != ret) {
        free_client(c); 
        return;
    }
    memcpy(c->resp_buf, c->req_buf, ret);
    c->resp_size = ret;
    if (aeCreateFileEvent(loop, fd, AE_WRITABLE, send_reply_to_client, c) == AE_ERR) {
        free_client(c);
    }
}

static void conn_handler(aeEventLoop* loop, int fd, void* data, int mask) {
    int max = kMaxAcceptPerCall;
    char cip[kNetIPStrLen];
    int cport;
    while (max--) {
        int cfd = anetTcpAccept(NULL, fd, cip, sizeof(cip), &cport); 
        if (cfd == ANET_ERR) {
            if (errno != EWOULDBLOCK) {
                //true err 
            }
            return;
        }
        client_t* c = create_client();
        if (NULL == c) {
            close(cfd); 
            return;
        }
        c->fd = cfd;
        anetNonBlock(NULL, cfd);
        anetEnableTcpNoDelay(NULL, cfd); 
        //anetKeepAlive(NULL,fd,1000); 
        if (aeCreateFileEvent(loop, cfd, AE_READABLE, 
                    request_handler, c) == AE_ERR) {
            close(cfd);
            delete c;
        }
    }
}

void start_server() {
    signal(SIGPIPE, SIG_IGN);
    int fd = anetTcpServer(g_server.err_msg, 9999, NULL, 700);
    assert(fd != ANET_ERR);
    anetNonBlock(g_server.err_msg, fd);
    assert(aeCreateFileEvent(g_server.loop, fd, AE_READABLE, conn_handler, NULL) 
            != AE_ERR);
    aeMain(g_server.loop);
}

}
