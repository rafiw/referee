#include "syntax.hpp"

ExprUnary::ExprUnary(int op, Expression* arg)
    : op(op)
    , arg(arg)
{
}

ExprBinary::ExprBinary(int op, Expression* lhs, Expression* rhs)
    : op(op)
    , lhs(lhs)
    , rhs(rhs)
{
}

ExprTernary::ExprTernary(int op, Expression* lhs, Expression* mhs, Expression* rhs)
    : op(op)
    , lhs(lhs)
    , mhs(mhs)
    , rhs(rhs)
{
}
