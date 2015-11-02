#ifndef PUSH_OFFHUB_OFFHUB_PROXY_H
#define PUSH_OFFHUB_OFFHUB_PROXY_H

#include <string>

namespace im {

class OffhubProxy {
public:
    OffhubProxy(const char* host, uint32_t port) 
        : _host(host), _port(port) {}
    virtual ~OffhubProxy(){}

public:
    void conn_off_notify(uint64_t conn_id);
    void conn_on_notify(uint64_t conn_id);

private:
    std::string _host;
    uint32_t _port;
};

extern OffhubProxy* g_offhub_proxy;

}

#endif
