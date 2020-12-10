#pragma once
#include "visitor.hpp"
#include "syntax.hpp"

class Negated
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
public:
    Negated();
    virtual ~Negated();
    
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
    
private:
    struct Details;
    std::unique_ptr<Details> details;
};

Expression* negated(Expression* expr);
