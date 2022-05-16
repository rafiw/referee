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
             , ExprTrue
             , ExprFalse
             , ExprBinary
             , Temporal<ExprBinary>
             , ExprUnary
             , Temporal<ExprUnary>
             , ExprData>
{
    PrinterImpl(std::ostream& os)
        : os(os)
    {
    }
    
    void    output(Expr* expr);

    void    visit(Expr*                 expr) override;
    void    visit(ExprTrue*             expr) override;
    void    visit(ExprFalse*            expr) override;
    void    visit(ExprBinary*           expr) override;
    void    visit(Temporal<ExprBinary>* expr) override;
    void    visit(ExprUnary*            expr) override;
    void    visit(Temporal<ExprUnary>*  expr) override;
    void    visit(ExprData*             expr) override;

    std::ostream&   os;
};

void    PrinterImpl::visit( Expr*           expr)   
{
    os << "???";
}

void    PrinterImpl::visit( ExprTrue*               expr)
{
    os << "true";
}

void    PrinterImpl::visit( ExprFalse*              expr)
{
    os << "false";
}

void    PrinterImpl::visit( ExprUnary*              expr)
{
    std::cout << multichar(expr->op) << " ";
    expr->arg->accept(*this);}

void    PrinterImpl::visit( ExprBinary*             expr)
{
    expr->lhs->accept(*this);
    std::cout << " " << multichar(expr->op) << " ";
    expr->rhs->accept(*this);
}

void    PrinterImpl::visit( Temporal<ExprUnary>*    expr)
{
    std::cout << multichar(expr->op) << "(";
    expr->arg->accept(*this);
    os << ")";
}

void    PrinterImpl::visit( Temporal<ExprBinary>*   expr)
{
    std::cout << multichar(expr->op) << "(";
    expr->lhs->accept(*this);
    os << ", ";
    expr->rhs->accept(*this);
    os << ")";
}

void    PrinterImpl::visit( ExprData*               expr)
{
    os << expr->data;
}



void    PrinterImpl::output(Expr* expr)
{
    expr->accept(*this);
}


std::ostream&   Printer::output(std::ostream& os, Expr* expr)
{
    PrinterImpl impl(os);

    impl.output(expr);

    return os;
}
