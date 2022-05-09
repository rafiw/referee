#include "canonic.hpp"
#include "negated.hpp"
#include "factory.hpp"

struct CanonicImpl
    : Visitor< Expr
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
    Expr* t = Factory<ExprTrue>::create();
    Expr* f = Factory<ExprFalse>::create();

    Expr* make(Expr* expr);

    void        visit(Expr*   expr) override;
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
void    CanonicImpl::visit(ExprNot*     expr) {canonic = Negated::make(expr->arg);}
void    CanonicImpl::visit(ExprOr*      expr) {canonic = Factory<ExprOr>::create(make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprAnd*     expr) {canonic = Factory<ExprAnd>::create(make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprXor*     expr) {canonic = Factory<ExprXor>::create(make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprImp*     expr) {canonic = Factory<ExprOr>::create(make(Negated::make(expr->lhs)), make(expr->rhs));}
void    CanonicImpl::visit(ExprEqu*     expr) {}
void    CanonicImpl::visit(ExprG*       expr) {canonic = Factory<ExprRw>::create(f, make(expr->arg));}
void    CanonicImpl::visit(ExprF*       expr) {canonic = Factory<ExprUs>::create(t, make(expr->arg));}
void    CanonicImpl::visit(ExprXs*      expr) {canonic = Factory<ExprXs>::create(make(expr->arg));}
void    CanonicImpl::visit(ExprXw*      expr) {canonic = Factory<ExprXw>::create(make(expr->arg));}
void    CanonicImpl::visit(ExprUs*      expr) {canonic = Factory<ExprUs>::create(make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprUw*      expr) {canonic = Factory<ExprUw>::create(make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprRs*      expr) {canonic = Factory<ExprRs>::create(make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprRw*      expr) {canonic = Factory<ExprRw>::create(make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprH*       expr) {canonic = Factory<ExprTw>::create(f, make(expr->arg));}
void    CanonicImpl::visit(ExprO*       expr) {canonic = Factory<ExprSs>::create(t, make(expr->arg));}
void    CanonicImpl::visit(ExprYs*      expr) {canonic = Factory<ExprYs>::create(make(expr->arg));}
void    CanonicImpl::visit(ExprYw*      expr) {canonic = Factory<ExprYw>::create(make(expr->arg));}
void    CanonicImpl::visit(ExprSs*      expr) {canonic = Factory<ExprSs>::create(make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprSw*      expr) {canonic = Factory<ExprSw>::create(make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprTs*      expr) {canonic = Factory<ExprTs>::create(make(expr->lhs), make(expr->rhs));}
void    CanonicImpl::visit(ExprTw*      expr) {canonic = Factory<ExprTw>::create(make(expr->lhs), make(expr->rhs));}

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

