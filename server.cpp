#include "server.h"
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "ae/ae.h"
#include "ae/anet.h"
#include "common.h"
#include "service.h"

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
    init_service();
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
        ret = write(fd, c->response.c_str() + nwrite, c->response.size() - nwrite); 
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
        if (nwrite == c->response.size()) {
            break; 
        }
    }
    c->response.clear();
    aeDeleteFileEvent(g_server.loop, c->fd, AE_WRITABLE);
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
    ret = service(c);
    if (0 != ret) {
        free_client(c); 
        return;
    }
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
    LOG_INFO << "start server";
    aeMain(g_server.loop);
}

}
