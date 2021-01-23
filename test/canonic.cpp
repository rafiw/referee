#include "gtest/gtest.h"
#include "core/canonic.hpp"
#include "core/factory.hpp"
#include <iostream>

class testCanonic
    : public ::testing::Test
{
public:
    void    SetUp()
    {
        t   = Factory<ExprTrue>::create();
        f   = Factory<ExprFalse>::create();
    }

    Expression* t = nullptr;
    Expression* f = nullptr;
};


TEST_F(testCanonic, True)
{
    auto    inp = t;
    auto    exp = t;
    EXPECT_EQ(Canonic::make(inp), exp);
}

TEST_F(testCanonic, NotTrue)
{
    auto    inp = Factory<ExprNot>::create(t);
    auto    exp = f;
    EXPECT_EQ(Canonic::make(inp), exp);
}

TEST_F(testCanonic, False)
{
    auto    inp = f;
    auto    exp = f;
    EXPECT_EQ(Canonic::make(inp), exp);
}

TEST_F(testCanonic, NotFalse)
{
    auto    inp = Factory<ExprNot>::create(f);
    auto    exp = t;
    EXPECT_EQ(Canonic::make(inp), exp);
}

TEST_F(testCanonic, G)
{
    auto    inp = Factory<ExprG>::create(t);
    auto    exp = Factory<ExprRw>::create(f, t);
    EXPECT_EQ(Canonic::make(inp), exp);
}

TEST_F(testCanonic, F)
{
    auto    inp = Factory<ExprF>::create(f);
    auto    exp = Factory<ExprUs>::create(t, f);
    EXPECT_EQ(Canonic::make(inp), exp);
}
