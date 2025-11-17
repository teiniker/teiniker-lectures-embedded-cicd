#include <gtest/gtest.h>
#include <regex>
#include <string>
#include <iostream>

using namespace std;

TEST(RegexGroup, MatchOctalNumberTest) 
{
    regex pattern("0[0-7]+");
    string input = "0715";

    bool match = regex_match(input, pattern);

    EXPECT_TRUE(match);
}

TEST(RegexGroup, MatchHtmlTagTest) 
{
    regex pattern("[<>]");
    string input = "<script>";

    cout << "Input string: " << input << endl;

    bool match = regex_search(input, pattern);

    EXPECT_TRUE(match);
}

// There is no built-in normalization in the C++ standard library
TEST(RegexGroup, MatchHtmlTagEncodingTest) 
{
    regex pattern("[<>]");
    string input = string("\uFF1C") + "script" + "\uFF1E";
    
    cout << "Input string: " << input << endl;

    bool match = regex_search(input, pattern);

    EXPECT_FALSE(match);
}
