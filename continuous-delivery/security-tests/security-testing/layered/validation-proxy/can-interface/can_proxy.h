#pragma once

#include <memory>

#include "can.h"

class CanProxy : public CanInterface
{

private:
    std::shared_ptr<CanInterface> service;

public:
    CanProxy(std::shared_ptr<CanInterface> service) : service(service) {}
    ~CanProxy() override = default;

    void sendMessage(const CanMessage& message) override;
    CanMessage receiveMessage(void) override;
};
