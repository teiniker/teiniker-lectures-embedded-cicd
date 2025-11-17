#include <gtest/gtest.h>
#include "percentage.h"

using namespace std;

TEST(PercentageTest, ValidValue) 
{
    auto p = Percentage::make(50);
    ASSERT_EQ(50, p.value());
}

TEST(PercentageTest, InvalidValue) 
{
    ASSERT_THROW(Percentage::make(-10), invalid_argument);
}

TEST(PercentageTest, CopyConstructor) 
{
    Percentage a = Percentage::make(42);
    Percentage b = a;  // copy constructor

    ASSERT_EQ(a, b);
}

TEST(PercentageTest, AssignmentOperator) 
{
    Percentage a = Percentage::make(15);
    Percentage b = Percentage::make(30);

    b = a;  // assignment operator

    ASSERT_EQ(a, b);
}

TEST(PercentageTest, ValuesEqual) 
{
    Percentage a = Percentage::make(7);
    Percentage b = Percentage::make(7);

    ASSERT_TRUE(a == b);
}

TEST(PercentageTest, ValuesNotEqual) 
{
    Percentage a = Percentage::make(7);
    Percentage b = Percentage::make(3);

    ASSERT_TRUE(a != b);
}

TEST(PercentageTest, LessThan) 
{
    Percentage a = Percentage::make(3);
    Percentage b = Percentage::make(7);

    ASSERT_TRUE(a < b);
}

TEST(PercentageTest, GreaterThan) 
{
    Percentage a = Percentage::make(9);
    Percentage b = Percentage::make(7);

    ASSERT_TRUE(a > b);
}

