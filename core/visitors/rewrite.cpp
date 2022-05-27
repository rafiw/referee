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

#include "rewrite.hpp"
#include "negated.hpp"
#include "../factory.hpp"
#include "../builder.hpp"

#include <exception>

struct RewriteImpl
    : Visitor< ExprAdd
             , ExprAnd
             , ExprAt
             , ExprChoice
             , ExprConstBoolean
             , ExprConstInteger
             , ExprConstNumber
             , ExprConstString
             , ExprContext
             , ExprData
             , ExprDiv
             , ExprEq
             , ExprEqu
             , ExprF
             , ExprG
             , ExprGe
             , ExprGt
             , ExprH
             , ExprImp
             , ExprIndx
             , ExprInt
             , ExprLe
             , ExprLt
             , ExprMmbr
             , ExprMod
             , ExprMul
             , ExprNe
             , ExprNeg
             , ExprNot
             , ExprO
             , ExprOr
             , ExprParen
             , ExprRs
             , ExprRw
             , ExprSs
             , ExprSub
             , ExprSw
             , ExprTs
             , ExprTw
             , ExprUs
             , ExprUw
             , ExprXor
             , ExprXs
             , ExprXw
             , ExprYs
             , ExprYw
             , Time
             , TimeMin
             , TimeMax>

{
public:
    Expr*   make(   Expr*           expr);
    Time*   make(   Time*           time);

    void    visit(ExprAdd*          expr) override;
    void    visit(ExprAnd*          expr) override;
    void    visit(ExprAt*           expr) override;
    void    visit(ExprChoice*       expr) override;
    void    visit(ExprConstBoolean* expr) override;
    void    visit(ExprConstInteger* expr) override;
    void    visit(ExprConstNumber*  expr) override;
    void    visit(ExprConstString*  expr) override;
    void    visit(ExprContext*      expr) override;
    void    visit(ExprData*         expr) override;
    void    visit(ExprDiv*          expr) override;
    void    visit(ExprEq*           expr) override;
    void    visit(ExprEqu*          expr) override;
    void    visit(ExprF*            expr) override;
    void    visit(ExprG*            expr) override;
    void    visit(ExprGe*           expr) override;
    void    visit(ExprGt*           expr) override;
    void    visit(ExprH*            expr) override;
    void    visit(ExprImp*          expr) override;
    void    visit(ExprIndx*         expr) override;
    void    visit(ExprInt*          expr) override;
    void    visit(ExprLe*           expr) override;
    void    visit(ExprLt*           expr) override;
    void    visit(ExprMmbr*         expr) override;
    void    visit(ExprMod*          expr) override;
    void    visit(ExprMul*          expr) override;
    void    visit(ExprNe*           expr) override;
    void    visit(ExprNeg*          expr) override;
    void    visit(ExprNot*          expr) override;
    void    visit(ExprO*            expr) override;
    void    visit(ExprOr*           expr) override;
    void    visit(ExprParen*        expr) override;
    void    visit(ExprRs*           expr) override;
    void    visit(ExprRw*           expr) override;
    void    visit(ExprSs*           expr) override;
    void    visit(ExprSub*          expr) override;
    void    visit(ExprSw*           expr) override;
    void    visit(ExprTs*           expr) override;
    void    visit(ExprTw*           expr) override;
    void    visit(ExprUs*           expr) override;
    void    visit(ExprUw*           expr) override;
    void    visit(ExprXor*          expr) override;
    void    visit(ExprXs*           expr) override;
    void    visit(ExprXw*           expr) override;
    void    visit(ExprYs*           expr) override;
    void    visit(ExprYw*           expr) override;

    void    visit(Time*             expr) override;
    void    visit(TimeMin*          expr) override;
    void    visit(TimeMax*          expr) override;

private:
    Expr*   m_expr  = nullptr;
    Time*   m_time  = nullptr;
};

void    RewriteImpl::visit( ExprAdd*            expr)
{
    m_expr =  Factory<ExprAdd>::create(
        expr->where(),
        make(expr->lhs),
        make(expr->rhs));
}

void    RewriteImpl::visit( ExprAnd*            expr)
{
    m_expr =  Factory<ExprAnd>::create(
        expr->where(),
        make(expr->lhs),
        make(expr->rhs));
}

void    RewriteImpl::visit( ExprAt*             expr) 
{
    m_expr =  Factory<ExprAt>::create(
        expr->where(),
        expr->name,
        make(expr->arg));
}

void    RewriteImpl::visit( ExprChoice*         expr) 
{
    m_expr =  Factory<ExprChoice>::create(
        expr->where(),
        make(expr->lhs),
        make(expr->mhs),
        make(expr->rhs));
}

