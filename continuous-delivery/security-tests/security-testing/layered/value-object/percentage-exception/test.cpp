#include <gtest/gtest.h>
#include "percentage.h"

using namespace std;

TEST(PercentageTest, ValidValue) 
{
    Percentage p(50);
    ASSERT_EQ(50, p.value());
}

TEST(PercentageTest, InvalidValue) 
{
    ASSERT_THROW(Percentage(-10), invalid_argument);
}

TEST(PercentageTest, CopyConstructor) 
{
    Percentage a(42);
    Percentage b = a;  // copy constructor

    ASSERT_EQ(a, b);
}

TEST(PercentageTest, AssignmentOperator) 
{
    Percentage a(15);
    Percentage b(30);

    b = a;  // assignment operator

    ASSERT_EQ(a, b);
}

TEST(PercentageTest, ValuesEqual) 
{
    Percentage a(7);
    Percentage b(7);

    ASSERT_TRUE(a == b);
}

TEST(PercentageTest, ValuesNotEqual) 
{
    Percentage a(7);
    Percentage b(3);

    ASSERT_TRUE(a != b);
}

TEST(PercentageTest, LessThan) 
{
    Percentage a(3);
    Percentage b(7);

    ASSERT_TRUE(a < b);
}

TEST(PercentageTest, GreaterThan) 
{
    Percentage a(9);
    Percentage b(7);

    ASSERT_TRUE(a > b);
}

