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

    ASSERT_EQ("142.251.36.99", service->resolve_hostname("google.de"));
}   

TEST(NetworkServiceProxyTest, InvalidHostname) 
{
    auto service = create_network_service();

    ASSERT_THROW(service->resolve_hostname("google"), invalid_argument);
}   

TEST(NetworkServiceProxyTest, ValidateAndConnect) 
{
    auto service = create_network_service();
    
    service->connect("142.251.36.99", 80);

    ASSERT_EQ("142.251.36.99", service->ip_address());
    ASSERT_EQ(80, service->port());

    service->disconnect();
    ASSERT_EQ("", service->ip_address());
    ASSERT_EQ(0, service->port());
}   
