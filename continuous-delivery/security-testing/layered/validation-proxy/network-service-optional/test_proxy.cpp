#include <gtest/gtest.h>

#include "network.h"
#include "network_impl.h"
#include "network_proxy.h"

using namespace std;

shared_ptr<NetworkService> create_network_service() 
{
    auto service = make_shared<NetworkServiceImpl>();
    auto proxy = make_shared<NetworkServiceProxy>(service);
    return proxy;
}

TEST(NetworkServiceProxyTest, ValidateAndResolveHostname) 
{
    auto service = create_network_service();

    auto result = service->resolve_hostname("google.de");
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ("142.251.36.99", result.value());
}   

TEST(NetworkServiceProxyTest, InvalidHostname) 
{
    auto service = create_network_service();

    auto result = service->resolve_hostname("google");
    ASSERT_FALSE(result.has_value());
}   

TEST(NetworkServiceProxyTest, ValidateAndConnect) 
{
    auto service = create_network_service();
    
    auto result = service->connect("142.251.36.99", 80);
    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result.value());

    ASSERT_EQ("142.251.36.99", service->ip_address());
    ASSERT_EQ(80, service->port());

    service->disconnect();
    ASSERT_EQ("", service->ip_address());
    ASSERT_EQ(0, service->port());
}   
