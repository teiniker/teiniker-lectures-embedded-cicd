#include <gtest/gtest.h>
#include "filename.h"

using namespace std;

TEST(FilenameTest, ValidValue) 
{
    Filename filename("document.txt");
    ASSERT_EQ("document.txt", filename.value());
}

TEST(FilenameTest, InvalidValue) 
{
    ASSERT_THROW(Filename("invalid_filename"), invalid_argument);
}

TEST(FilenameTest, CopyConstructor) 
{
    Filename a("document.txt");
    Filename b = a;  // copy constructor

    ASSERT_EQ(a.value(), b.value());
}

TEST(FilenameTest, AssignmentOperator) 
{
    Filename a("document.txt");
    Filename b("another_document.txt");

    b = a;  // assignment operator

    ASSERT_EQ(a.value(), b.value());
}

TEST(FilenameTest, ValuesEqual) 
{
    Filename a("document.txt");
    Filename b("document.txt");

    ASSERT_TRUE(a == b);
}

TEST(FilenameTest, ValuesNotEqual) 
{
    Filename a("document.txt");
    Filename b("another_document.txt");

    ASSERT_TRUE(a != b);
}

TEST(FilenameTest, LessThan) 
{
    Filename a("adocument.txt");
    Filename b("document.txt");

    ASSERT_TRUE(a < b);
}

TEST(FilenameTest, GreaterThan) 
{
    Filename a("document.txt");
    Filename b("adocument.txt");

    ASSERT_TRUE(a > b);
}

