#include <gtest/gtest.h>

#include "network.h"
#include "network_impl.h"

using namespace std;

TEST(NetworkServiceTest, ResolveHostname) 
{
    auto service = NetworkServiceImpl();
    auto result = service.resolve_hostname("google.de");
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ("142.251.36.99", result.value());
}

TEST(NetworkServiceTest, ResolveHostnameNotFound) 
{
    auto service = NetworkServiceImpl();
    auto result = service.resolve_hostname("unknown.hostname");
    ASSERT_FALSE(result.has_value());
}

TEST(NetworkServiceTest, ConnectDisconnect) 
{
    auto service = NetworkServiceImpl();
    
    auto result = service.connect("142.251.36.99", 80);
    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result.value());
    
    ASSERT_EQ("142.251.36.99", service.ip_address());
    ASSERT_EQ(80, service.port());
    
    service.disconnect();
    ASSERT_EQ("", service.ip_address());
    ASSERT_EQ(0, service.port());
}

