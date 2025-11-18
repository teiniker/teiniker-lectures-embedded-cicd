#include <stdexcept>
#include <regex>
#include <optional>

#include "network_proxy.h"

using namespace std;


optional<string> NetworkServiceProxy::resolve_hostname(const string& hostname) const 
{
    // Validate hostname using regex
    regex pattern(R"(^([a-zA-Z0-9\-]{1,63}\.)+[a-zA-Z]{2,63}$)");
    if (!regex_match(hostname, pattern)) {
        return nullopt;
    }

    return service->resolve_hostname(hostname);
}

optional<bool> NetworkServiceProxy::connect(const string& address, int port)
{
    // Validate IP address using regex
    regex ip_pattern(R"(^([0-9]{1,3}\.){3}[0-9]{1,3}$)");
    if (!regex_match(address, ip_pattern)) 
    {
        return nullopt;
    }
    
    // TODO: Further validate each octet is between 0 and 255 if needed

    // validate port range
    if (port == 0 || port > 65535) 
    {
        return nullopt;
    }

    service->connect(address, port);
    return true;
}

void NetworkServiceProxy::disconnect()
{
    service->disconnect();
}
string NetworkServiceProxy::ip_address() const
{
    return service->ip_address();
}

int NetworkServiceProxy::port() const
{
    return service->port();
}

