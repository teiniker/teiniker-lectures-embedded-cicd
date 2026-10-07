#include <stdexcept>

#include "network_impl.h"

using namespace std;

NetworkServiceImpl::NetworkServiceImpl() 
{
    _dns_cache = {
        {"google.de", "142.251.36.99"},
        {"github.com", "140.82.121.3"},
        {"ieeexplore.ieee.org", "99.84.91.122"}
    };  
    
}

std::string NetworkServiceImpl::resolve_hostname(const std::string& hostname) const 
{
    auto it = _dns_cache.find(hostname);
    if (it != _dns_cache.end()) 
    {
        return it->second;
    } 
    else 
    {
        throw std::invalid_argument("Hostname not found in DNS cache");
    }
}

void NetworkServiceImpl::connect(const std::string& address, int port)
{
    _ip_address = address;
    _port = port;
}

void NetworkServiceImpl::disconnect()
{
    _ip_address.clear();
    _port = 0;
}

std::string NetworkServiceImpl::ip_address() const
{
    return _ip_address;
}

int NetworkServiceImpl::port() const
{
    return _port;
}