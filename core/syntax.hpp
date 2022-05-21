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

#pragma once

#include "visitor.hpp"
#include "position.hpp"

#include <map>
#include <vector>

class Module;

class Base
    : public Visitable<>
{
public:
    virtual ~Base() = default;

    Position    where()     {return _where;}
    void        where(Position where)
                            {_where = where;}

private:
    Position    _where;
}; 

class Exception
    : public std::exception
{
public:
    Exception(Position position, std::string info)
        : position(position)
    {
        std::ostringstream  os;

        os << info << " at [" << position.beg.row << ":" << position.beg.col << " .. " << position.end.row << ":" << position.end.col << "]";

        message     = os.str();
    }

    const char* what() const noexcept override 
    {
        return message.c_str();
    }

private:
    Position    position;
    std::string message;
};

class Type
    : public Visitable<Base, Type>
{
};

class TypePrimitive
    : public Visitable<Type, TypePrimitive>
{
};

class TypeComposite
    : public Visitable<Type, TypePrimitive>
{
public:
    virtual Type*   member(std::string name) = 0;
};

class TypeVoid    : public Visitable<TypePrimitive, TypeVoid> {};
class TypeBoolean : public Visitable<TypePrimitive, TypeBoolean> {}; 
class TypeInteger : public Visitable<TypePrimitive, TypeInteger> {}; 
class TypeNumber  : public Visitable<TypePrimitive, TypeNumber> {}; 
class TypeString  : public Visitable<TypePrimitive, TypeString> {}; 

template<typename T>
class Named
{
public:
    Named(std::string name, T* data)
        : name(name)
        , data(data)
    {
    }

    std::string name;
    T*          data;
};

class TypeContext
    : public Visitable<TypeComposite, TypeContext>
{
public:
    TypeContext(Module* module);

    Type*   member(std::string name) override;

private:
    Module* const   _module;
};

class TypeStruct
    : public Visitable<TypeComposite, TypeStruct>
{
public:
    TypeStruct(std::vector<Named<Type>> members);

    std::vector<Named<Type>>        members;

    Type*   member(std::string name) override;

private:
    std::map<std::string, Type*>    _name2type;
};

class TypeArray
    : public Visitable<Type, TypeArray>
{
public:
    TypeArray(Type* type, unsigned size);

    Type* const     type;
    unsigned const  size;   //  if size is 0, array is dynamic otherwise it's static
};

class   TypeEnum
    : public Visitable<TypeComposite, TypeEnum>
{
public:
    TypeEnum(std::vector<std::string>   items);

    Type*   member(std::string name) override;

    std::vector<std::string> const  items;
};   


class Expr
    : public Visitable<Base, Expr>
{
public:
    virtual bool is_temporal() {return false;}

    Type*   type()              {return _type;}
    void    type(Type* type)    {_type = type;}

private:
    Type*       _type   = nullptr;
};

class   TimeInterval
    : public Visitable<Expr, TimeInterval>
{
public:
    TimeInterval(Expr* lo, Expr* hi);

    Expr* const lo;
    Expr* const hi;
};

class   TimeLowerBound
    : public Visitable<TimeInterval, TimeLowerBound>
{
public:
    TimeLowerBound(Expr* lo);
};

class   TimeUpperBound
    : public Visitable<TimeInterval, TimeUpperBound>
{
public:
    TimeUpperBound(Expr* hi);
};

class ExprNullary
    : public Visitable<Expr, ExprNullary>
{
protected:
    ExprNullary();
};

class ExprUnary
    : public Visitable<Expr, ExprUnary>
{
protected:
    ExprUnary(int op, Expr* arg);

    bool is_temporal() override;

public:
    const int   op;
    Expr* const arg;
};

class ExprBinary
    : public Visitable<Expr, ExprBinary>
{
protected:
    ExprBinary(int op, Expr* lhs, Expr* rhs);

    bool is_temporal() override;

public:
    const int   op;
    Expr* const lhs;
    Expr* const rhs;
};

