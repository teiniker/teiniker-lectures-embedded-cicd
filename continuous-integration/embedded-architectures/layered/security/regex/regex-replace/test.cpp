#include <gtest/gtest.h>
#include <regex>
#include <string>
#include <iostream>

using namespace std;

TEST(RegexReplace, LogStringEncoding1) 
{
    regex pattern(R"([\n\r]+)");
    string input = "bart\n2099-01-01 00:00:00,000 [main] INFO HACK!!!!";
    cout << "Log: " << input << endl;

    string output = regex_replace(input, pattern, "_");

    EXPECT_EQ(output, "bart_2099-01-01 00:00:00,000 [main] INFO HACK!!!!");
}


TEST(RegexReplace, LogStringEncoding2) 
{
    regex pattern(R"([\n\r]+)");
    string input = "2099-01-01 00:00:00,000 [main] \rINFO HACK!!!!";
    cout << "Log: " << input << endl;

    string output = regex_replace(input, pattern, "_");

    EXPECT_EQ(output, "2099-01-01 00:00:00,000 [main] _INFO HACK!!!!");
}
