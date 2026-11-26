#include <gtest/gtest.h>
#include <regex>
#include <string>
#include <iostream>

using namespace std;

TEST(RegexSearch, HtmlTagTest) 
{
    regex pattern("[<>]");
    string input = "<script>";

    cout << "Input string: " << input << endl;

    bool found = regex_search(input, pattern);

    EXPECT_TRUE(found);
}

// There is no built-in normalization in the C++ standard library
TEST(RegexSearch, HtmlTagEncodingTest) 
{
    regex pattern("[<>]");
    string input = string("\uFF1C") + "script" + "\uFF1E";
    
    cout << "Input string: " << input << endl;

    bool found = regex_search(input, pattern);

    EXPECT_FALSE(found);
}
