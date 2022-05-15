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

#pragma once

#include "antlr4-runtime/antlr4-runtime.h"
#include "refereeParser.h"
#include "refereeLexer.h"
#include "refereeBaseVisitor.h"

class Antlr2AST
    : public referee::refereeBaseVisitor
{
private:
    template<typename Type, typename Ctxt>
    std::any    acceptTernary(Ctxt* ctxt) {
        auto lhs = ctxt->expression()[0]->accept(this);
        auto mid = ctxt->expression()[1]->accept(this);
        auto rhs = ctxt->expression()[2]->accept(this);
        
        return static_cast<Expr*>(Factory<Type>::create(std::any_cast<Expr*>(lhs), std::any_cast<Expr*>(mid), std::any_cast<Expr*>(rhs)));
    }

    template<typename Type, typename Ctxt>
    std::any    acceptBinary(Ctxt* ctxt) {
        auto lhs = ctxt->expression()[0]->accept(this);
        auto rhs = ctxt->expression()[1]->accept(this);
        
        return static_cast<Expr*>(Factory<Type>::create(std::any_cast<Expr*>(lhs), std::any_cast<Expr*>(rhs)));
    }

    template<typename Type, typename Ctxt>
    std::any    acceptUnary(Ctxt* ctxt) {
        auto arg = ctxt->expression()->accept(this);
        
        return static_cast<Expr*>(Factory<Type>::create(std::any_cast<Expr*>(arg)));
    }

public:
    std::any visitExprConst(referee::refereeParser::ExprConstContext*   ctx) override   { return static_cast<Expr*>(Factory<ExprTrue>::create()); }

    //  time
    std::any visitTimeFull( referee::refereeParser::TimeFullContext*    ctx) override
    { 
        auto lo     = ctx->expression(0)->accept(this);
        auto hi     = ctx->expression(1)->accept(this);
        return static_cast<TimeInterval*>(Factory<TimeInterval>::create(std::any_cast<Expr*>(lo), std::any_cast<Expr*>(hi)));
    }

    std::any visitTimeUpper(referee::refereeParser::TimeUpperContext*   ctx) override
    {
        auto hi     = ctx->expression()->accept(this);
        return static_cast<TimeInterval*>(Factory<TimeUpperBound>::create(std::any_cast<Expr*>(hi)));
    }

    std::any visitTimeLower(referee::refereeParser::TimeLowerContext*   ctx) override
    {
        auto lo     = ctx->expression()->accept(this);
        return static_cast<TimeInterval*>(Factory<TimeLowerBound>::create(std::any_cast<Expr*>(lo)));
    }

    //  comparison
    std::any visitExprLt( referee::refereeParser::ExprLtContext*   ctx) override { return acceptBinary<ExprLt>(ctx); }
    std::any visitExprLe( referee::refereeParser::ExprLeContext*   ctx) override { return acceptBinary<ExprLe>(ctx); }
    std::any visitExprGt( referee::refereeParser::ExprGtContext*   ctx) override { return acceptBinary<ExprGt>(ctx); }
    std::any visitExprGe( referee::refereeParser::ExprGeContext*   ctx) override { return acceptBinary<ExprGe>(ctx); }
    std::any visitExprEq( referee::refereeParser::ExprEqContext*   ctx) override { return acceptBinary<ExprEq>(ctx); }
    std::any visitExprNe( referee::refereeParser::ExprNeContext*   ctx) override { return acceptBinary<ExprNe>(ctx); }

    //  arithmetic
    std::any visitExprAdd( referee::refereeParser::ExprAddContext* ctx) override { return acceptBinary<ExprAdd>(ctx); }
    std::any visitExprSub( referee::refereeParser::ExprSubContext* ctx) override { return acceptBinary<ExprSub>(ctx); }
    std::any visitExprMul( referee::refereeParser::ExprMulContext* ctx) override { return acceptBinary<ExprMul>(ctx); }
    std::any visitExprDiv( referee::refereeParser::ExprDivContext* ctx) override { return acceptBinary<ExprDiv>(ctx); }

    //  ternary
    std::any visitExprTer( referee::refereeParser::ExprTerContext* ctx) override { return acceptTernary<ExprChoice>(ctx);}

    //  propisitional
    std::any visitExprOr( referee::refereeParser::ExprOrContext*   ctx) override { return acceptBinary<ExprOr>(ctx); }
    std::any visitExprAnd(referee::refereeParser::ExprAndContext*  ctx) override { return acceptBinary<ExprAnd>(ctx); }
    std::any visitExprXor(referee::refereeParser::ExprXorContext*  ctx) override { return acceptBinary<ExprXor>(ctx); }
    std::any visitExprImp(referee::refereeParser::ExprImpContext*  ctx) override { return acceptBinary<ExprImp>(ctx); }
    std::any visitExprEqu(referee::refereeParser::ExprEquContext*  ctx) override { return acceptBinary<ExprEqu>(ctx); }

    //  temporal future
    std::any visitExprF( referee::refereeParser::ExprFContext*     ctx) override { return acceptUnary<ExprF>(ctx); }
    std::any visitExprG( referee::refereeParser::ExprGContext*     ctx) override { return acceptUnary<ExprG>(ctx); }
    std::any visitExprXs(referee::refereeParser::ExprXsContext*    ctx) override { return acceptUnary<ExprXs>(ctx); }
    std::any visitExprXw(referee::refereeParser::ExprXwContext*    ctx) override { return acceptUnary<ExprXw>(ctx); }

    std::any visitExprUs(referee::refereeParser::ExprUsContext*    ctx) override { return acceptBinary<ExprUs>(ctx); }
    std::any visitExprUw(referee::refereeParser::ExprUwContext*    ctx) override { return acceptBinary<ExprUw>(ctx); }
    std::any visitExprRs(referee::refereeParser::ExprRsContext*    ctx) override { return acceptBinary<ExprRs>(ctx); }
    std::any visitExprRw(referee::refereeParser::ExprRwContext*    ctx) override { return acceptBinary<ExprRw>(ctx); }

    //  temporal past
    std::any visitExprO( referee::refereeParser::ExprOContext*     ctx) override { return acceptUnary<ExprO>(ctx); }
    std::any visitExprH( referee::refereeParser::ExprHContext*     ctx) override { return acceptUnary<ExprH>(ctx); }
    std::any visitExprYs(referee::refereeParser::ExprYsContext*    ctx) override { return acceptUnary<ExprYs>(ctx); }
    std::any visitExprYw(referee::refereeParser::ExprYwContext*    ctx) override { return acceptUnary<ExprYw>(ctx); }

    std::any visitExprSs(referee::refereeParser::ExprSsContext*    ctx) override { return acceptBinary<ExprSs>(ctx); }
    std::any visitExprSw(referee::refereeParser::ExprSwContext*    ctx) override { return acceptBinary<ExprSw>(ctx); }
    std::any visitExprTs(referee::refereeParser::ExprTsContext*    ctx) override { return acceptBinary<ExprTs>(ctx); }
    std::any visitExprTw(referee::refereeParser::ExprTwContext*    ctx) override { return acceptBinary<ExprTw>(ctx); }

    std::map<std::string, Type*>    m_name2type = {
        {"boolean", Factory<TypeBoolean>::create()},
        {"integer", Factory<TypeInteger>::create()},
        {"string",  Factory<TypeString>::create()},
        {"number",  Factory<TypeNumber>::create()}
    };

    std::any visitTypeBool(     referee::refereeParser::TypeBoolContext*    ctx) override { return static_cast<Type*>(Factory<TypeBoolean>::create()); }
    std::any visitTypeInteger(  referee::refereeParser::TypeIntegerContext* ctx) override { return static_cast<Type*>(Factory<TypeInteger>::create()); }
    std::any visitTypeNumber(   referee::refereeParser::TypeNumberContext*  ctx) override { return static_cast<Type*>(Factory<TypeNumber>::create()); }
    std::any visitTypeString(   referee::refereeParser::TypeStringContext*  ctx) override { return static_cast<Type*>(Factory<TypeString>::create()); }
    std::any visitTypeStruct(   referee::refereeParser::TypeStructContext*  ctx) override
    {
        std::vector<Named<Type>> members;

        auto    names   = ctx->mmbrList()->ID();
        auto    types   = ctx->mmbrList()->type();
        auto    size    = names.size();

        for(auto i = 0; i < size; i++)
        {
            auto    name    = names[i]->getText();
            auto    type    = types[i]->accept(this);

            members.push_back(Named<Type>(name, std::any_cast<Type*>(type)));
        }

        return static_cast<Type*>(new TypeStruct(members));
    } 
    
    std::any visitTypeAlias(    referee::refereeParser::TypeAliasContext*   ctx) override 
    {
        auto name   = ctx->typeID()->getText();

        return m_name2type[name];
    }

    std::any visitDeclType(     referee::refereeParser::DeclTypeContext*    ctx) override
    {
        auto name   = ctx->typeID()->getText();
        auto type   = ctx->type()->accept(this);

        m_name2type[name]   = std::any_cast<Type*>(type);

        return m_name2type[name];
    }

    std::any visitExprIndx( referee::refereeParser::ExprIndxContext*    ctx) override { return acceptBinary<ExprIndx>(ctx); }

    std::any visitExprData( referee::refereeParser::ExprDataContext*    ctx) override
    {
        return static_cast<Expr*>(Factory<ExprData>::create(std::string(ctx->dataID()->getText())));
    }

    std::any visitExprMmbr( referee::refereeParser::ExprMmbrContext*    ctx) override
    {
        auto base   = ctx->expression()->accept(this);
       
        return static_cast<Expr*>(Factory<ExprMmbr>::create(std::any_cast<Expr*>(base), std::string(ctx->mmbrID()->getText())));
    }
};
