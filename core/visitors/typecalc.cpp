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

#include "typecalc.hpp"
#include "../factory.hpp"

#include <exception>

struct TypeCalcImpl
    : Visitor< ExprConstBoolean
             , ExprConstInteger
             , ExprConstNumber
             , ExprConstString
             , ExprContext
             , ExprParen
             , ExprData
             , ExprMmbr
             , ExprIndx
             , ExprEq
             , ExprNe
             , ExprGt
             , ExprGe
             , ExprLt
             , ExprLe
             , ExprNot
             , ExprOr
             , ExprAnd
             , ExprXor
             , ExprImp
             , ExprEqu
             , ExprAdd
             , ExprSub
             , ExprDiv
             , ExprMul
             , ExprMod
             , Temporal<ExprUnary>
             , Temporal<ExprBinary>
             , Time
             , ExprAt>
{
    TypeCalcImpl(Module* module)
        : m_module(module)
    {
    }
    
    Type*   m_type  = nullptr;

    Type*   make(Expr*          expr);

    void    visit(ExprAdd*              expr) override;
    void    visit(ExprAnd*              expr) override;
    void    visit(ExprAt*               expr) override;
    void    visit(ExprConstBoolean*     expr) override;
    void    visit(ExprConstInteger*     expr) override;
    void    visit(ExprConstNumber*      expr) override;
    void    visit(ExprConstString*      expr) override;
    void    visit(ExprContext*          expr) override;
    void    visit(ExprData*             expr) override;
    void    visit(ExprDiv*              expr) override;
    void    visit(ExprEq*               expr) override;
    void    visit(ExprEqu*              expr) override;
    void    visit(ExprGe*               expr) override;
    void    visit(ExprGt*               expr) override;
    void    visit(ExprImp*              expr) override;
    void    visit(ExprLe*               expr) override;
    void    visit(ExprLt*               expr) override;
    void    visit(ExprMmbr*             expr) override;
    void    visit(ExprIndx*             expr) override;
    void    visit(ExprMod*              expr) override;
    void    visit(ExprMul*              expr) override;
    void    visit(ExprNe*               expr) override;
    void    visit(ExprNot*              expr) override;
    void    visit(ExprOr*               expr) override;
    void    visit(ExprParen*            expr) override;
    void    visit(ExprSub*              expr) override;
    void    visit(ExprXor*              expr) override;
    void    visit(Temporal<ExprBinary>* expr) override;
    void    visit(Temporal<ExprUnary>*  expr) override;
    void    visit(Time*                 time) override;

    Type*    boolBool2Bool(  
                    Expr*       expr,
                    Expr*       lhs,
                    Expr*       rhs);
    Type*   bool2Bool(  
                    Expr*       expr,
                    Expr*       arg);
    Type*   nmbrNmbr2Bool(  
                    Expr*       expr,
                    Expr*       lhs,
                    Expr*       rhs);

    Type*   typeBoolean = Factory<TypeBoolean>::create();
    Type*   typeInteger = Factory<TypeInteger>::create();
    Type*   typeNumber  = Factory<TypeNumber>::create();
    Type*   typeString  = Factory<TypeString>::create();
    Type*   typeVoid    = Factory<TypeVoid>::create();

private:
    Module* m_module;
};

void    TypeCalcImpl::visit(ExprAdd*                expr)
{
    auto    lhs = make(expr->lhs);
    auto    rhs = make(expr->rhs);

    if(lhs != typeInteger && lhs != typeNumber && rhs != typeInteger && rhs != typeNumber)
    {
        throw Exception(expr->where(), "bad type");
    }

    if(lhs == rhs)
        m_type  = lhs;
    else 
        m_type  = typeNumber;
}

void    TypeCalcImpl::visit(ExprAnd*                expr)
{
    m_type = boolBool2Bool(expr, expr->lhs, expr->rhs);
}

void    TypeCalcImpl::visit(ExprAt*                 expr)
{
    m_type = make(expr->arg);
}

void    TypeCalcImpl::visit(ExprConstBoolean*       expr)
{
    m_type = Factory<TypeBoolean>::create();
}