void    RewriteImpl::visit( ExprConstBoolean*   expr) 
{
    m_expr =  Factory<ExprConstBoolean>::create(
        expr->where(),
        expr->value);
}

void    RewriteImpl::visit( ExprConstInteger*   expr) 
{
    m_expr =  Factory<ExprConstInteger>::create(
        expr->where(),
         expr->value);
}

void    RewriteImpl::visit( ExprConstNumber*    expr) 
{
    m_expr =  Factory<ExprConstNumber>::create(
        expr->where(),
        expr->value);
}

void    RewriteImpl::visit( ExprConstString*    expr)
{
    m_expr =  Factory<ExprConstString>::create(
        expr->where(),
        expr->value);
}

void    RewriteImpl::visit( ExprContext*        expr)
{
    m_expr =  Factory<ExprContext>::create(
        expr->where(),
        expr->name);
}
void    RewriteImpl::visit( ExprData*           expr)
{
    m_expr =  Factory<ExprData>::create(
        expr->where(),
        expr->name);
}

void    RewriteImpl::visit( ExprDiv*            expr)
{
    m_expr =  Factory<ExprDiv>::create(
        expr->where(),
        make(expr->lhs),
        make(expr->rhs));
}

void    RewriteImpl::visit( ExprEq*             expr)
{
    m_expr =  Factory<ExprEq>::create(
        expr->where(),
        make(expr->lhs),
        make(expr->rhs));
}

void    RewriteImpl::visit( ExprEqu*            expr)
{
    m_expr =  Factory<ExprEqu>::create(
        expr->where(),
        make(expr->lhs),
        make(expr->rhs));
}

void    RewriteImpl::visit( ExprF*              expr)
{
    m_expr =  Factory<ExprF>::create(
        expr->where(),
        make(expr->time),
        make(expr->arg));
}

void    RewriteImpl::visit( ExprG*              expr) 
{
    m_expr =  Factory<ExprG>::create(
        expr->where(),
        make(expr->time),
        make(expr->arg));
}

void    RewriteImpl::visit( ExprGe*             expr)
{
    m_expr =  Factory<ExprGe>::create(
        expr->where(),
        make(expr->lhs),
        make(expr->rhs));
}

void    RewriteImpl::visit( ExprGt*             expr) 
{
    m_expr =  Factory<ExprGt>::create(
        expr->where(),
        make(expr->lhs),
        make(expr->rhs));
}

void    RewriteImpl::visit( ExprH*              expr)
{
    m_expr =  Factory<ExprH>::create(
        expr->where(),
        make(expr->time),
        make(expr->arg));
}

void    RewriteImpl::visit( ExprImp*            expr) 
{
    m_expr =  Factory<ExprImp>::create(
        expr->where(),
        make(expr->lhs),
        make(expr->rhs));
}

void    RewriteImpl::visit( ExprIndx*           expr) 
{
    m_expr =  Factory<ExprIndx>::create(
        expr->where(),
        make(expr->lhs),
        make(expr->rhs));
}

void    RewriteImpl::visit( ExprInt*            expr)
{
    m_expr =  Factory<ExprInt>::create(
        expr->where(),
        make(expr->lhs),
        make(expr->rhs));
}

void    RewriteImpl::visit( ExprLe*             expr)
{
    m_expr =  Factory<ExprLe>::create(
        expr->where(),
        make(expr->lhs),
        make(expr->rhs));
}

void    RewriteImpl::visit( ExprLt*             expr) 
{
    m_expr =  Factory<ExprLt>::create(
        expr->where(),
        make(expr->lhs),
        make(expr->rhs));
}

void    RewriteImpl::visit( ExprMmbr*           expr) 
{
    m_expr =  Factory<ExprMmbr>::create(
        expr->where(),
        make(expr->arg),
        expr->mmbr);
}

void    RewriteImpl::visit( ExprMod*            expr)
{
    m_expr =  Factory<ExprMod>::create(
        expr->where(),
        make(expr->lhs),
        make(expr->rhs));
}

void    RewriteImpl::visit( ExprMul*            expr)
{
    m_expr =  Factory<ExprMul>::create(
        expr->where(),
        make(expr->lhs),
        make(expr->rhs));
}

void    RewriteImpl::visit( ExprNe*             expr)
{
    m_expr =  Factory<ExprNe>::create(
        expr->where(),
        make(expr->lhs),
        make(expr->rhs));
}

void    RewriteImpl::visit( ExprNeg*            expr) 
{
    m_expr =  Factory<ExprNeg>::create(
        expr->where(),
        make(expr->arg));
}

void    RewriteImpl::visit( ExprNot*            expr)
{
    m_expr =  Negated::make(
        make(expr->arg));
}

