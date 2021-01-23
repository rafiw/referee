#include "canonic.hpp"
#include "negated.hpp"
#include "factory.hpp"

struct Canonic::Details
    : Visitor< Expression
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
    Expression* canonic = nullptr;
    Expression* t = Factory<ExprTrue>::create();
    Expression* f = Factory<ExprFalse>::create();

    Expression* make(Expression* expr);

    void        visit(Expression*   expr) override;
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

Canonic::Canonic()
{
    details = std::make_unique<Details>();
}

Canonic::~Canonic()
{
}

Expression* Canonic::make(Expression* expr)
{
    Canonic inst;

    return inst.details->make(expr);
}

Expression* Canonic::Details::make(Expression* expr)
{
    canonic = expr;

    expr->accept(*this);

    return  canonic;
}

void    Canonic::Details::visit(Expression*  expr) {canonic = expr;}
void    Canonic::Details::visit(ExprNot*     expr) {canonic = Negated::make(expr->arg);}
void    Canonic::Details::visit(ExprOr*      expr) {canonic = Factory<ExprOr>::create(make(expr->lhs), make(expr->rhs));}
void    Canonic::Details::visit(ExprAnd*     expr) {canonic = Factory<ExprAnd>::create(make(expr->lhs), make(expr->rhs));}
void    Canonic::Details::visit(ExprXor*     expr) {canonic = Factory<ExprXor>::create(make(expr->lhs), make(expr->rhs));}
void    Canonic::Details::visit(ExprImp*     expr) {canonic = Factory<ExprOr>::create(make(Negated::make(expr->lhs)), make(expr->rhs));}
void    Canonic::Details::visit(ExprEqu*     expr) {}
void    Canonic::Details::visit(ExprG*       expr) {canonic = Factory<ExprRw>::create(f, make(expr->arg));}
void    Canonic::Details::visit(ExprF*       expr) {canonic = Factory<ExprUs>::create(t, make(expr->arg));}
void    Canonic::Details::visit(ExprXs*      expr) {canonic = Factory<ExprXs>::create(make(expr->arg));}
void    Canonic::Details::visit(ExprXw*      expr) {canonic = Factory<ExprXw>::create(make(expr->arg));}
void    Canonic::Details::visit(ExprUs*      expr) {canonic = Factory<ExprUs>::create(make(expr->lhs), make(expr->rhs));}
void    Canonic::Details::visit(ExprUw*      expr) {canonic = Factory<ExprUw>::create(make(expr->lhs), make(expr->rhs));}
void    Canonic::Details::visit(ExprRs*      expr) {canonic = Factory<ExprRs>::create(make(expr->lhs), make(expr->rhs));}
void    Canonic::Details::visit(ExprRw*      expr) {canonic = Factory<ExprRw>::create(make(expr->lhs), make(expr->rhs));}
void    Canonic::Details::visit(ExprH*       expr) {canonic = Factory<ExprTw>::create(f, make(expr->arg));}
void    Canonic::Details::visit(ExprO*       expr) {canonic = Factory<ExprSs>::create(t, make(expr->arg));}
void    Canonic::Details::visit(ExprYs*      expr) {canonic = Factory<ExprYs>::create(make(expr->arg));}
void    Canonic::Details::visit(ExprYw*      expr) {canonic = Factory<ExprYw>::create(make(expr->arg));}
void    Canonic::Details::visit(ExprSs*      expr) {canonic = Factory<ExprSs>::create(make(expr->lhs), make(expr->rhs));}
void    Canonic::Details::visit(ExprSw*      expr) {canonic = Factory<ExprSw>::create(make(expr->lhs), make(expr->rhs));}
void    Canonic::Details::visit(ExprTs*      expr) {canonic = Factory<ExprTs>::create(make(expr->lhs), make(expr->rhs));}
void    Canonic::Details::visit(ExprTw*      expr) {canonic = Factory<ExprTw>::create(make(expr->lhs), make(expr->rhs));}
