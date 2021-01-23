#pragma once
#include "visitor.hpp"

struct Expression
    : public Visitable<>
{
    virtual ~Expression() = default;
};

struct ExprNullary
    : public Visitable<Expression, ExprNullary>
{
};

struct ExprUnary
    : public Visitable<Expression, ExprUnary>
{
    ExprUnary(int op, Expression* arg);

    const int         op;
    Expression* const arg;
};

struct ExprBinary
    : public Visitable<Expression, ExprBinary>
{
    ExprBinary(int op, Expression* lhs, Expression* rhs);

    const int         op;
    Expression* const lhs;
    Expression* const rhs;
};

struct ExprTernary
    : public Visitable<Expression, ExprTernary>
{
    ExprTernary(int op, Expression* lhs, Expression* mhs, Expression* rhs);

    const int         op;
    Expression* const lhs;
    Expression* const mhs;
    Expression* const rhs;
};

template<typename Type>
class ExprConstT
    : public Visitable<ExprNullary, ExprConstT<Type>>
{
public:
    ExprConstT(Type value) : value(value) {} 
    Type const value;
};

template<typename Type, Type Value>
class ExprConstTV
    : public Visitable<ExprConstT<Type>, ExprConstTV<Type, Value>>
{
public:
    ExprConstTV() : Visitable<ExprConstT<Type>, ExprConstTV<Type, Value>>(Value) {}
};

template<int OP, int NOP = OP>
class ExprUnaryT
    : public Visitable<ExprUnary, ExprUnaryT<OP, NOP>>
{
public:
    ExprUnaryT(Expression* arg)
        : Visitable<ExprUnary, ExprUnaryT<OP, NOP>>(OP, arg)
    {
    }
};

template<int OP>
class ExprBinaryT
    : public Visitable<ExprBinary, ExprBinaryT<OP>>
{
public:
    ExprBinaryT(Expression* lhs, Expression* rhs)
        : Visitable<ExprBinary, ExprBinaryT<OP>>(OP, lhs, rhs)
    {
    }
};

using ExprTrue  = ExprConstTV<bool, true>;
using ExprFalse = ExprConstTV<bool, false>;

using ExprNeg   = ExprUnaryT<'-'>;

using ExprAdd   = ExprBinaryT<'+'>;
using ExprSub   = ExprBinaryT<'-'>;
using ExprMul   = ExprBinaryT<'*'>;
using ExprDiv   = ExprBinaryT<'/'>;

using ExprEQ    = ExprBinaryT<'=='>;
using ExprNE    = ExprBinaryT<'!='>;
using ExprGT    = ExprBinaryT<'>'>;
using ExprGE    = ExprBinaryT<'>='>;
using ExprLT    = ExprBinaryT<'<'>;
using ExprLE    = ExprBinaryT<'<='>;

using ExprNot   = ExprUnaryT<'!'>;

using ExprOr    = ExprBinaryT<'||'>;
using ExprAnd   = ExprBinaryT<'&&'>;
using ExprXor   = ExprBinaryT<'^'>;
using ExprImp   = ExprBinaryT<'=>'>;
using ExprEqu   = ExprBinaryT<'<=>'>;

using ExprG     = ExprUnaryT<'G'>;
using ExprF     = ExprUnaryT<'F'>;
using ExprXs    = ExprUnaryT<'Xs'>;
using ExprXw    = ExprUnaryT<'Xw'>;
using ExprUs    = ExprBinaryT<'Us'>;
using ExprUw    = ExprBinaryT<'Uw'>;
using ExprRs    = ExprBinaryT<'Rs'>;
using ExprRw    = ExprBinaryT<'Rw'>;

using ExprH     = ExprUnaryT<'H'>;
using ExprO     = ExprUnaryT<'O'>;
using ExprYs    = ExprUnaryT<'Ys'>;
using ExprYw    = ExprUnaryT<'Yw'>;
using ExprSs    = ExprBinaryT<'Ss'>;
using ExprSw    = ExprBinaryT<'Sw'>;
using ExprTs    = ExprBinaryT<'Ts'>;
using ExprTw    = ExprBinaryT<'Tw'>;
