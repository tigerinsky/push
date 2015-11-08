#include "offhub_proxy.h"
#include "thrift/protocol/TBinaryProtocol.h"
#include "thrift/transport/TSocket.h"
#include "thrift/transport/TTransportUtils.h"
#include "PostService.h"
#include "common.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

namespace im {

OffhubProxy* g_offhub_proxy = NULL;

void OffhubProxy::conn_off_notify(uint64_t conn_id) {
    boost::shared_ptr<TTransport> socket(new TSocket(_host.c_str(), _port));
    boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    lj::PostServiceClient client(protocol);
    try {
        transport->open();
        lj::ConnectOnOfflineRequest request;
        request.connect_id = conn_id;
        request.status = 1;
        client.ConnectOnOffline(request);
        transport->close();
    } catch (TException& tx) {
        LOG_ERROR << "offhub conn off notify fail, conn_id["
            << conn_id << "] what[" << tx.what() << "]";
    }
}

void OffhubProxy::conn_on_notify(uint64_t conn_id) {
    boost::shared_ptr<TTransport> socket(new TSocket(_host.c_str(), _port));
    boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    lj::PostServiceClient client(protocol);
    try {
        transport->open();
        lj::ConnectOnOfflineRequest request;
        request.connect_id = conn_id;
        request.status = 0;
        client.ConnectOnOffline(request);
        transport->close();
    } catch (TException& tx) {
        LOG_ERROR << "offhub conn on notify fail, conn_id["
            << conn_id << "] what[" << tx.what() << "]";
    }
}

void OffhubProxy::conn_not_exist_notify(uint64_t conn_id) {
    boost::shared_ptr<TTransport> socket(new TSocket(_host.c_str(), _port));
    boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    lj::PostServiceClient client(protocol);
    try {
        transport->open();
        lj::ConnectOnOfflineRequest request;
        request.connect_id = conn_id;
        request.status = 2;
        client.ConnectOnOffline(request);
        transport->close();
    } catch (TException& tx) {
        LOG_ERROR << "offhub conn on notify fail, conn_id["
            << conn_id << "] what[" << tx.what() << "]";
    }
}

}

