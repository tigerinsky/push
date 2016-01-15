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
#include "buffer.h"
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

static inline bool can_shutdown(client_t* c) {
    return c->status == NONE_PERSIST && !(c->in_white_list);
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

client_t* create_client(int fd) {
    client_t* c = new(std::nothrow) client_t;
    if (NULL == c) {
        return NULL; 
    }
    c->id = g_server.next_client_id++;
    c->fd = fd;
    c->reader = new(std::nothrow) SocketReader(kProtoIOBufLen, fd);
    if (NULL == c->reader) {
        delete c;
        return NULL; 
    }
    c->writer = new(std::nothrow) SocketWriter(fd);
    if (NULL == c->writer) {
        delete c; 
        return NULL;
    }
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
    switch (c->writer->nonblock_write(true)) {
    case SocketWriter::kOk: 
        if (can_shutdown(c)) {
            free_client(c); 
        } else {
            aeDeleteFileEvent(g_server.loop, c->fd, AE_WRITABLE);
        }
        break;
    case SocketWriter::kLeftSome:
        return;
    case SocketWriter::kError:
        if (can_shutdown(c)) {
            free_client(c); 
        } else {
            free_client(c, ERROR); 
        }
        return;
    default:
        // not suppose to enter here
        return;
    }
}

static void request_handler(aeEventLoop* loop, int fd, void* data, int mask) {
    client_t* c = (client_t*)data;
    int ret = c->reader->nonblock_read();
    LOG_INFO << "debug ret[" << ret << "]";
    if (SocketReader::kPeerClosed == ret) {
        LOG_INFO << "client close connect, conn_id[" << c->conn_id << "] client_id["
                << c->id << "]";
        free_client(c, ERROR);
        return;
    }
    if (SocketReader::kError == ret) {
        LOG_ERROR << "client read error, conn_id[" << c->conn_id << "] client_id["
        << c->id << "]";
        free_client(c, ERROR);
        return; 
    }
    service(c);
    if (c->writer->has_data()) {
        if (aeCreateFileEvent(loop, fd, AE_WRITABLE, send_reply_to_client, c) == AE_ERR) {
            free_client(c, ERROR);
        }
    } else {
        if (can_shutdown(c)) {
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
        client_t* c = create_client(cfd);
        if (NULL == c) {
            close(cfd); 
            return;
        }
        LOG_DEBUG << cip;
        memcpy(c->ip, cip, sizeof(cip));
        if (g_server.white_list.find(cip) != g_server.white_list.end()) {
            c->in_white_list = true; 
        }
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

void get_server_info(std::string* info) {
    info->clear();
    string_appendf(info, "active_client_num:%u\n", g_server.client_map.size());
    string_appendf(info, "total_client_num:%u\n", g_server.client_num);
    string_appendf(info, "inhold_push_task_num:%u\n", g_server.msg_queue.size());
}

}
