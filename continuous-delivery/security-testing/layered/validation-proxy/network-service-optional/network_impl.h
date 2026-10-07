#pragma once

#include <string>
#include <map>
#include <optional>

#include "network.h"

class NetworkServiceImpl : public NetworkService
{

private:
    std::string _ip_address;
    int _port;
    std::map<std::string, std::string> _dns_cache;

public:
    NetworkServiceImpl();
    ~NetworkServiceImpl() override = default;

    std::optional<std::string> resolve_hostname(const std::string& hostname) const override;
    std::optional<bool> connect(const std::string& address, int port) override;
    void disconnect() override;
    std::string ip_address() const override;
    int port() const override;
};
