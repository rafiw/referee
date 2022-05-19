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

#include "antlr2ast.hpp"

#include "visitors/typecalc.hpp"
#include "visitors/printer.hpp"     //  TODO: remove
#include "module.hpp"
#include "utils.hpp"
#include "factory.hpp"

Antlr2AST::Antlr2AST()
    : module(Factory<Module>::create("main"))   // TODO: ?
{
}

template<typename Type, typename Ctxt>
std::any    Antlr2AST::acceptTernary(Ctxt* ctxt) {
    auto lhs = ctxt->expression()[0]->accept(this);
    auto mid = ctxt->expression()[1]->accept(this);
    auto rhs = ctxt->expression()[2]->accept(this);

    return  static_cast<Expr*>(build<Type>(ctxt,
        std::any_cast<Expr*>(lhs),
        std::any_cast<Expr*>(mid),
        std::any_cast<Expr*>(rhs)));
}

template<typename Type, typename Ctxt>
std::any    Antlr2AST::acceptBinary(Ctxt* ctxt) {
    auto lhs = ctxt->expression()[0]->accept(this);
    auto rhs = ctxt->expression()[1]->accept(this);

    return  static_cast<Expr*>(build<Type>(ctxt,
        std::any_cast<Expr*>(lhs),
        std::any_cast<Expr*>(rhs)));
}

template<typename Type, typename Ctxt>
std::any    Antlr2AST::acceptUnary(Ctxt* ctxt) {
    auto arg = ctxt->expression()->accept(this);

    return  static_cast<Expr*>(build<Type>(ctxt,
        std::any_cast<Expr*>(arg)));
}

