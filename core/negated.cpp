#include "negated.hpp"
#include "canonic.hpp"
#include "factory.hpp"

struct Negated::Details
    : Visitor< Expression
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
    Expression* negated = nullptr;

    Expression* make(Expression* expr);

    void    visit(Expression*   expr) override;
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

Negated::Negated()
{
    details.reset(new Details);
}

Negated::~Negated()
{
}

Expression* Negated::Details::make(Expression* expr)
{
    negated = nullptr;
    expr->accept(*this);
    return negated;
}

void    Negated::Details::visit(Expression* expr) {negated = Factory<ExprNot>::create(expr);}

void    Negated::Details::visit(ExprTrue*   expr) {negated = Factory<ExprFalse>::create();}
void    Negated::Details::visit(ExprFalse*  expr) {negated = Factory<ExprTrue>::create();}

void    Negated::Details::visit(ExprEQ*     expr) {negated = Factory<ExprNE>::create(Canonic::make(expr->lhs), Canonic::make(expr->rhs));}
void    Negated::Details::visit(ExprNE*     expr) {negated = Factory<ExprEQ>::create(Canonic::make(expr->lhs), Canonic::make(expr->rhs));}

void    Negated::Details::visit(ExprGT*     expr) {negated = Factory<ExprLE>::create(Canonic::make(expr->lhs), Canonic::make(expr->rhs));}
void    Negated::Details::visit(ExprGE*     expr) {negated = Factory<ExprLT>::create(Canonic::make(expr->lhs), Canonic::make(expr->rhs));}

void    Negated::Details::visit(ExprLT*     expr) {negated = Factory<ExprGE>::create(Canonic::make(expr->lhs), Canonic::make(expr->rhs));}
void    Negated::Details::visit(ExprLE*     expr) {negated = Factory<ExprGT>::create(Canonic::make(expr->lhs), Canonic::make(expr->rhs));}

void    Negated::Details::visit(ExprNot*    expr) {negated = Canonic::make(expr->arg);}

void    Negated::Details::visit(ExprOr*     expr) {negated = Factory<ExprAnd>::create(make(expr->lhs), make(expr->rhs));}
void    Negated::Details::visit(ExprAnd*    expr) {negated = Factory<ExprOr>::create(make(expr->lhs), make(expr->rhs));}

void    Negated::Details::visit(ExprXor*    expr) {negated = Factory<ExprEqu>::create(Canonic::make(expr->lhs), Canonic::make(expr->rhs));}
void    Negated::Details::visit(ExprEqu*    expr) {negated = Factory<ExprXor>::create(Canonic::make(expr->lhs), Canonic::make(expr->rhs));}

void    Negated::Details::visit(ExprImp*    expr) {negated = Factory<ExprAnd>::create(Canonic::make(expr->lhs), make(expr->rhs));}

void    Negated::Details::visit(ExprG*      expr) {negated = Factory<ExprF>::create(make(expr->arg));}
void    Negated::Details::visit(ExprF*      expr) {negated = Factory<ExprG>::create(make(expr->arg));}
void    Negated::Details::visit(ExprXs*     expr) {negated = Factory<ExprXw>::create(make(expr->arg));}
void    Negated::Details::visit(ExprXw*     expr) {negated = Factory<ExprXs>::create(make(expr->arg));}
void    Negated::Details::visit(ExprUs*     expr) {negated = Factory<ExprRw>::create(make(expr->lhs), make(expr->rhs));}
void    Negated::Details::visit(ExprUw*     expr) {negated = Factory<ExprRs>::create(make(expr->lhs), make(expr->rhs));}
void    Negated::Details::visit(ExprRs*     expr) {negated = Factory<ExprUw>::create(make(expr->lhs), make(expr->rhs));}
void    Negated::Details::visit(ExprRw*     expr) {negated = Factory<ExprUs>::create(make(expr->lhs), make(expr->rhs));}

void    Negated::Details::visit(ExprH*      expr) {negated = Factory<ExprO>::create(make(expr->arg));}
void    Negated::Details::visit(ExprO*      expr) {negated = Factory<ExprH>::create(make(expr->arg));}
void    Negated::Details::visit(ExprYs*     expr) {negated = Factory<ExprYw>::create(make(expr->arg));}
void    Negated::Details::visit(ExprYw*     expr) {negated = Factory<ExprYs>::create(make(expr->arg));}
void    Negated::Details::visit(ExprSs*     expr) {negated = Factory<ExprTw>::create(make(expr->lhs), make(expr->rhs));}
void    Negated::Details::visit(ExprSw*     expr) {negated = Factory<ExprTs>::create(make(expr->lhs), make(expr->rhs));}
void    Negated::Details::visit(ExprTs*     expr) {negated = Factory<ExprSw>::create(make(expr->lhs), make(expr->rhs));}
void    Negated::Details::visit(ExprTw*     expr) {negated = Factory<ExprSs>::create(make(expr->lhs), make(expr->rhs));}

Expression* Negated::make(Expression* expr)
{
    Negated self;

    return  self.details->make(expr);
}
