#include <gtest/gtest.h>
#include <regex>
#include <string>
#include <iostream>

using namespace std;

TEST(RegexGroup, MatchTest) 
{
    regex pattern("[a-z]+\\d+");
    string input = "hello123";

    bool match = regex_match(input, pattern);

    EXPECT_TRUE(match);
}
