#pragma once

#include <string>
#include <cstdint>

class NetworkService 
{
public:
    virtual ~NetworkService() = default;
    
    virtual std::string resolve_hostname(const std::string& hostname) const = 0;

    virtual void connect(const std::string& address, int port) = 0;
    virtual void disconnect() = 0;
    virtual std::string ip_address() const = 0;
    virtual int port() const = 0;
};
