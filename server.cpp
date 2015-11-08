#include "server.h"
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "ae/ae.h"
#include "ae/anet.h"
#include "common.h"
#include "service.h"
#include "flag.h"
#include "offhub/offhub_proxy.h"
#ifdef _USE_PROF
#include "gperftools/profiler.h"
#endif

namespace im {

server_t g_server;

#define run_with_period(ms) if ((ms <= kServerCronInterval) || !(g_server.cronloops % (ms / kServerCronInterval)))

static void try_stop() {
    LOG_INFO << "stop server: left client[" 
        << g_server.client_map.size() << "] left msg["
        << g_server.msg_queue.size() << "]";
    exit(0);
}

static int server_cron(aeEventLoop* loop, long long id, void* data) {
    if (g_server.stop) {
        try_stop(); 
    }
    run_with_period(100) {
        check_alive();
    }
    send_message();
    ++g_server.cronloops;
    if (0 != g_server.msg_queue.size()) {
        return kServerShortCronInterval;
    } else {
        return kServerCronInterval;
    }
}

void init_server() {
    g_offhub_proxy = new OffhubProxy(FLAGS_offhub_host.c_str(), FLAGS_offhub_port);
    g_server.loop = aeCreateEventLoop(FLAGS_max_fd_num);
    aeCreateTimeEvent(g_server.loop, 
                      1, 
                      server_cron, 
                      NULL, 
                      NULL);
    init_service();
}

client_t* create_client() {
    client_t* c = new(std::nothrow) client_t;
    if (NULL == c) {
        return NULL; 
    }
    c->id = g_server.next_client_id++;
    c->fd = -1;
    return c;
}

void free_client(client_t* c, Status status) {
    if (c->fd > 0) {
        aeDeleteFileEvent(g_server.loop, c->fd, AE_READABLE);
        aeDeleteFileEvent(g_server.loop, c->fd, AE_WRITABLE);
        close(c->fd);
        c->fd = -1;
    }
    switch (c->status) {
    case PERSIST:
        g_server.client_map.erase(c->conn_id);
        if (OFFLINE == status) {
            delete c;
        } else {
            c->status = status;
        }
        break;
    case ERROR:
    case NONE_PERSIST:
        delete c;
        break;
    default:
        LOG_WARN << "server panic: unexpecting status[" 
            << c->status << "] conn_id["
            << c->conn_id << "] client_id["
            << c->id <<"]";
    }
}

static void send_reply_to_client(aeEventLoop* loop, int fd, void *data, int mask) {
    client_t* c = (client_t*)data;
    int ret = -1;
    int nwrite = 0;
    while (true) {
        ret = write(fd, 
                    c->output_buf.c_str() + nwrite, 
                    c->output_buf.size() - nwrite); 
        if (ret < 0) {
            if (EAGAIN == errno) {
                return;  
            } else {
                LOG_ERROR << "write response error, conn_id["
                    << c->conn_id << "] client_id[" << c->id << "]";
                if (NONE_PERSIST == c->status) {
                    free_client(c);
                } else {
                    free_client(c, ERROR);
                }
                return;
            }
        }
        nwrite += ret;
        if (nwrite == c->output_buf.size()) {
            break; 
        }
    }
    c->output_buf.clear();
    aeDeleteFileEvent(g_server.loop, c->fd, AE_WRITABLE);
    if (NONE_PERSIST == c->status) {
        free_client(c); 
    }
}

static void request_handler(aeEventLoop* loop, int fd, void* data, int mask) {
    client_t* c = (client_t*)data;
    int ret = read(fd, c->input_buf, kProtoIOBufLen);
    if (0 == ret) {
        LOG_INFO << "client close connect, conn_id[" << c->conn_id << "] client_id["
                << c->id << "]";
        free_client(c, ERROR);
        return;
    } else if (ret < 0) {
        if (EAGAIN == errno) {
            return; 
        } else {
            LOG_ERROR << "client read error, conn_id[" << c->conn_id << "] client_id["
                << c->id << "]";
            free_client(c, ERROR);
            return; 
        }
    }
    c->input_size = ret;
    ret = service(c);
    if (0 != ret) {
   //     free_client(c, ERROR); 
   //     return;
    }
    if (c->output_buf.size() > 0) {
        if (aeCreateFileEvent(loop, fd, AE_WRITABLE, send_reply_to_client, c) == AE_ERR) {
            free_client(c, ERROR);
        }
    } else {
        if (NONE_PERSIST == c->status) {
            free_client(c); 
        } 
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
    int fd = anetTcpServer(g_server.err_msg, FLAGS_port, NULL, 700);
    assert(fd != ANET_ERR);
    anetNonBlock(g_server.err_msg, fd);
    assert(aeCreateFileEvent(g_server.loop, fd, AE_READABLE, conn_handler, NULL) 
            != AE_ERR);
    if (FLAGS_enable_profiling) {
#ifdef _USE_PROF
        ProfilerStart("./haha");  
#endif
    }
    LOG_INFO << "start server";
    aeMain(g_server.loop);
}

void stop_server() {
    //TODO
    if (FLAGS_enable_profiling) {
#ifdef _USE_PROF
        ProfilerStop();  
#endif
    }
    g_server.stop = true;
}

}
