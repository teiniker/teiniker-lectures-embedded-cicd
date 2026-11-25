#include <gtest/gtest.h>
#include <regex>
#include <string>
#include <iostream>

using namespace std;

TEST(RegexNumbers, ValidOctalNumberTest) 
{
    regex pattern("0[0-7]+");
    string input = "0715";

    bool match = regex_match(input, pattern);

    EXPECT_TRUE(match);
}


TEST(RegexNumbers, InValidOctalNumberTest) 
{
    regex pattern("0[0-7]+");
    string input = "0815";

    bool match = regex_match(input, pattern);

    EXPECT_FALSE(match);
}


TEST(RegexNumbers, ValidDecimalNumberTest) 
{
    regex pattern("[-+]?[0-9]+");
    string input = "-999";

    bool match = regex_match(input, pattern);

    EXPECT_TRUE(match);
}

TEST(RegexNumbers, InValidDecimalNumberTest) 
{
    regex pattern("[-+]?[0-9]+");
    string input = "-99A9";

    bool match = regex_match(input, pattern);

    EXPECT_FALSE(match);
}


TEST(RegexNumbers, ValidHexadecimalNumberTest) 
{
    regex pattern("0[xX][0-9a-fA-F]+");
    string input = "0x1A3F";

    bool match = regex_match(input, pattern);

    EXPECT_TRUE(match);
}

TEST(RegexNumbers, InValidHexadecimalNumberTest) 
{
    regex pattern("0[xX][0-9a-fA-F]+");
    string input = "0x1G3F";

    bool match = regex_match(input, pattern);

    EXPECT_FALSE(match);
}

TEST(RegexNumbers, ValidBinaryNumberTest) 
{
    regex pattern("0[bB][01]+");
    string input = "0b101101";

    bool match = regex_match(input, pattern);

    EXPECT_TRUE(match);
}

TEST(RegexNumbers, InValidBinaryNumberTest) 
{
    regex pattern("0[bB][01]+");
    string input = "0b102101";

    bool match = regex_match(input, pattern);

    EXPECT_FALSE(match);
}

TEST(RegexNumbers, ValidFloatingPointNumberTest) 
{
    regex pattern("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?");
    string input = "-3.14e10";

    bool match = regex_match(input, pattern);

    EXPECT_TRUE(match);
}

TEST(RegexNumbers, InValidFloatingPointNumberTest) 
{
    // Using a raw string literal
    regex pattern(R"([-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?)");
    string input = "3.14.15";

    bool match = regex_match(input, pattern);

    EXPECT_FALSE(match);
}