void    TypeCalcImpl::visit(ExprConstInteger*       expr)
{
    m_type = Factory<TypeInteger>::create();
}

void    TypeCalcImpl::visit(ExprConstNumber*        expr)
{
    m_type = Factory<TypeNumber>::create();
}

void    TypeCalcImpl::visit(ExprConstString*        expr)
{
    m_type = Factory<TypeString>::create();
}

void    TypeCalcImpl::visit(ExprContext*            expr)
{
    m_type = Factory<TypeContext>::create(m_module);
}

void    TypeCalcImpl::visit(ExprDiv*                expr)
{
    auto    lhs = make(expr->lhs);
    auto    rhs = make(expr->rhs);

    if(lhs != typeInteger && lhs != typeNumber && rhs != typeInteger && rhs != typeNumber)
    {
        throw Exception(expr->where(), "bad type");
    }

    if(lhs == rhs)
        m_type  = lhs;
    else 
        m_type  = typeNumber;
}

void    TypeCalcImpl::visit(ExprEq*                 expr)
{
    m_type = nmbrNmbr2Bool(expr, expr->lhs, expr->rhs);
}

void    TypeCalcImpl::visit(ExprEqu*                expr)
{
    m_type = boolBool2Bool(expr, expr->lhs, expr->rhs);
}

void    TypeCalcImpl::visit(ExprGe*                 expr)
{
    m_type = nmbrNmbr2Bool(expr, expr->lhs, expr->rhs);
}

void    TypeCalcImpl::visit(ExprGt*                 expr)
{
    m_type = nmbrNmbr2Bool(expr, expr->lhs, expr->rhs);
}

void    TypeCalcImpl::visit(ExprImp*                expr)
{
    m_type = boolBool2Bool(expr, expr->lhs, expr->rhs);
}

void    TypeCalcImpl::visit(ExprLe*                 expr)
{
    m_type = nmbrNmbr2Bool(expr, expr->lhs, expr->rhs);
}

void    TypeCalcImpl::visit(ExprLt*                 expr)
{
    m_type = nmbrNmbr2Bool(expr, expr->lhs, expr->rhs);
}

void    TypeCalcImpl::visit(ExprMmbr*               expr)
{
    auto    base    = dynamic_cast<TypeComposite*>(make(expr->arg));

    if(base == nullptr)
    {
        throw Exception(expr->where(), "bad type");
    }

    m_type  = base->member(expr->mmbr);

    if(m_type == nullptr)
    {
        throw Exception(expr->where(), "no such a member");
    }
}

void    TypeCalcImpl::visit(ExprIndx*               expr)
{
    auto    lhs     = make(expr->lhs);
    auto    rhs     = make(expr->rhs);
    auto    base    = dynamic_cast<TypeArray*>(lhs);

    if(base == nullptr)
    {
        throw Exception(expr->where(), "bad type");
    }

    if(rhs != Factory<TypeInteger>::create())
    {
        throw Exception(expr->where(), "index should be integer");
    }

    m_type  = base->type;
}


void    TypeCalcImpl::visit(ExprData*               expr)
{
    auto    ctxt    = dynamic_cast<TypeComposite*>(make(expr->ctxt));

    if(ctxt == nullptr)
    {
        throw Exception(expr->where(), "bad type");
    }

    m_type  = ctxt->member(expr->name);

    if(m_type == nullptr)
    {
        throw Exception(expr->where(), "no such a member");
    }
}

void    TypeCalcImpl::visit(ExprMod*                expr)
{
    auto    lhs = make(expr->lhs);
    auto    rhs = make(expr->rhs);

    if(lhs != typeInteger && rhs != typeInteger)
    {
        throw Exception(expr->where(), "bad type");
    }

    m_type  = typeInteger;
}


void    TypeCalcImpl::visit(ExprMul*                expr)
{
    auto    lhs = make(expr->lhs);
    auto    rhs = make(expr->rhs);

    if(lhs != typeInteger && lhs != typeNumber && rhs != typeInteger && rhs != typeNumber)
    {
        throw Exception(expr->where(), "bad type");
    }

    if(lhs == rhs)
        m_type  = lhs;
    else 
        m_type  = typeNumber;
}