void    RewriteImpl::visit( ExprO*              expr) 
{
    m_expr =  Factory<ExprO>::create(
        expr->where(),
        make(expr->arg));
}

void    RewriteImpl::visit( ExprOr*             expr)
{
    m_expr =  Factory<ExprOr>::create(
        expr->where(),
        make(expr->lhs),
        make(expr->rhs));
}


void    RewriteImpl::visit( ExprParen*          expr) 
{
    auto    arg     = make(expr->arg);

    if(     dynamic_cast<ExprParen*>(arg) 
        ||  dynamic_cast<ExprNullary*>(arg)
        ||  dynamic_cast<ExprContext*>(arg)
        ||  dynamic_cast<ExprData*>(arg)
        ||  dynamic_cast<ExprAt*>(arg)
        ||  dynamic_cast<ExprMmbr*>(arg)
        ||  dynamic_cast<Temporal<ExprUnary>*>(arg)
        ||  dynamic_cast<Temporal<ExprBinary>*>(arg))
    {
        m_expr = make(arg);
    }
    else 
    {
        m_expr = Factory<ExprParen>::create(
            expr->where(),
            arg);
    }
}

void    RewriteImpl::visit( ExprRs*             expr)
{
    if(expr->time)
    {
        m_expr = make(!(Uw(expr->time, Not(expr->lhs), Not(expr->rhs))));
    }
    else 
    {
        m_expr =  Factory<ExprRs>::create(
            expr->where(),
            make(expr->time),
            make(expr->lhs),
            make(expr->rhs));
    }
}

void    RewriteImpl::visit( ExprRw*             expr)
{
    if(expr->time)
    {
        m_expr = make(!(Us(expr->time, Not(expr->lhs), Not(expr->rhs))));
    }
    else 
    {
        m_expr =  Factory<ExprRw>::create(
            expr->where(),make(expr->time),
            make(expr->lhs),
            make(expr->rhs));
    }
}

void    RewriteImpl::visit( ExprSs*             expr)
{
    if(expr->time)
    {
        auto    time        = make(expr->time);
        auto    lo          = Wrapper(time->lo);
        auto    hi          = Wrapper(time->hi);
        auto    prev_time   = Wrapper(Factory<ExprMmbr>::create(Factory<ExprContext>::create("starting"), "__time__"));
        auto    curr_time   = Wrapper(Factory<ExprMmbr>::create(Factory<ExprContext>::create("__curr__"), "__time__"));
        auto    cT_lt_hi    = (prev_time - curr_time) < hi;
        auto    lo_lt_Nt    = Yw(lo < (prev_time - curr_time));
        auto    lhs         = (Wrapper(make(expr->lhs)) && cT_lt_hi) || !lo_lt_Nt;
        auto    rhs         = (Wrapper(make(expr->rhs)) && cT_lt_hi) &&  lo_lt_Nt;

        m_expr  = At("starting", make(Ss(lhs, rhs).get()));
    }
    else
    {
        m_expr =  Factory<ExprSs>::create(
            expr->where(),
            make(expr->lhs),
            make(expr->rhs));
    }
}

void    RewriteImpl::visit( ExprSub*            expr)
{
    m_expr =  Factory<ExprSub>::create(
        expr->where(),
        make(expr->lhs),
        make(expr->rhs));
}


void    RewriteImpl::visit( ExprSw*             expr)
{
    if(expr->time)
    {
        auto    time        = make(expr->time);
        auto    lo          = Wrapper(time->lo);
        auto    hi          = Wrapper(time->hi);
        auto    prev_time   = Wrapper(Factory<ExprMmbr>::create(Factory<ExprContext>::create("starting"), "__time__"));
        auto    curr_time   = Wrapper(Factory<ExprMmbr>::create(Factory<ExprContext>::create("__curr__"), "__time__"));
        auto    cT_lt_hi    = (prev_time - curr_time) < hi;
        auto    lo_lt_Nt    = Yw(lo < (prev_time - curr_time));
        auto    lhs         = (Wrapper(make(expr->lhs)) && cT_lt_hi) || !lo_lt_Nt;
        auto    rhs         = (Wrapper(make(expr->rhs)) && cT_lt_hi) &&  lo_lt_Nt;

        m_expr  = At("starting", make(Sw(lhs, rhs).get()));
    }
    else
    {
        m_expr =  Factory<ExprSw>::create(
            expr->where(),
            make(expr->lhs),
            make(expr->rhs));
    }
}

void    RewriteImpl::visit( ExprTs*             expr)
{
    if(expr->time)
    {
        m_expr = make(!(Sw(expr->time, Not(expr->lhs), Not(expr->rhs))));
    }
    else 
    {
        m_expr =  Factory<ExprTs>::create(
            expr->where(),
            make(expr->time),
            make(expr->lhs),
            make(expr->rhs));
    }
}