class ExprTernary
    : public Visitable<Expr, ExprTernary>
{
protected:
    ExprTernary(int op, Expr* lhs, Expr* mhs, Expr* rhs);

    bool is_temporal() override;

public:
    const int   op;
    Expr* const lhs;
    Expr* const mhs;
    Expr* const rhs;
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

template<typename Expr>
class Temporal
    : public Visitable<Expr, Temporal<Expr>>
{
public:
    template<typename ... Args>
    Temporal(int op, TimeInterval* time, Args ... args)
        : Visitable<Expr, Temporal<Expr>>(op, args...)
        , time(time)
    {
        //  assert(time != nullptr);    //  TODO: put it back
    }

    template<typename ... Args>
    Temporal(int op, Args ... args)
        : Visitable<Expr, Temporal<Expr>>(op, args...)
        , time(nullptr)
    {
    }

    bool is_temporal() override {return true;}

    TimeInterval* const time    = nullptr;
};

template<int OP, typename Expr>
class SetOper
    : public Visitable<Expr, SetOper<OP, Expr>>
{
public:
    template<typename ... Args>
    SetOper(Args ... args)
        : Visitable<Expr, SetOper<OP, Expr>>(OP, args...)
    {
    }
};

template<typename Class>
class Final final
    : public Visitable<Class, Final<Class>>
{
public:
    template<typename ... Args>
    Final(Args ... args)
        : Visitable<Class, Final<Class>>(args ...)
    {
    }
};

using ExprConstInteger  = Final<ExprConstT<int64_t>>;
using ExprConstNumber   = Final<ExprConstT<double>>;
using ExprConstString   = Final<ExprConstT<std::string>>;
using ExprConstBoolean  = Final<ExprConstT<bool>>;

using ExprParen = Final<SetOper<'()',  ExprUnary>>;

using ExprNeg   = Final<SetOper<'-',   ExprUnary>>;

using ExprAdd   = Final<SetOper<'+',   ExprBinary>>;
using ExprSub   = Final<SetOper<'-',   ExprBinary>>;
using ExprMul   = Final<SetOper<'*',   ExprBinary>>;
using ExprDiv   = Final<SetOper<'/',   ExprBinary>>;
using ExprMod   = Final<SetOper<'%',   ExprBinary>>;

using ExprEq    = Final<SetOper<'==',  ExprBinary>>;
using ExprNe    = Final<SetOper<'!=',  ExprBinary>>;
using ExprGt    = Final<SetOper<'>',   ExprBinary>>;
using ExprGe    = Final<SetOper<'>=',  ExprBinary>>;
using ExprLt    = Final<SetOper<'<',   ExprBinary>>;
using ExprLe    = Final<SetOper<'<=',  ExprBinary>>;

using ExprNot   = Final<SetOper<'!',   ExprUnary>>;

using ExprOr    = Final<SetOper<'||',  ExprBinary>>;
using ExprAnd   = Final<SetOper<'&&',  ExprBinary>>;
using ExprXor   = Final<SetOper<'^',   ExprBinary>>;
using ExprImp   = Final<SetOper<'=>',  ExprBinary>>;
using ExprEqu   = Final<SetOper<'<=>', ExprBinary>>;

using ExprChoice= Final<SetOper<'?:',  ExprTernary>>;

using ExprG     = Final<SetOper<'G',   Temporal<ExprUnary>>>;
using ExprF     = Final<SetOper<'F',   Temporal<ExprUnary>>>;
using ExprXs    = Final<SetOper<'Xs',  Temporal<ExprUnary>>>;
using ExprXw    = Final<SetOper<'Xw',  Temporal<ExprUnary>>>;
using ExprUs    = Final<SetOper<'Us',  Temporal<ExprBinary>>>;
using ExprUw    = Final<SetOper<'Uw',  Temporal<ExprBinary>>>;
using ExprRs    = Final<SetOper<'Rs',  Temporal<ExprBinary>>>;
using ExprRw    = Final<SetOper<'Rw',  Temporal<ExprBinary>>>;
                                                
using ExprH     = Final<SetOper<'H',   Temporal<ExprUnary>>>;
using ExprO     = Final<SetOper<'O',   Temporal<ExprUnary>>>;
using ExprYs    = Final<SetOper<'Ys',  Temporal<ExprUnary>>>;
using ExprYw    = Final<SetOper<'Yw',  Temporal<ExprUnary>>>;
using ExprSs    = Final<SetOper<'Ss',  Temporal<ExprBinary>>>;
using ExprSw    = Final<SetOper<'Sw',  Temporal<ExprBinary>>>;
using ExprTs    = Final<SetOper<'Ts',  Temporal<ExprBinary>>>;
using ExprTw    = Final<SetOper<'Tw',  Temporal<ExprBinary>>>;
using ExprInt   = Final<SetOper<'I',   Temporal<ExprBinary>>>;

class ExprAt final
    : public Visitable<SetOper<'@', ExprUnary>, ExprAt>
{
public:
    ExprAt(std::string name, Expr* expr)
        : Visitable<SetOper<'@', ExprUnary>, ExprAt>(expr)
        , name(name)
    {
    }

public:
    std::string name;
};


class ExprData final
    : public Visitable<ExprNullary, ExprData>
{
public:
    ExprData(std::string name)
        : Visitable<ExprNullary, ExprData>()
        , name(name)
    {
    }

public:
    std::string const   name;
};

class ExprContext final
    : public Visitable<ExprNullary, ExprContext>
{
public:
    ExprContext(std::string name)
        : Visitable<ExprNullary, ExprContext>()
        , name(name)
    {
    }

public:
    std::string const   name;
};

class ExprMmbr final
    : public Visitable<ExprUnary, ExprMmbr>
{
public:
    ExprMmbr(Expr* base, std::string mmbr)
        : Visitable<ExprUnary, ExprMmbr>(0, base)
        , mmbr(mmbr)
    {
    }

public:
    std::string const   mmbr;
};

using ExprIndx  = Final<SetOper<'[]',  ExprBinary>>;


class Data
    : public Visitable<>
{
public:
    virtual ~Data() = default;
};

//  real data coming from a DB
class DataReal
    : public Visitable<Data, DataReal>
{
public:
    DataReal(Type* type);
};

//  temp data, result of an external function call 
class DataTemp
    : public Visitable<Data, DataReal>
{
};

//  result of an expression
class DataExpr
    : public Visitable<Data, DataReal>
{
public:
    DataExpr(Expr* expr);
};
