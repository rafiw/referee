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

#include "canonic.hpp"
#include "negated.hpp"
#include "../factory.hpp"

struct CanonicImpl
    : Visitor< Expr
             , ExprConstBoolean
             , ExprAt
             , ExprNot
             , ExprOr
             , ExprAnd
             , ExprXor
             , ExprImp
             , ExprEqu
             , ExprG
             , ExprF
             , ExprXs
             , ExprXw
             , ExprUs
             , ExprUw
             , ExprRs
             , ExprRw
             , ExprH
             , ExprO
             , ExprYs
             , ExprYw
             , ExprSs
             , ExprSw
             , ExprTs
             , ExprTw>
{
    Expr* canonic = nullptr;

    Expr* t = Factory<ExprConstBoolean>::create(true);
    Expr* f = Factory<ExprConstBoolean>::create(false);

    Expr* make(Expr* expr);

    void        visit(Expr*   expr) override;
    void        visit(ExprConstBoolean* expr) override;

    void        visit(ExprAt*       expr) override;
    void        visit(ExprNot*      expr) override;
    void        visit(ExprOr*       expr) override;
    void        visit(ExprAnd*      expr) override;
    void        visit(ExprXor*      expr) override;
    void        visit(ExprImp*      expr) override;
    void        visit(ExprEqu*      expr) override;
    void        visit(ExprG*        expr) override;
    void        visit(ExprF*        expr) override;
    void        visit(ExprXs*       expr) override;
    void        visit(ExprXw*       expr) override;
    void        visit(ExprUs*       expr) override;
    void        visit(ExprUw*       expr) override;
    void        visit(ExprRs*       expr) override;
    void        visit(ExprRw*       expr) override;
    void        visit(ExprH*        expr) override;
    void        visit(ExprO*        expr) override;
    void        visit(ExprYs*       expr) override;
    void        visit(ExprYw*       expr) override;
    void        visit(ExprSs*       expr) override;
    void        visit(ExprSw*       expr) override;
    void        visit(ExprTs*       expr) override;
    void        visit(ExprTw*       expr) override;
};

void    CanonicImpl::visit(Expr*  expr) {canonic = expr;}
void    CanonicImpl::visit(ExprConstBoolean*    expr) {canonic = expr;}
void    CanonicImpl::visit(ExprAt*     expr)  {canonic = Factory<ExprAt>::create(expr->name, make(expr->arg));}
void    CanonicImpl::visit(ExprNot*     expr) {canonic = Negated::make(expr->arg);}
void    CanonicImpl::visit(ExprOr*      expr) {canonic = Factory<ExprOr>::create(make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprAnd*     expr) {canonic = Factory<ExprAnd>::create(make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprXor*     expr) {canonic = Factory<ExprXor>::create(make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprImp*     expr) {canonic = Factory<ExprOr>::create(make(Negated::make(expr->lhs)), make(expr->rhs));}
void    CanonicImpl::visit(ExprEqu*     expr) {}
void    CanonicImpl::visit(ExprG*       expr) {canonic = Factory<ExprRw>::create(expr->time, f, make(expr->arg));}
void    CanonicImpl::visit(ExprF*       expr) {canonic = Factory<ExprUs>::create(expr->time, t, make(expr->arg));}
void    CanonicImpl::visit(ExprXs*      expr) {canonic = Factory<ExprXs>::create(expr->time, make(expr->arg));}
void    CanonicImpl::visit(ExprXw*      expr) {canonic = Factory<ExprXw>::create(expr->time, make(expr->arg));}
void    CanonicImpl::visit(ExprUs*      expr) {canonic = Factory<ExprUs>::create(expr->time, make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprUw*      expr) {canonic = Factory<ExprUw>::create(expr->time, make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprRs*      expr) {canonic = Factory<ExprRs>::create(expr->time, make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprRw*      expr) {canonic = Factory<ExprRw>::create(expr->time, make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprH*       expr) {canonic = Factory<ExprTw>::create(expr->time, f, make(expr->arg));}
void    CanonicImpl::visit(ExprO*       expr) {canonic = Factory<ExprSs>::create(expr->time, t, make(expr->arg));}
void    CanonicImpl::visit(ExprYs*      expr) {canonic = Factory<ExprYs>::create(expr->time, make(expr->arg));}
void    CanonicImpl::visit(ExprYw*      expr) {canonic = Factory<ExprYw>::create(expr->time, make(expr->arg));}
void    CanonicImpl::visit(ExprSs*      expr) {canonic = Factory<ExprSs>::create(expr->time, make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprSw*      expr) {canonic = Factory<ExprSw>::create(expr->time, make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprTs*      expr) {canonic = Factory<ExprTs>::create(expr->time, make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprTw*      expr) {canonic = Factory<ExprTw>::create(expr->time, make(expr->lhs), make(expr->rhs));}

Expr* CanonicImpl::make(Expr* expr)
{
    canonic = expr;

    expr->accept(*this);

    return  canonic;
}

Expr* Canonic::make(Expr* expr)
{
    CanonicImpl impl;

    return impl.make(expr);
}

