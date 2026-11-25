#include <gtest/gtest.h>
#include <regex>
#include <string>
#include <iostream>

using namespace std;

TEST(RegexStrings, ValidUsernameTest) 
{
    regex pattern("[a-zA-Z0-9_]{8,16}");
    string input = "lisa_simpson";

    bool match = regex_match(input, pattern);

    EXPECT_TRUE(match);
}

TEST(RegexStrings, InvalidUsernameTest) 
{
    regex pattern("[a-zA-Z0-9_]{8,16}");
    string input = "lisa simpson";
    
    bool match = regex_search(input, pattern);

    EXPECT_FALSE(match);
}


TEST(RegexStrings, ValidFilenameTest) 
{
    regex pattern(R"([a-zA-Z0-9_]{1,255}\.[a-zA-Z0-9]{0,3})");
    string input = "document.txt";

    bool match = regex_match(input, pattern);
 
    EXPECT_TRUE(match);
}

TEST(RegexStrings, InvalidFilenameTest) 
{
    regex pattern(R"([a-zA-Z0-9_]{1,255}\.[a-zA-Z0-9]{0,3})");
    string input = "document.backup.txt";

    bool match = regex_match(input, pattern);

    EXPECT_FALSE(match);
}


TEST(RegexStrings, InvalidMailTest) 
{
    regex pattern(R"([a-z0-9._%+-]+@[a-z0-9.-]+\.[a-z]{2,4})");
    string input = "lisa.simpson@springfield.com";

    bool match = regex_match(input, pattern);

    EXPECT_TRUE(match);
}

TEST(RegexStrings, ValidMailTest) 
{
    regex pattern(R"([a-z0-9._%+-]+@[a-z0-9.-]+\.[a-z]{2,4})");
    string input = "lisa.simpson@springfield";

    bool match = regex_match(input, pattern);

    EXPECT_FALSE(match);
}