void    RewriteImpl::visit( ExprTw*             expr)
{
    if(expr->time)
    {
        m_expr = make(!(Ss(expr->time, Not(expr->lhs), Not(expr->rhs))));
    }
    else 
    {
        m_expr =  Factory<ExprTw>::create(
            expr->where(),
            make(expr->time),
            make(expr->lhs),
            make(expr->rhs));
    }
}

void    RewriteImpl::visit( ExprUs*             expr) 
{
    if(expr->time)
    {
        auto    time        = make(expr->time);
        auto    lo          = Wrapper(time->lo);
        auto    hi          = Wrapper(time->hi);
        auto    prev_time   = Wrapper(Factory<ExprMmbr>::create(Factory<ExprContext>::create("starting"), "__time__"));
        auto    curr_time   = Wrapper(Factory<ExprMmbr>::create(Factory<ExprContext>::create("__curr__"), "__time__"));
        auto    cT_lt_hi    = (curr_time - prev_time) < hi;
        auto    lo_lt_Nt    = Xw(lo < (curr_time - prev_time));
        auto    lhs         = (Wrapper(make(expr->lhs)) && cT_lt_hi) || !lo_lt_Nt;
        auto    rhs         = (Wrapper(make(expr->rhs)) && cT_lt_hi) &&  lo_lt_Nt;

        m_expr  = At("starting", make(Us(lhs, rhs).get()));
    }
    else
    {
        m_expr =  Factory<ExprUs>::create(
            expr->where(),
            make(expr->lhs),
            make(expr->rhs));
    }
}

void    RewriteImpl::visit( ExprUw*             expr)
{
    if(expr->time)
    {
        auto    time        = make(expr->time);
        auto    lo          = Wrapper(time->lo);
        auto    hi          = Wrapper(time->hi);
        auto    prev_time   = Wrapper(Factory<ExprMmbr>::create(Factory<ExprContext>::create("starting"), "__time__"));
        auto    curr_time   = Wrapper(Factory<ExprMmbr>::create(Factory<ExprContext>::create("__curr__"), "__time__"));
        auto    cT_lt_hi    = (curr_time - prev_time) < hi;
        auto    lo_lt_Nt    = Xw(lo < (curr_time - prev_time));
        auto    lhs         = (Wrapper(make(expr->lhs)) && cT_lt_hi) || !lo_lt_Nt;
        auto    rhs         = (Wrapper(make(expr->rhs)) && cT_lt_hi) &&  lo_lt_Nt;

        m_expr  = At("starting", make(Uw(lhs, rhs).get()));
    }
    else
    {
        m_expr =  Factory<ExprUw>::create(
            expr->where(),
            make(expr->lhs),
            make(expr->rhs));
    }
}

void    RewriteImpl::visit( ExprXor*            expr) 
{
    m_expr =  Factory<ExprXor>::create(
        expr->where(),
        make(expr->lhs),
        make(expr->rhs));
}

void    RewriteImpl::visit( ExprXs*             expr) 
{
    m_expr =  Factory<ExprXs>::create(
        expr->where(),
        make(expr->time),
        make(expr->arg));
}

void    RewriteImpl::visit( ExprXw*             expr)
{
    m_expr =  Factory<ExprXw>::create(
        expr->where(),
        make(expr->time),
        make(expr->arg));
}

void    RewriteImpl::visit( ExprYs*             expr)
{
    m_expr =  Factory<ExprYs>::create(
        expr->where(),
        make(expr->time),
        make(expr->arg));
}

void    RewriteImpl::visit( ExprYw*             expr)
{
    m_expr =  Factory<ExprYw>::create(
        expr->where(),
        make(expr->time),
        make(expr->arg));
}

void    RewriteImpl::visit( Time*               expr)
{
    m_time = Factory<Time>::create(
        expr->where(),
        make(expr->lo),
        make(expr->hi));
}

void    RewriteImpl::visit(TimeMin*             expr)
{
    m_time = Factory<TimeMin>::create(
        expr->where(),
        make(expr->lo));
}

void    RewriteImpl::visit(TimeMax*             expr)
{
    m_time = Factory<TimeMax>::create(
        expr->where(),
        make(expr->hi));
}

Expr*   RewriteImpl::make(Expr* expr)
{
    expr->accept(*this);
    
    return  m_expr;
}

Time*   RewriteImpl::make(Time* time)
{
    if(time)
    {
        time->accept(*this);
        
        return  m_time;
    }
    return nullptr;
}

Expr*   Rewrite::make(Expr* expr)
{
    RewriteImpl impl;

    return  impl.make(expr);
}

Time*   Rewrite::make(Time* time)
{
    RewriteImpl impl;

    return  impl.make(time);
}
