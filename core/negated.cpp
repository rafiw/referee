#include "negated.hpp"
#include "canonic.hpp"
#include "factory.hpp"

struct NegatedImpl
    : Visitor< Expr
             , ExprTrue
             , ExprFalse
             , ExprEQ
             , ExprNE
             , ExprGT
             , ExprGE
             , ExprLT
             , ExprLE
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
    Expr* negated = nullptr;

    Expr* make(Expr* expr);

    void    visit(Expr*   expr) override;
    void    visit(ExprTrue*     expr) override;
    void    visit(ExprFalse*    expr) override;
    void    visit(ExprEQ*       expr) override;
    void    visit(ExprNE*       expr) override;
    void    visit(ExprGT*       expr) override;
    void    visit(ExprGE*       expr) override;
    void    visit(ExprLT*       expr) override;
    void    visit(ExprLE*       expr) override;
    void    visit(ExprNot*      expr) override;
    void    visit(ExprOr*       expr) override;
    void    visit(ExprAnd*      expr) override;
    void    visit(ExprXor*      expr) override;
    void    visit(ExprImp*      expr) override;
    void    visit(ExprEqu*      expr) override;
    void    visit(ExprG*        expr) override;
    void    visit(ExprF*        expr) override;
    void    visit(ExprXs*       expr) override;
    void    visit(ExprXw*       expr) override;
    void    visit(ExprUs*       expr) override;
    void    visit(ExprUw*       expr) override;
    void    visit(ExprRs*       expr) override;
    void    visit(ExprRw*       expr) override;
    void    visit(ExprH*        expr) override;
    void    visit(ExprO*        expr) override;
    void    visit(ExprYs*       expr) override;
    void    visit(ExprYw*       expr) override;
    void    visit(ExprSs*       expr) override;
    void    visit(ExprSw*       expr) override;
    void    visit(ExprTs*       expr) override;
    void    visit(ExprTw*       expr) override;
};

void    NegatedImpl::visit(Expr* expr) {negated = Factory<ExprNot>::create(expr);}

void    NegatedImpl::visit(ExprTrue*   expr) {negated = Factory<ExprFalse>::create();}
void    NegatedImpl::visit(ExprFalse*  expr) {negated = Factory<ExprTrue>::create();}

void    NegatedImpl::visit(ExprEQ*     expr) {negated = Factory<ExprNE>::create(Canonic::make(expr->lhs), Canonic::make(expr->rhs));}
void    NegatedImpl::visit(ExprNE*     expr) {negated = Factory<ExprEQ>::create(Canonic::make(expr->lhs), Canonic::make(expr->rhs));}

void    NegatedImpl::visit(ExprGT*     expr) {negated = Factory<ExprLE>::create(Canonic::make(expr->lhs), Canonic::make(expr->rhs));}
void    NegatedImpl::visit(ExprGE*     expr) {negated = Factory<ExprLT>::create(Canonic::make(expr->lhs), Canonic::make(expr->rhs));}

void    NegatedImpl::visit(ExprLT*     expr) {negated = Factory<ExprGE>::create(Canonic::make(expr->lhs), Canonic::make(expr->rhs));}
void    NegatedImpl::visit(ExprLE*     expr) {negated = Factory<ExprGT>::create(Canonic::make(expr->lhs), Canonic::make(expr->rhs));}

void    NegatedImpl::visit(ExprNot*    expr) {negated = Canonic::make(expr->arg);}

void    NegatedImpl::visit(ExprOr*     expr) {negated = Factory<ExprAnd>::create(make(expr->lhs), make(expr->rhs));}
void    NegatedImpl::visit(ExprAnd*    expr) {negated = Factory<ExprOr>::create(make(expr->lhs), make(expr->rhs));}

void    NegatedImpl::visit(ExprXor*    expr) {negated = Factory<ExprEqu>::create(Canonic::make(expr->lhs), Canonic::make(expr->rhs));}
void    NegatedImpl::visit(ExprEqu*    expr) {negated = Factory<ExprXor>::create(Canonic::make(expr->lhs), Canonic::make(expr->rhs));}

void    NegatedImpl::visit(ExprImp*    expr) {negated = Factory<ExprAnd>::create(Canonic::make(expr->lhs), make(expr->rhs));}

void    NegatedImpl::visit(ExprG*      expr) {negated = Factory<ExprF>::create(make(expr->arg));}
void    NegatedImpl::visit(ExprF*      expr) {negated = Factory<ExprG>::create(make(expr->arg));}
void    NegatedImpl::visit(ExprXs*     expr) {negated = Factory<ExprXw>::create(make(expr->arg));}
void    NegatedImpl::visit(ExprXw*     expr) {negated = Factory<ExprXs>::create(make(expr->arg));}
void    NegatedImpl::visit(ExprUs*     expr) {negated = Factory<ExprRw>::create(make(expr->lhs), make(expr->rhs));}
void    NegatedImpl::visit(ExprUw*     expr) {negated = Factory<ExprRs>::create(make(expr->lhs), make(expr->rhs));}
void    NegatedImpl::visit(ExprRs*     expr) {negated = Factory<ExprUw>::create(make(expr->lhs), make(expr->rhs));}
void    NegatedImpl::visit(ExprRw*     expr) {negated = Factory<ExprUs>::create(make(expr->lhs), make(expr->rhs));}

void    NegatedImpl::visit(ExprH*      expr) {negated = Factory<ExprO>::create(make(expr->arg));}
void    NegatedImpl::visit(ExprO*      expr) {negated = Factory<ExprH>::create(make(expr->arg));}
void    NegatedImpl::visit(ExprYs*     expr) {negated = Factory<ExprYw>::create(make(expr->arg));}
void    NegatedImpl::visit(ExprYw*     expr) {negated = Factory<ExprYs>::create(make(expr->arg));}
void    NegatedImpl::visit(ExprSs*     expr) {negated = Factory<ExprTw>::create(make(expr->lhs), make(expr->rhs));}
void    NegatedImpl::visit(ExprSw*     expr) {negated = Factory<ExprTs>::create(make(expr->lhs), make(expr->rhs));}
void    NegatedImpl::visit(ExprTs*     expr) {negated = Factory<ExprSw>::create(make(expr->lhs), make(expr->rhs));}
void    NegatedImpl::visit(ExprTw*     expr) {negated = Factory<ExprSs>::create(make(expr->lhs), make(expr->rhs));}

Expr* NegatedImpl::make(Expr* expr)
{
    negated = nullptr;
    expr->accept(*this);
    return negated;
}


Expr* Negated::make(Expr* expr)
{
    NegatedImpl impl;

    return  impl.make(expr);
}