template<typename Type, typename Ctxt>
std::any    Antlr2AST::acceptTemporalBinary(Ctxt* ctxt) {
    auto lhs = ctxt->expression()[0]->accept(this);
    auto rhs = ctxt->expression()[1]->accept(this);

    if(ctxt->time())
    {
        auto time= ctxt->time()->accept(this);

        return  static_cast<Expr*>(build<Type>(ctxt,
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
std::any    Antlr2AST::acceptTemporalUnary(Ctxt* ctxt) {
    auto arg = ctxt->expression()->accept(this);

    if(ctxt->time())
    {
        auto time= ctxt->time()->accept(this);

        return  static_cast<Expr*>(build<Type>(ctxt,
            std::any_cast<TimeInterval*>(time),
            std::any_cast<Expr*>(arg)));
    }
    else
    {
        return  static_cast<Expr*>(build<Type>(ctxt,
            std::any_cast<Expr*>(arg)));
    }
}

template<typename Type, typename ... Args>
Type*       Antlr2AST::build(antlr4::ParserRuleContext* rule, Args ... args)
{
    return  Factory<Type>::create(position(rule), args...);
}

Position    Antlr2AST::position(antlr4::ParserRuleContext* rule)
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

std::any Antlr2AST::visitDeclData(      referee::refereeParser::DeclDataContext*    ctx)
{
    auto    name    = ctx->dataID()->getText();
    auto    type    = ctx->type()->accept(this);

    module->add_data(name, std::any_cast<Type*>(type));

    return nullptr;
}

std::any Antlr2AST::visitDeclType(      referee::refereeParser::DeclTypeContext*    ctx)
{
    auto    name    = ctx->typeID()->getText();
    auto    type    = ctx->type()->accept(this);

    module->add_type(name, std::any_cast<Type*>(type));

    return nullptr;
}

std::any Antlr2AST::visitExprAdd(       referee::refereeParser::ExprAddContext*     ctx)
{
    return acceptBinary<ExprAdd>(ctx);
}

std::any Antlr2AST::visitExprAnd(       referee::refereeParser::ExprAndContext*     ctx)
{
    return acceptBinary<ExprAnd>(ctx);
}

std::any Antlr2AST::visitExprAt(        referee::refereeParser::ExprAtContext*      ctx)
{
    auto    name    = ctx->ID()->getText();
    
    module->push_context(name);
    auto    expr    = ctx->expression()->accept(this);
    module->pop_context();

    return static_cast<Expr*>(build<ExprAt>(ctx, name, std::any_cast<Expr*>(expr)));
}

std::any Antlr2AST::visitExprConst(     referee::refereeParser::ExprConstContext*   ctx)
{
    if(ctx->integer() != nullptr)
        return  static_cast<Expr*>(build<ExprConstInteger>(ctx, parse_integer(ctx->integer()->getText())));

    if(ctx->floating() != nullptr)
        return  static_cast<Expr*>(build<ExprConstNumber>(ctx, parse_number(ctx->floating()->getText())));

    if(ctx->boolean() != nullptr)
        return  static_cast<Expr*>(build<ExprConstBoolean>(ctx, parse_boolean(ctx->boolean()->getText())));

    if(ctx->string() != nullptr)
        return  static_cast<Expr*>(build<ExprConstString>(ctx, parse_string(ctx->string()->getText())));

    throw std::runtime_error(__PRETTY_FUNCTION__);
    /*
SignContext *sign();
FloatingContext *floating();
StringContext *string();
BooleanContext *boolean();
*/
    return nullptr;
}

std::any Antlr2AST::visitExprData(      referee::refereeParser::ExprDataContext*    ctx)
{
    auto    name    = ctx->dataID()->getText();

    if(module->has_context(name))
    {
        auto    expr    = static_cast<Expr*>(build<ExprContext>(ctx, name));
        auto    type    = build<TypeContext>(ctx, module);

        expr->type(type);

        return  expr;
    }
    else
    {
        auto    type    = module->get_data(name);
        auto    expr    = static_cast<Expr*>(build<ExprData>(ctx, name));

        expr->type(type);

        return expr;
    }
}

std::any Antlr2AST::visitExprDiv(       referee::refereeParser::ExprDivContext*     ctx)
{
    return acceptBinary<ExprDiv>(ctx);
}

std::any Antlr2AST::visitExprEq(        referee::refereeParser::ExprEqContext*      ctx)
{
    return acceptBinary<ExprEq>(ctx);
}

std::any Antlr2AST::visitExprEqu(       referee::refereeParser::ExprEquContext*     ctx)
{
    return acceptBinary<ExprEqu>(ctx);
}

std::any Antlr2AST::visitExprF(         referee::refereeParser::ExprFContext*       ctx)
{
    return acceptTemporalUnary<ExprF>(ctx);
}

std::any Antlr2AST::visitExprG(         referee::refereeParser::ExprGContext*       ctx)
{
    return acceptTemporalUnary<ExprG>(ctx);
}

std::any Antlr2AST::visitExprGe(        referee::refereeParser::ExprGeContext*      ctx)
{
    return acceptBinary<ExprGe>(ctx);
}

std::any Antlr2AST::visitExprGt(        referee::refereeParser::ExprGtContext*      ctx)
{
    return acceptBinary<ExprGt>(ctx);
}

std::any Antlr2AST::visitExprH(         referee::refereeParser::ExprHContext*       ctx)
{
    return acceptTemporalUnary<ExprH>(ctx);
}

std::any Antlr2AST::visitExprImp(       referee::refereeParser::ExprImpContext*     ctx)
{
    return acceptBinary<ExprImp>(ctx);
}

std::any Antlr2AST::visitExprIndx(      referee::refereeParser::ExprIndxContext*    ctx)
{
    return acceptBinary<ExprIndx>(ctx);
}

std::any Antlr2AST::visitExprInt(       referee::refereeParser::ExprIntContext*     ctx)
{
    std::any    lhs;
    std::any    rhs;

    if(ctx->expression().size() == 1)
    {
        lhs = Factory<ExprConstBoolean>::create(true);
        rhs = ctx->expression()[0]->accept(this);
    }
    else
    {
        lhs = ctx->expression()[0]->accept(this);
        rhs = ctx->expression()[1]->accept(this);
    }

    if(ctx->time())
    {
        auto time= ctx->time()->accept(this);

        return  static_cast<Expr*>(build<ExprInt>(ctx,
            std::any_cast<TimeInterval*>(time),
            std::any_cast<Expr*>(lhs),
            std::any_cast<Expr*>(rhs)));
    }
    else
    {
        return  static_cast<Expr*>(build<ExprInt>(ctx,
            std::any_cast<Expr*>(lhs),
            std::any_cast<Expr*>(rhs)));
    }
}

std::any Antlr2AST::visitExprLe(        referee::refereeParser::ExprLeContext*      ctx)
{
    return acceptBinary<ExprLe>(ctx);
}

std::any Antlr2AST::visitExprLt(        referee::refereeParser::ExprLtContext*      ctx)
{
    return acceptBinary<ExprLt>(ctx);
}

std::any Antlr2AST::visitExprMmbr(      referee::refereeParser::ExprMmbrContext*    ctx)
{
    auto name   = ctx->mmbrID()->getText();
    auto base   = ctx->expression()->accept(this);

    return static_cast<Expr*>(build<ExprMmbr>(ctx, std::any_cast<Expr*>(base), name));
}

std::any Antlr2AST::visitExprMod(       referee::refereeParser::ExprModContext*     ctx)
{
    return acceptBinary<ExprMod>(ctx);
}

std::any Antlr2AST::visitExprMul(       referee::refereeParser::ExprMulContext*     ctx)
{
    return acceptBinary<ExprMul>(ctx);
}

std::any Antlr2AST::visitExprNe(        referee::refereeParser::ExprNeContext*      ctx)
{
    return acceptBinary<ExprNe>(ctx);
}

std::any Antlr2AST::visitExprO(         referee::refereeParser::ExprOContext*       ctx)
{
    return acceptTemporalUnary<ExprO>(ctx);
}

std::any Antlr2AST::visitExprOr(        referee::refereeParser::ExprOrContext*      ctx)
{
    return acceptBinary<ExprOr>(ctx);
}

std::any Antlr2AST::visitExprParen(     referee::refereeParser::ExprParenContext*   ctx)
{
    return acceptUnary<ExprParen>(ctx);
}


std::any Antlr2AST::visitExprRs(        referee::refereeParser::ExprRsContext*      ctx)
{
    return acceptTemporalBinary<ExprRs>(ctx);
}

std::any Antlr2AST::visitExprRw(        referee::refereeParser::ExprRwContext*      ctx)
{
    return acceptTemporalBinary<ExprRw>(ctx);
}

std::any Antlr2AST::visitExprSs(        referee::refereeParser::ExprSsContext*      ctx)
{
    return acceptTemporalBinary<ExprSs>(ctx);
}

std::any Antlr2AST::visitExprSub(       referee::refereeParser::ExprSubContext*     ctx)
{
    return acceptBinary<ExprSub>(ctx);
}

std::any Antlr2AST::visitExprSw(        referee::refereeParser::ExprSwContext*      ctx)
{
    return acceptTemporalBinary<ExprSw>(ctx);
}

std::any Antlr2AST::visitExprTer(       referee::refereeParser::ExprTerContext*     ctx)
{
    return acceptTernary<ExprChoice>(ctx);
}

std::any Antlr2AST::visitExprTs(        referee::refereeParser::ExprTsContext*      ctx)
{
    return acceptTemporalBinary<ExprTs>(ctx);
}

std::any Antlr2AST::visitExprTw(        referee::refereeParser::ExprTwContext*      ctx)
{
    return acceptTemporalBinary<ExprTw>(ctx);
}

std::any Antlr2AST::visitExprUs(        referee::refereeParser::ExprUsContext*      ctx)
{
    return acceptTemporalBinary<ExprUs>(ctx);
}

std::any Antlr2AST::visitExprUw(        referee::refereeParser::ExprUwContext*      ctx)
{
    return acceptTemporalBinary<ExprUw>(ctx);
}

std::any Antlr2AST::visitExprXor(       referee::refereeParser::ExprXorContext*     ctx)
{
    return acceptBinary<ExprXor>(ctx);
}

std::any Antlr2AST::visitExprXs(        referee::refereeParser::ExprXsContext*      ctx)
{
    return acceptTemporalUnary<ExprXs>(ctx);
}

std::any Antlr2AST::visitExprXw(        referee::refereeParser::ExprXwContext*      ctx)
{
    return acceptTemporalUnary<ExprXw>(ctx);
}

std::any Antlr2AST::visitExprYs(        referee::refereeParser::ExprYsContext*      ctx)
{
    return acceptTemporalUnary<ExprYs>(ctx);
}

std::any Antlr2AST::visitExprYw(        referee::refereeParser::ExprYwContext*      ctx)
{
    return acceptTemporalUnary<ExprYw>(ctx);
}

std::any Antlr2AST::visitStatement(     referee::refereeParser::StatementContext*   ctx)
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

std::any Antlr2AST::visitTimeFull(      referee::refereeParser::TimeFullContext*    ctx)
{
    auto lo     = ctx->expression(0)->accept(this);
    auto hi     = ctx->expression(1)->accept(this);
    return static_cast<TimeInterval*>(build<TimeInterval>(ctx, std::any_cast<Expr*>(lo), std::any_cast<Expr*>(hi)));
}

std::any Antlr2AST::visitTimeLower(     referee::refereeParser::TimeLowerContext*   ctx)
{
    auto lo     = ctx->expression()->accept(this);
    return static_cast<TimeInterval*>(build<TimeLowerBound>(ctx, std::any_cast<Expr*>(lo)));
}

std::any Antlr2AST::visitTimeUpper(     referee::refereeParser::TimeUpperContext*   ctx)
{
    auto hi     = ctx->expression()->accept(this);
    return static_cast<TimeInterval*>(build<TimeUpperBound>(ctx, std::any_cast<Expr*>(hi)));
}

std::any Antlr2AST::visitTypeAlias(     referee::refereeParser::TypeAliasContext*   ctx)
{
    auto name   = ctx->typeID()->getText();

    return module->get_type(name);
}

std::any Antlr2AST::visitTypeBool(      referee::refereeParser::TypeBoolContext*    ctx)
{
    return static_cast<Type*>(build<TypeBoolean>(ctx));
}

std::any Antlr2AST::visitTypeEnum(      referee::refereeParser::TypeEnumContext*    ctx)
{
    std::vector<std::string>    items;

    for(auto it: ctx->itemList()->ID())
    {
        items.push_back(it->getText());
    }

    return static_cast<Type*>(build<TypeEnum>(ctx, items));
}

std::any Antlr2AST::visitTypeInteger(   referee::refereeParser::TypeIntegerContext* ctx)
{
    return static_cast<Type*>(build<TypeInteger>(ctx));
}

std::any Antlr2AST::visitTypeNumber(    referee::refereeParser::TypeNumberContext*  ctx)
{
    return static_cast<Type*>(build<TypeNumber>(ctx));
}

std::any Antlr2AST::visitTypeString(    referee::refereeParser::TypeStringContext*  ctx)
{
    return static_cast<Type*>(build<TypeString>(ctx));
}

std::any Antlr2AST::visitTypeStruct(    referee::refereeParser::TypeStructContext*  ctx)
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

