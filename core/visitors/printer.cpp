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

#include "printer.hpp"
#include "../factory.hpp"

std::string multichar(int data)
{
    std::ostringstream os;

    data    = ((data & 0xffff0000) >> 16)
            | ((data & 0x0000ffff) << 16);
    data    = ((data & 0xff00ff00) >>  8)
            | ((data & 0x00ff00ff) <<  8);            

    while(data) 
    {
        os << (char)(data & 0xff);
        data >>= 8;
    }

    return os.str();
}

struct PrinterImpl
    : Visitor< Expr
             , ExprBinary
             , ExprParen
             , Temporal<ExprBinary>
             , ExprUnary
             , Temporal<ExprUnary>
             , ExprContext
             , ExprData
             , ExprMmbr
             , ExprConstBoolean
             , ExprConstInteger
             , ExprConstNumber
             , ExprAt
             , TimeInterval
             , TimeUpperBound
             , TimeLowerBound>
{
    PrinterImpl(std::ostream& os)
        : os(os)
    {
    }
    
    void    output(Expr* expr);

    void    visit(Expr*                 expr) override;
    void    visit(ExprBinary*           expr) override;
    void    visit(Temporal<ExprBinary>* expr) override;
    void    visit(ExprParen*            expr) override;
    void    visit(ExprUnary*            expr) override;
    void    visit(Temporal<ExprUnary>*  expr) override;
    void    visit(ExprContext*          expr) override;
    void    visit(ExprData*             expr) override;
    void    visit(ExprMmbr*             expr) override;
    void    visit(ExprConstInteger*     expr) override;
    void    visit(ExprConstNumber*      expr) override;
    void    visit(ExprConstBoolean*     expr) override;
    void    visit(ExprAt*               expr) override;
    void    visit(TimeInterval*         expr) override;
    void    visit(TimeUpperBound*       expr) override;
    void    visit(TimeLowerBound*       expr) override;

    std::ostream&   os;
};

void    PrinterImpl::visit( Expr*           expr)   
{
    os << "???";
}

void    PrinterImpl::visit( ExprParen*              expr)
{
    os << "(";
    expr->arg->accept(*this);
    os << ")";
}


void    PrinterImpl::visit( ExprUnary*              expr)
{
    os << multichar(expr->op) << " ";
    expr->arg->accept(*this);
}


void    PrinterImpl::visit( ExprBinary*             expr)
{
    expr->lhs->accept(*this);
    os << " " << multichar(expr->op) << " ";
    expr->rhs->accept(*this);
}

void    PrinterImpl::visit( Temporal<ExprUnary>*    expr)
{
    os << multichar(expr->op);
    if(expr->time)
        expr->time->accept(*this);
    os << "(";
    expr->arg->accept(*this);
    os << ")";
}

void    PrinterImpl::visit( Temporal<ExprBinary>*   expr)
{
    os << multichar(expr->op);
    if(expr->time)
        expr->time->accept(*this);
    os << "(";
    expr->lhs->accept(*this);
    os << ", ";
    expr->rhs->accept(*this);
    os << ")";
}

void    PrinterImpl::visit( ExprContext*   expr)
{
    os << expr->name;
}

void    PrinterImpl::visit( ExprData*       expr)
{
    os << expr->name;
}

void    PrinterImpl::visit(ExprMmbr*        expr)
{
    expr->arg->accept(*this);
    os << "." << expr->mmbr;
}

void    PrinterImpl::visit(ExprConstInteger*    expr)
{
    os << expr->value;
}

void    PrinterImpl::visit(ExprConstNumber*     expr)
{
    os << expr->value; 
}

void    PrinterImpl::visit(ExprConstBoolean*    expr)
{
    os << (expr->value ? "true" : "false"); 
}

void    PrinterImpl::visit(TimeInterval*         expr)
{
    os << "[";
    expr->lo->accept(*this);
    os << ":";
    expr->hi->accept(*this);
    os << "]";
}

void    PrinterImpl::visit(TimeUpperBound*  expr)
{
    os << "[";
    os << ":";
    expr->hi->accept(*this);
    os << "]";
}

void    PrinterImpl::visit(TimeLowerBound*  expr)
{
    os << "[";
    expr->lo->accept(*this);
    os << ":";
    os << "]";
}

void    PrinterImpl::visit(ExprAt*          expr)
{
    os << expr->name;
    expr->arg->accept(*this);
}

void    PrinterImpl::output(Expr* expr)
{
    expr->accept(*this);
}

std::ostream&   Printer::output(std::ostream& os, Expr* expr)
{
    PrinterImpl impl(os);

    impl.output(expr);

    return os << std::endl;
}
