#include "canonic.hpp"
#include "negated.hpp"

struct Canonic::Details
{
    Expression* expr = nullptr;
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
    return details->expr;
}

void    Canonic::visit(Expression*  expr) {/*nothing*/}
void    Canonic::visit(ExprNot*     expr) {details->expr = negated(expr);}
void    Canonic::visit(ExprOr*      expr) {details->expr = new ExprOr(make(expr->lhs), make(expr->rhs));}
void    Canonic::visit(ExprAnd*     expr) {details->expr = new ExprAnd(make(expr->lhs), make(expr->rhs));}
void    Canonic::visit(ExprXor*     expr) {details->expr = new ExprXor(make(expr->lhs), make(expr->rhs));}
void    Canonic::visit(ExprImp*     expr) {details->expr = new ExprOr(make(negated(expr->lhs)), make(expr->rhs));}
void    Canonic::visit(ExprEqu*     expr) {}
void    Canonic::visit(ExprG*       expr) {details->expr = new ExprRw(new ExprFalse, make(negated(expr->arg)));}
void    Canonic::visit(ExprF*       expr) {details->expr = new ExprUs(new ExprTrue,  make(expr->arg));}
void    Canonic::visit(ExprXs*      expr) {details->expr = new ExprXs(make(expr->arg));}
void    Canonic::visit(ExprXw*      expr) {details->expr = new ExprXw(make(expr->arg));}
void    Canonic::visit(ExprUs*      expr) {details->expr = new ExprUs(make(expr->lhs), make(expr->rhs));}
void    Canonic::visit(ExprUw*      expr) {details->expr = new ExprUw(make(expr->lhs), make(expr->rhs));}
void    Canonic::visit(ExprRs*      expr) {details->expr = new ExprRs(make(expr->lhs), make(expr->rhs));}
void    Canonic::visit(ExprRw*      expr) {details->expr = new ExprRw(make(expr->lhs), make(expr->rhs));}
void    Canonic::visit(ExprH*       expr) {details->expr = new ExprTw(new ExprFalse, make(negated(expr->arg)));}
void    Canonic::visit(ExprO*       expr) {details->expr = new ExprSs(new ExprTrue,  make(expr->arg));}
void    Canonic::visit(ExprYs*      expr) {details->expr = new ExprYs(make(expr->arg));}
void    Canonic::visit(ExprYw*      expr) {details->expr = new ExprYw(make(expr->arg));}
void    Canonic::visit(ExprSs*      expr) {details->expr = new ExprSs(make(expr->lhs), make(expr->rhs));}
void    Canonic::visit(ExprSw*      expr) {details->expr = new ExprSw(make(expr->lhs), make(expr->rhs));}
void    Canonic::visit(ExprTs*      expr) {details->expr = new ExprTs(make(expr->lhs), make(expr->rhs));}
void    Canonic::visit(ExprTw*      expr) {details->expr = new ExprTw(make(expr->lhs), make(expr->rhs));}

Expression* canonic(Expression* expr)
{
    static Canonic  self;

    return  self.make(expr);
}