void    TypeCalcImpl::visit(ExprNe*                 expr)
{
    m_type = nmbrNmbr2Bool(expr, expr->lhs, expr->rhs);
}

void    TypeCalcImpl::visit(ExprNot*                expr)
{
    m_type = bool2Bool(expr, expr->arg);
}

void    TypeCalcImpl::visit(ExprOr*                 expr)
{
    m_type = boolBool2Bool(expr, expr->lhs, expr->rhs);
}

void    TypeCalcImpl::visit(ExprParen*              expr)
{
    m_type  = make(expr->arg);
}

void    TypeCalcImpl::visit(ExprSub*                expr)
{
    auto    lhs = make(expr->lhs);
    auto    rhs = make(expr->rhs);

    if(lhs != typeInteger && lhs != typeNumber && rhs != typeInteger && rhs != typeNumber)
    {
        throw Exception(expr->where(), "bad type");
    }

    if(lhs == rhs)
        m_type  = lhs;
    else 
        m_type  = typeNumber;
}

void    TypeCalcImpl::visit(ExprXor*                expr)
{   
    m_type = boolBool2Bool(expr, expr->lhs, expr->rhs);
}

void    TypeCalcImpl::visit(Temporal<ExprBinary>*   expr)
{
    if(expr->time)
    {
        make(expr->time);
    }

    m_type = boolBool2Bool(expr, expr->lhs, expr->rhs);
}

void    TypeCalcImpl::visit(Temporal<ExprUnary>*    expr)
{
    if(expr->time)
    {
        make(expr->time);
    }

    m_type = bool2Bool(expr, expr->arg);
}


void    TypeCalcImpl::visit(Time*                   time)
{
    auto    lo  = time->lo ? TypeCalcImpl::make(time->lo) : typeInteger;
    auto    hi  = time->hi ? TypeCalcImpl::make(time->hi) : typeInteger;
    
    if (lo != typeInteger)
        throw std::runtime_error(__PRETTY_FUNCTION__);

    if (hi != typeInteger)
        throw std::runtime_error(__PRETTY_FUNCTION__);

    m_type = typeVoid;
}


Type*   TypeCalcImpl::boolBool2Bool(  
                    Expr*       expr,
                    Expr*       lhs,
                    Expr*       rhs)
{
    auto    typeLhs = make(lhs);    
    auto    typeRhs = make(rhs);

    if(typeLhs != typeBoolean || typeRhs != typeBoolean)
    {
        throw Exception(expr->where(), "bad type");
    }

    return  typeBoolean;
}

Type*   TypeCalcImpl::bool2Bool(  
                    Expr*       expr,
                    Expr*       arg)
{
    auto    typeArg = make(arg);    

    if(typeArg != typeBoolean)
    {
        throw Exception(expr->where(), "bad type");
    }

    return  typeBoolean;
}

Type*   TypeCalcImpl::nmbrNmbr2Bool(  
                    Expr*       expr,
                    Expr*       lhs,
                    Expr*       rhs)
{
    auto    typeLhs = make(lhs);    
    auto    typeRhs = make(rhs);

    if(     (typeLhs == typeInteger || typeLhs == typeNumber)
        &&  (typeRhs == typeInteger || typeRhs == typeNumber)
        ||  (typeLhs == typeBoolean && typeRhs == typeBoolean)
        ||  (typeLhs == typeString  && typeRhs == typeString))
        
    {
        return  typeBoolean;
    }
    
    throw Exception(expr->where(), "bad type");
}

Type*   TypeCalcImpl::make(Expr* expr)
{
    if(expr->type() == nullptr)
    {
        auto    save    = m_type;

        m_type = nullptr;
        expr->accept(*this);
        expr->type(m_type);
    
        m_type  = save;
    }
    
    return expr->type();
}

Type*   TypeCalc::make(Module* module, Expr* expr)
{
    TypeCalcImpl impl(module);

    return  impl.make(expr);
}
