#include "drop_connect_handler.h"
#include "drop_connect.pb.h"
#include "reconnect.pb.h"
#include "server.h"
#include "service.h"
#include "ae/anet.h"

namespace im {

static void client_reconn(client_t* c) {
    client::ReConnectRequest reconn_req;
    std::string buf;
    char id[21];
    snprintf(id, 21, "%llu", c->conn_id);
    reconn_req.set_conn_id(id);
    reconn_req.SerializeToString(&buf);
    c->response.set_mid(0);
    c->response.set_name(RE_CONN_CMD);
    c->response.set_content(buf);
    Protocol* protocol = Protocol::get_protocol(1);
    protocol->encode(c->response, c->writer);
    c->writer->nonblock_write(true);
    free_client(c);
}

void DropConnectHandler::handle(client_t* c) {
    router::DropConnectRequest request;
    router::DropConnectResponse response;
    c->response.Clear();
    if (!request.ParseFromArray(c->request.content().c_str(), 
                                c->request.content().size())) {
        return;
    }
    auto ite = g_server.client_map.find(request.conn_id());
    if (g_server.client_map.end() == ite) {
        LOG_INFO << "drop client: client["
            << request.conn_id() <<"] not exist"; 
        return;
    }
    client_reconn(ite->second);
    LOG_INFO << "drop client: conn["
            << request.conn_id() << "]";
    response.set_err_code(OK);
    if (!response.SerializeToString(c->response.mutable_content())) {
        c->response.Clear(); 
    }
}

}

