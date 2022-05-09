#include "syntax.hpp"

ExprNullary::ExprNullary()
{
}

ExprUnary::ExprUnary(int op, Expr* arg)
    : op(op)
    , arg(arg)
{
}

bool ExprUnary::is_temporal()
{
    return arg->is_temporal();
}

ExprBinary::ExprBinary(int op, Expr* lhs, Expr* rhs)
    : op(op)
    , lhs(lhs)
    , rhs(rhs)
{
}

bool ExprBinary::is_temporal()
{
    return lhs->is_temporal() || rhs->is_temporal();
}

ExprTernary::ExprTernary(int op, Expr* lhs, Expr* mhs, Expr* rhs)
    : op(op)
    , lhs(lhs)
    , mhs(mhs)
    , rhs(rhs)
{
}

bool ExprTernary::is_temporal()
{
    return lhs->is_temporal() || mhs->is_temporal() || rhs->is_temporal();
}

TimeInterval::TimeInterval(Expr* lo, Expr* hi)
    : lo(lo)
    , hi(hi)
{
}

TimeLowerBound::TimeLowerBound(Expr* lo)
    : TimeInterval(lo, nullptr)
{
}

TimeUpperBound::TimeUpperBound(Expr* hi)
    : TimeInterval(nullptr, hi)
{
}

TypeStruct::TypeStruct(std::vector<Named<Type>> members)
    : members(members)
{
}

TypeArray::TypeArray(Type* type, unsigned size)
    : type(type)
    , size(size)
{
}

TypeEnum::TypeEnum(std::vector<std::string>   items)
    : items(items)
{
}
