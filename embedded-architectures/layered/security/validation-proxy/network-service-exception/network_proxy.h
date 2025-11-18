#pragma once

#include <memory>

#include "network.h"

class NetworkServiceProxy : public NetworkService
{

private:
    std::shared_ptr<NetworkService> service;

public:
    NetworkServiceProxy(std::shared_ptr<NetworkService> service) : service(service) {}
    ~NetworkServiceProxy() override = default;

    std::string resolve_hostname(const std::string& hostname) const override;
    void connect(const std::string& address, int port) override;
    void disconnect() override;
    std::string ip_address() const override;
    int port() const override;
};
