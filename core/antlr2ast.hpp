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

#include "visitors/typecalc.hpp"
#include "visitors/printer.hpp"     //  TODO: remove
#include "module.hpp"
#include "utils.hpp"

class Antlr2AST
    : public referee::refereeBaseVisitor
{
private:
    template<typename Type, typename Ctxt>
    std::any    acceptTernary(Ctxt* ctxt) {
        auto lhs = ctxt->expression()[0]->accept(this);
        auto mid = ctxt->expression()[1]->accept(this);
        auto rhs = ctxt->expression()[2]->accept(this);
        
        return  static_cast<Expr*>(Factory<Type>::create(
            position(ctxt),
            std::any_cast<Expr*>(lhs), 
            std::any_cast<Expr*>(mid), 
            std::any_cast<Expr*>(rhs)));
    }

    template<typename Type, typename Ctxt>
    std::any    acceptBinary(Ctxt* ctxt) {
        auto lhs = ctxt->expression()[0]->accept(this);
        auto rhs = ctxt->expression()[1]->accept(this);
        
        return  static_cast<Expr*>(Factory<Type>::create(
            position(ctxt),
            std::any_cast<Expr*>(lhs), 
            std::any_cast<Expr*>(rhs)));
    }

    template<typename Type, typename Ctxt>
    std::any    acceptUnary(Ctxt* ctxt) {
        auto arg = ctxt->expression()->accept(this);
        
        return  static_cast<Expr*>(Factory<Type>::create(
            position(ctxt),
            std::any_cast<Expr*>(arg)));
    }

    template<typename Type, typename Ctxt>
    std::any    acceptTemporalBinary(Ctxt* ctxt) {
        auto lhs = ctxt->expression()[0]->accept(this);
        auto rhs = ctxt->expression()[1]->accept(this);

        if(ctxt->time())
        {
            auto time= ctxt->time()->accept(this);

            return  static_cast<Expr*>(Factory<Type>::create(
                position(ctxt), 
                std::any_cast<TimeInterval*>(time),
                std::any_cast<Expr*>(lhs), 
                std::any_cast<Expr*>(rhs)));
        }
        else
        {
            return  static_cast<Expr*>(Factory<Type>::create(
                position(ctxt), 
                std::any_cast<Expr*>(lhs), 
                std::any_cast<Expr*>(rhs)));
        }
    }

    template<typename Type, typename Ctxt>
    std::any    acceptTemporalUnary(Ctxt* ctxt) {
        auto arg = ctxt->expression()->accept(this);

        if(ctxt->time())
        {
            auto time= ctxt->time()->accept(this);

            return  static_cast<Expr*>(Factory<Type>::create(
                position(ctxt), 
                std::any_cast<TimeInterval*>(time),
                std::any_cast<Expr*>(arg)));
        }
        else
        {
            return  static_cast<Expr*>(Factory<Type>::create(
                position(ctxt), 
                std::any_cast<Expr*>(arg)));
        }
    }

    Position    position(antlr4::ParserRuleContext* rule)
    {
        auto    start   = rule->start;
        auto    stop    = rule->stop;

        return  Position(
            Location(
                start->getLine(),  
                start->getCharPositionInLine()),
            Location(
                stop->getLine(),   
                stop->getCharPositionInLine() + stop->getText().length())
        );
    }

    Module* module  = nullptr;

public:
    Antlr2AST()
        : module(Factory<Module>::create("main"))   // TODO: ?
    {
    }
    
    //  time
    std::any visitTimeFull( referee::refereeParser::TimeFullContext*    ctx) override
    { 
        auto lo     = ctx->expression(0)->accept(this);
        auto hi     = ctx->expression(1)->accept(this);
        return static_cast<TimeInterval*>(Factory<TimeInterval>::create(
            std::any_cast<Expr*>(lo), 
            std::any_cast<Expr*>(hi)));
    }

    std::any visitTimeUpper(referee::refereeParser::TimeUpperContext*   ctx) override
    {
        auto hi     = ctx->expression()->accept(this);
        return static_cast<TimeInterval*>(Factory<TimeUpperBound>::create(
            std::any_cast<Expr*>(hi)));
    }

    std::any visitTimeLower(referee::refereeParser::TimeLowerContext*   ctx) override
    {
        auto lo     = ctx->expression()->accept(this);
        return static_cast<TimeInterval*>(Factory<TimeLowerBound>::create(
            std::any_cast<Expr*>(lo)));
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
    std::any visitExprF( referee::refereeParser::ExprFContext*     ctx) override { return acceptTemporalUnary<ExprF>(ctx); }
    std::any visitExprG( referee::refereeParser::ExprGContext*     ctx) override { return acceptTemporalUnary<ExprG>(ctx); }
    std::any visitExprXs(referee::refereeParser::ExprXsContext*    ctx) override { return acceptTemporalUnary<ExprXs>(ctx); }
    std::any visitExprXw(referee::refereeParser::ExprXwContext*    ctx) override { return acceptTemporalUnary<ExprXw>(ctx); }

    std::any visitExprUs(referee::refereeParser::ExprUsContext*    ctx) override { return acceptTemporalBinary<ExprUs>(ctx); }
    std::any visitExprUw(referee::refereeParser::ExprUwContext*    ctx) override { return acceptTemporalBinary<ExprUw>(ctx); }
    std::any visitExprRs(referee::refereeParser::ExprRsContext*    ctx) override { return acceptTemporalBinary<ExprRs>(ctx); }
    std::any visitExprRw(referee::refereeParser::ExprRwContext*    ctx) override { return acceptTemporalBinary<ExprRw>(ctx); }

    //  temporal past
    std::any visitExprO( referee::refereeParser::ExprOContext*     ctx) override { return acceptTemporalUnary<ExprO>(ctx); }
    std::any visitExprH( referee::refereeParser::ExprHContext*     ctx) override { return acceptTemporalUnary<ExprH>(ctx); }
    std::any visitExprYs(referee::refereeParser::ExprYsContext*    ctx) override { return acceptTemporalUnary<ExprYs>(ctx); }
    std::any visitExprYw(referee::refereeParser::ExprYwContext*    ctx) override { return acceptTemporalUnary<ExprYw>(ctx); }

    std::any visitExprSs(referee::refereeParser::ExprSsContext*    ctx) override { return acceptTemporalBinary<ExprSs>(ctx); }
    std::any visitExprSw(referee::refereeParser::ExprSwContext*    ctx) override { return acceptTemporalBinary<ExprSw>(ctx); }
    std::any visitExprTs(referee::refereeParser::ExprTsContext*    ctx) override { return acceptTemporalBinary<ExprTs>(ctx); }
    std::any visitExprTw(referee::refereeParser::ExprTwContext*    ctx) override { return acceptTemporalBinary<ExprTw>(ctx); }

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

        return static_cast<Type*>(new TypeStruct(members)); //  TODO: use factory
    } 
    
    std::any visitTypeAlias(    referee::refereeParser::TypeAliasContext*   ctx) override 
    {
        auto name   = ctx->typeID()->getText();

        return module->get_type(name);
    }

    std::any visitExprIndx( referee::refereeParser::ExprIndxContext*    ctx) override { return acceptBinary<ExprIndx>(ctx); }

    std::any visitExprData( referee::refereeParser::ExprDataContext*    ctx) override
    {
        auto    name    = ctx->dataID()->getText();
        auto    type    = module->get_data(name);
        auto    expr    = static_cast<Expr*>(Factory<ExprData>::create(std::string(ctx->dataID()->getText())));

        expr->type(type);

        return expr;
    }

    std::any visitExprConst(referee::refereeParser::ExprConstContext*   ctx) override
    {
        if(ctx->integer() != nullptr)
            return  static_cast<Expr*>(Factory<ExprConstInteger>::create(parse_integer(ctx->integer()->getText())));
        
        if(ctx->floating() != nullptr)
            return  static_cast<Expr*>(Factory<ExprConstNumber>::create(parse_number(ctx->floating()->getText())));

        if(ctx->boolean() != nullptr)
            return  static_cast<Expr*>(Factory<ExprConstBoolean>::create(parse_boolean(ctx->boolean()->getText())));

        if(ctx->string() != nullptr)
            return  static_cast<Expr*>(Factory<ExprConstString>::create(parse_string(ctx->string()->getText())));

        throw std::runtime_error(__PRETTY_FUNCTION__);
        /*
    SignContext *sign();
    FloatingContext *floating();
    StringContext *string();
    BooleanContext *boolean();
    */
        return nullptr;
    }


    std::any visitExprMmbr( referee::refereeParser::ExprMmbrContext*    ctx) override
    {
        auto base   = ctx->expression()->accept(this);
       
        return static_cast<Expr*>(Factory<ExprMmbr>::create(position(ctx), std::any_cast<Expr*>(base), std::string(ctx->mmbrID()->getText())));
    }

    std::any visitStatement(referee::refereeParser::StatementContext*   ctx) override
    {
        if (ctx->expression())
        {
            auto    expr    = ctx->expression()->accept(this);

            TypeCalc::make(std::any_cast<Expr*>(expr));
            Printer::output(std::cout, std::any_cast<Expr*>(expr));
        }

        if (ctx->declaraion())
        {
            ctx->declaraion()->accept(this);
        }

        return nullptr;
    }

    std::any visitDeclType(referee::refereeParser::DeclTypeContext* ctx) override
    {
        auto    name    = ctx->typeID()->getText();
        auto    type    = ctx->type()->accept(this);
        
        module->add_type(name, std::any_cast<Type*>(type));

        return nullptr;
    }

    std::any visitDeclData(referee::refereeParser::DeclDataContext* ctx) override
    {
        auto    name    = ctx->dataID()->getText();
        auto    type    = ctx->type()->accept(this);
        
        module->add_data(name, std::any_cast<Type*>(type));

        return nullptr;
    }

};
