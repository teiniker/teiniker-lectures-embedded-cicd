#include <gtest/gtest.h>

#include "network.h"
#include "network_impl.h"

using namespace std;

TEST(NetworkServiceTest, ResolveHostname) 
{
    auto service = NetworkServiceImpl();
    ASSERT_EQ("142.251.36.99", service.resolve_hostname("google.de"));
}

TEST(NetworkServiceTest, ResolveHostnameNotFound) 
{
    auto service = NetworkServiceImpl();
    ASSERT_THROW(service.resolve_hostname("unknown.hostname"), invalid_argument);
}

TEST(NetworkServiceTest, ConnectDisconnect) 
{
    auto service = NetworkServiceImpl();
    
    service.connect("142.251.36.99", 80);
    ASSERT_EQ("142.251.36.99", service.ip_address());
    ASSERT_EQ(80, service.port());
    
    service.disconnect();
    ASSERT_EQ("", service.ip_address());
    ASSERT_EQ(0, service.port());
}

