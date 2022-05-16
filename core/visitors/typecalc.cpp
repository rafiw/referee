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
    : Visitor< ExprTrue
             , ExprFalse
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
             , Temporal<ExprUnary>
             , Temporal<ExprBinary>>
{
    Type*   type    = nullptr;

    Type*   make(Expr*          expr);

    void    visit(ExprTrue*     expr) override;
    void    visit(ExprFalse*    expr) override;
    void    visit(ExprEq*       expr) override;
    void    visit(ExprNe*       expr) override;
    void    visit(ExprGt*       expr) override;
    void    visit(ExprGe*       expr) override;
    void    visit(ExprLt*       expr) override;
    void    visit(ExprLe*       expr) override;
    void    visit(ExprNot*      expr) override;
    void    visit(ExprOr*       expr) override;
    void    visit(ExprAnd*      expr) override;
    void    visit(ExprXor*      expr) override;
    void    visit(ExprImp*      expr) override;
    void    visit(ExprEqu*      expr) override;
    void    visit(Temporal<ExprUnary>*    expr) override;
    void    visit(Temporal<ExprBinary>*   expr) override;

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
};

Type*   TypeCalcImpl::boolBool2Bool(  
                    Expr*       expr,
                    Expr*       lhs,
                    Expr*       rhs)
{
    auto    typeLhs = TypeCalc::make(lhs);    
    auto    typeRhs = TypeCalc::make(lhs);

    if(typeLhs != typeBoolean || typeRhs != typeBoolean)
    {
        throw Exception(expr->position(), "bad type");
    }

    return  typeBoolean;
}

Type*   TypeCalcImpl::bool2Bool(  
                    Expr*       expr,
                    Expr*       arg)
{
    auto    typeArg = TypeCalc::make(arg);    

    if(typeArg != typeBoolean)
    {
        throw Exception(expr->position(), "bad type");
    }

    return  typeBoolean;
}

Type*   TypeCalcImpl::nmbrNmbr2Bool(  
                    Expr*       expr,
                    Expr*       lhs,
                    Expr*       rhs)
{
    auto    typeLhs = TypeCalc::make(lhs);    
    auto    typeRhs = TypeCalc::make(lhs);

    if(     (typeLhs == typeInteger || typeLhs == typeNumber)
        &&  (typeRhs == typeInteger || typeRhs == typeNumber)
        ||  (typeLhs == typeBoolean && typeRhs == typeBoolean)
        ||  (typeLhs == typeString  && typeRhs == typeString))
        
    {
        return  typeBoolean;
    }
    
    throw Exception(expr->position(), "bad type");
}

void    TypeCalcImpl::visit(ExprTrue*   expr) {type = typeBoolean;}
void    TypeCalcImpl::visit(ExprFalse*  expr) {type = typeBoolean;}

void    TypeCalcImpl::visit(ExprEq*     expr) {type = nmbrNmbr2Bool(expr, expr->lhs, expr->rhs);}
void    TypeCalcImpl::visit(ExprNe*     expr) {type = nmbrNmbr2Bool(expr, expr->lhs, expr->rhs);}

void    TypeCalcImpl::visit(ExprGt*     expr) {type = nmbrNmbr2Bool(expr, expr->lhs, expr->rhs);}
void    TypeCalcImpl::visit(ExprGe*     expr) {type = nmbrNmbr2Bool(expr, expr->lhs, expr->rhs);}

void    TypeCalcImpl::visit(ExprLt*     expr) {type = nmbrNmbr2Bool(expr, expr->lhs, expr->rhs);}
void    TypeCalcImpl::visit(ExprLe*     expr) {type = nmbrNmbr2Bool(expr, expr->lhs, expr->rhs);}

void    TypeCalcImpl::visit(ExprNot*    expr) {type = bool2Bool(expr, expr->arg);}

void    TypeCalcImpl::visit(ExprOr*     expr) {type = boolBool2Bool(expr, expr->lhs, expr->rhs);}
void    TypeCalcImpl::visit(ExprAnd*    expr) {type = boolBool2Bool(expr, expr->lhs, expr->rhs);}

void    TypeCalcImpl::visit(ExprXor*    expr) {type = boolBool2Bool(expr, expr->lhs, expr->rhs);}
void    TypeCalcImpl::visit(ExprEqu*    expr) {type = boolBool2Bool(expr, expr->lhs, expr->rhs);}

void    TypeCalcImpl::visit(ExprImp*    expr) {type = boolBool2Bool(expr, expr->lhs, expr->rhs);}

void    TypeCalcImpl::visit(Temporal<ExprUnary>*    expr) {type = bool2Bool(expr, expr->arg);}
void    TypeCalcImpl::visit(Temporal<ExprBinary>*   expr) {type = boolBool2Bool(expr, expr->lhs, expr->rhs);}

Type*   TypeCalcImpl::make(Expr* expr)
{
    if(expr->type() == nullptr)
    {
        type = nullptr;
        expr->accept(*this);
        expr->type(type);
    }
    
    return expr->type();
}


Type*   TypeCalc::make(Expr* expr)
{
    TypeCalcImpl impl;

    return  impl.make(expr);
}
