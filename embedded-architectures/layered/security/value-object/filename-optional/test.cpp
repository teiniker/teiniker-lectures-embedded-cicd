#include <gtest/gtest.h>
#include "filename.h"

using namespace std;

TEST(FilenameTest, ValidValue) 
{
    auto filename_opt = Filename::make("document.txt");
    ASSERT_TRUE(filename_opt.has_value());
    ASSERT_EQ("document.txt", filename_opt->value());
}

TEST(FilenameTest, InvalidValue) 
{
    auto filename_opt = Filename::make("invalid_filename");
    ASSERT_FALSE(filename_opt.has_value());
}

TEST(FilenameTest, CopyConstructor) 
{
    auto filename_opt = Filename::make("document.txt");
    ASSERT_TRUE(filename_opt.has_value());
    Filename a = filename_opt.value();
    Filename b = a;  // copy constructor

    ASSERT_EQ(a.value(), b.value());
}

TEST(FilenameTest, AssignmentOperator) 
{
    auto filename_opt_a = Filename::make("document.txt");
    auto filename_opt_b = Filename::make("another_document.txt");
    ASSERT_TRUE(filename_opt_a.has_value());
    ASSERT_TRUE(filename_opt_b.has_value());

    Filename a = filename_opt_a.value();
    Filename b = filename_opt_b.value();

    b = a;  // assignment operator

    ASSERT_EQ(a.value(), b.value());
}

TEST(FilenameTest, ValuesEqual) 
{
    auto filename_opt_a = Filename::make("document.txt");
    auto filename_opt_b = Filename::make("document.txt");
    ASSERT_TRUE(filename_opt_a.has_value());
    ASSERT_TRUE(filename_opt_b.has_value());

    Filename a = filename_opt_a.value();
    Filename b = filename_opt_b.value();

    ASSERT_TRUE(a == b);
}

TEST(FilenameTest, ValuesNotEqual) 
{
    auto filename_opt_a = Filename::make("document.txt");
    auto filename_opt_b = Filename::make("another_document.txt");
    ASSERT_TRUE(filename_opt_a.has_value());
    ASSERT_TRUE(filename_opt_b.has_value());

    Filename a = filename_opt_a.value();
    Filename b = filename_opt_b.value();

    ASSERT_TRUE(a != b);
}

TEST(FilenameTest, LessThan) 
{
    auto filename_opt_a = Filename::make("adocument.txt");
    auto filename_opt_b = Filename::make("document.txt");
    ASSERT_TRUE(filename_opt_a.has_value());
    ASSERT_TRUE(filename_opt_b.has_value());

    Filename a = filename_opt_a.value();
    Filename b = filename_opt_b.value();

    ASSERT_TRUE(a < b);
}

TEST(FilenameTest, GreaterThan) 
{
    auto filename_opt_a = Filename::make("document.txt");
    auto filename_opt_b = Filename::make("adocument.txt");
    ASSERT_TRUE(filename_opt_a.has_value());
    ASSERT_TRUE(filename_opt_b.has_value());

    Filename a = filename_opt_a.value();
    Filename b = filename_opt_b.value();

    ASSERT_TRUE(a > b);
}

