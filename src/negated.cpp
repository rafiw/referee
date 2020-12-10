#include "negated.hpp"
#include "canonic.hpp"

struct Negated::Details
{
    Expression* expr = nullptr;
};

Negated::Negated()
{
    details.reset(new Details);
}

Negated::~Negated()
{
}

Expression* Negated::make(Expression* expr)
{
    details->expr = canonic(expr);
    expr->accept(*this);
    return details->expr;
}

void    Negated::visit(Expression*  expr) {/*nothing*/}

void    Negated::visit(ExprTrue*    expr) {details->expr = new ExprFalse();}
void    Negated::visit(ExprFalse*   expr) {details->expr = new ExprTrue();}

void    Negated::visit(ExprEQ*      expr) {details->expr = new ExprNE(canonic(expr->lhs), canonic(expr->rhs));}
void    Negated::visit(ExprNE*      expr) {details->expr = new ExprEQ(canonic(expr->lhs), canonic(expr->rhs));}

void    Negated::visit(ExprGT*      expr) {details->expr = new ExprLE(canonic(expr->lhs), canonic(expr->rhs));}
void    Negated::visit(ExprGE*      expr) {details->expr = new ExprLT(canonic(expr->lhs), canonic(expr->rhs));}

void    Negated::visit(ExprLT*      expr) {details->expr = new ExprGE(canonic(expr->lhs), canonic(expr->rhs));}
void    Negated::visit(ExprLE*      expr) {details->expr = new ExprGT(canonic(expr->lhs), canonic(expr->rhs));}

void    Negated::visit(ExprNot*     expr) {details->expr = canonic(expr->arg);}

void    Negated::visit(ExprOr*      expr) {details->expr = new ExprAnd(make(expr->lhs), make(expr->rhs));}
void    Negated::visit(ExprAnd*     expr) {details->expr = new ExprOr(make(expr->lhs), make(expr->rhs));}

void    Negated::visit(ExprXor*     expr) {details->expr = new ExprEqu(canonic(expr->lhs), canonic(expr->rhs));}
void    Negated::visit(ExprEqu*     expr) {details->expr = new ExprXor(canonic(expr->lhs), canonic(expr->rhs));}

void    Negated::visit(ExprImp*     expr) {details->expr = new ExprAnd(canonic(expr->lhs), make(expr->rhs));}

void    Negated::visit(ExprG*       expr) {details->expr = new ExprF(make(expr->arg));}
void    Negated::visit(ExprF*       expr) {details->expr = new ExprG(make(expr->arg));}
void    Negated::visit(ExprXs*      expr) {details->expr = new ExprXw(make(expr->arg));}
void    Negated::visit(ExprXw*      expr) {details->expr = new ExprXs(make(expr->arg));}
void    Negated::visit(ExprUs*      expr) {details->expr = new ExprRw(make(expr->lhs), make(expr->rhs));}
void    Negated::visit(ExprUw*      expr) {details->expr = new ExprRs(make(expr->lhs), make(expr->rhs));}
void    Negated::visit(ExprRs*      expr) {details->expr = new ExprUw(make(expr->lhs), make(expr->rhs));}
void    Negated::visit(ExprRw*      expr) {details->expr = new ExprUs(make(expr->lhs), make(expr->rhs));}

void    Negated::visit(ExprH*       expr) {details->expr = new ExprO(make(expr->arg));}
void    Negated::visit(ExprO*       expr) {details->expr = new ExprH(make(expr->arg));}
void    Negated::visit(ExprYs*      expr) {details->expr = new ExprYw(make(expr->arg));}
void    Negated::visit(ExprYw*      expr) {details->expr = new ExprYs(make(expr->arg));}
void    Negated::visit(ExprSs*      expr) {details->expr = new ExprTw(make(expr->lhs), make(expr->rhs));}
void    Negated::visit(ExprSw*      expr) {details->expr = new ExprTs(make(expr->lhs), make(expr->rhs));}
void    Negated::visit(ExprTs*      expr) {details->expr = new ExprSw(make(expr->lhs), make(expr->rhs));}
void    Negated::visit(ExprTw*      expr) {details->expr = new ExprSs(make(expr->lhs), make(expr->rhs));}

Expression* negated(Expression* expr)
{
    static Negated  self;

    return  self.make(expr);
}
