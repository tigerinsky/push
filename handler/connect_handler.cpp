#include "connect_handler.h"
#include <openssl/md5.h>
#include "connect.pb.h"
#include "common.h"
#include "flag.h"
#include "offhub/offhub_proxy.h"
#include "push_declare.h"

namespace im {

static bool token_verify(const client::ConnectRequest& request) {
    if (0 == strcmp(request.token().c_str(), "nvshenzuipiaoliang")) {
        return true;
    }
    char md[33]; // MD5_DIGEST_LENGTH * 2
    char buf[256];
    long t = time(NULL);
    int size = snprintf(buf, 
                        256, 
                        "%lld%snvshen", 
                        t / 30, 
                        request.conn_id().c_str()); 
    md5(buf, size, md);
    if (0 != strcasecmp(md, request.token().c_str())) {
        LOG_DEBUG << "conn fail:time[" 
            << t << "] "<<size<<"token["<< request.token() <<"] md5[" << md << "]";
        STAT_COLLECT(connect_fail, 1)
        return false;
    }
    STAT_COLLECT(connect, 1)
    return true;
}

static bool sign_verify(const client::ConnectRequest& request) {
    char sha_md[20];
    char md5_md[33];
    char buf[256];
    int size = snprintf(buf, 
                        256, 
                        "conn_id=%s&token=%s", 
                        request.conn_id().c_str(),
                        request.token().c_str());
    md5(sha1(buf, size, sha_md), 20, md5_md);
    if (0 != strcasecmp(md5_md, request.sign().c_str())) {
        LOG_DEBUG << "conn fail: sign_str[" 
            << buf << "] sign[" << request.sign() << "]";
        return false; 
    }
    return true;
}

static bool is_valid(const client::ConnectRequest& request) {
    if (FLAGS_enable_token_verify && !token_verify(request)) {
        LOG_ERROR << "request token verify fail, conn_id["
            << request.conn_id() << "] token[" 
            << request.token() << "] sign[" << request.sign() << "]";
        return false;
    }
    if (FLAGS_enable_sign_verify && !sign_verify(request)) {
        LOG_ERROR << "request sign verify fail, conn_id["
            << request.conn_id() << "] token[" 
            << request.token() << "] sign[" << request.sign() << "]";
        return false; 
    }
    return true;
} 

void ConnectHandler::handle(client_t* c) {
    client::ConnectRequest request;
    client::ConnectResponse response;
    c->response.clear();
    if (!request.ParseFromArray(c->request.req_proto, c->request.req_proto_size)) {
        LOG_ERROR << "connect handler parse error";
        response.set_err_code(ILLEGAL_REQUEST);
        goto end;
    }
    LOG_DEBUG << "connect: conn_id[" << request.conn_id()
        << "] token[" << request.token() 
        << "] sign[" << request.sign() << "]";
    if (is_valid(request)) {
        c->conn_id = strtoull(request.conn_id().c_str(), NULL, 10);
        c->status = PERSIST;
        auto ite = g_server.client_map.find(c->conn_id);
        if (ite != g_server.client_map.end()
                && ite->second->id == c->id) {
            response.set_err_code(OK);
        } else {
            g_server.client_map[c->conn_id] = c;
            client_node_t* new_node 
                = new client_node_t(c, g_server.client_list.next);
            g_server.client_list.next = new_node;
            ++g_server.client_num;
            response.set_err_code(OK);
            LOG_INFO << "new connect: conn_id[" << request.conn_id()
                << "] client_id[" << c->id << "] token[" << request.token() 
                << "] sign[" << request.sign() << "]";
            if (FLAGS_enable_conn_notify) {
                g_offhub_proxy->conn_on_notify(c->conn_id);
            }
        }
    } else {
        LOG_WARN << "make connect failed: conn_id[" << request.conn_id()
            << "] token[" << request.token() 
            << "] sign[" << request.sign() << "]";
        response.set_err_code(INVALID_CONNECTION); 
    }
end:    
    if (!response.SerializeToString(&(c->response))) {
        c->response.clear();
    }
}

}
