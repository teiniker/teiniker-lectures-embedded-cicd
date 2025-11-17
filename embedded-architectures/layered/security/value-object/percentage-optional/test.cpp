#include <gtest/gtest.h>
#include "percentage.h"

using namespace std;

TEST(PercentageTest, ValidValue) 
{
    auto p = Percentage::make(50);
    ASSERT_TRUE(p.has_value());
    ASSERT_EQ(50, p.value().value());
}

TEST(PercentageTest, InvalidValue) 
{
    auto p = Percentage::make(-10);
    ASSERT_FALSE(p);
}

TEST(PercentageTest, CopyConstructor) 
{
    auto a = Percentage::make(42);
    auto b = a;  // copy constructor

    ASSERT_EQ(a, b);
}

TEST(PercentageTest, AssignmentOperator) 
{
    auto a = Percentage::make(15);
    auto b = Percentage::make(30);

    b = a;  // assignment operator

    ASSERT_EQ(a, b);
}

TEST(PercentageTest, ValuesEqual) 
{
    auto a = Percentage::make(7);
    auto b = Percentage::make(7);

    ASSERT_TRUE(a == b);
}

TEST(PercentageTest, ValuesNotEqual) 
{
    auto a = Percentage::make(7);
    auto b = Percentage::make(3);

    ASSERT_TRUE(a != b);
}

TEST(PercentageTest, LessThan) 
{
    auto a = Percentage::make(3);
    auto b = Percentage::make(7);

    ASSERT_TRUE(a < b);
}

TEST(PercentageTest, GreaterThan) 
{
    auto a = Percentage::make(9);
    auto b = Percentage::make(7);

    ASSERT_TRUE(a > b);
}

