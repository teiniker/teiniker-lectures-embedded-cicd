#include <stdexcept>
#include <regex>

#include "network_proxy.h"

using namespace std;


string NetworkServiceProxy::resolve_hostname(const string& hostname) const 
{
    // Validate hostname using regex
    regex pattern(R"(^([a-zA-Z0-9\-]{1,63}\.)+[a-zA-Z]{2,63}$)");
    if (!regex_match(hostname, pattern)) {
        throw invalid_argument("Invalid hostname format: " + hostname);
    }

    return service->resolve_hostname(hostname);
}

void NetworkServiceProxy::connect(const string& address, int port)
{
    // Validate IP address using regex
    regex ip_pattern(R"(^([0-9]{1,3}\.){3}[0-9]{1,3}$)");
    if (!regex_match(address, ip_pattern)) 
    {
        throw invalid_argument("Invalid IP address format: " + address);
    }
    
    // TODO: Further validate each octet is between 0 and 255 if needed

    // validate port range
    if (port == 0 || port > 65535) 
    {
        throw invalid_argument("Port number out of range: " + to_string(port));
    }

    service->connect(address, port);
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

