/*
 *  MIT License
 *  
 *  Copyright (c) 2022 Michael Rolnik
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

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
    : Visitable<Expr, TimeInterval>()
    , lo(lo)
    , hi(hi)
{
}

TimeLowerBound::TimeLowerBound(Expr* lo)
    : Visitable<TimeInterval, TimeLowerBound>(lo, nullptr)
{
}

TimeUpperBound::TimeUpperBound(Expr* hi)
    : Visitable<TimeInterval, TimeUpperBound>(nullptr, hi)
{
}

TypeStruct::TypeStruct(std::vector<Named<Type>> members)
    : members(members)
{
    for(auto member: members)
    {
        _name2type[member.name] = member.data;
    }
}

Type*   TypeStruct::member(std::string name)
{
    if(_name2type.contains(name))
    {
        return _name2type[name];
    }

    return nullptr;
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
