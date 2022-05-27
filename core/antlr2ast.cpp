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
#include "visitors/rewrite.hpp"     //  TODO: remove
#include "visitors/canonic.hpp"     //  TODO: remove
#include "colormod.hpp"             //  TODO: remove
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
            std::any_cast<Time*>(time),
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
            std::any_cast<Time*>(time),
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
    //  TODO: add sign
    
    if(ctx->integer() != nullptr)
        return  ctx->integer()->accept(this);

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
        auto    base    = static_cast<Expr*>(build<ExprContext>(ctx, "__curr__"));
        auto    type    = module->get_data(name);
        auto    expr    = static_cast<Expr*>(build<ExprMmbr>(ctx, base, name));

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
            std::any_cast<Time*>(time),
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

std::any Antlr2AST::visitInteger(       referee::refereeParser::IntegerContext*     ctx)
{
    if(ctx->BININT())
        return  static_cast<Expr*>(build<ExprConstInteger>(ctx, parse_binint(ctx->BININT()->getText())));
    if(ctx->OCTINT())
        return  static_cast<Expr*>(build<ExprConstInteger>(ctx, parse_octint(ctx->OCTINT()->getText())));
    if(ctx->INTEGER())
        return  static_cast<Expr*>(build<ExprConstInteger>(ctx, parse_decint(ctx->INTEGER()->getText())));
    if(ctx->HEXINT())
        return  static_cast<Expr*>(build<ExprConstInteger>(ctx, parse_hexint(ctx->HEXINT()->getText())));

    throw std::runtime_error(__PRETTY_FUNCTION__);
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

std::any Antlr2AST::visitProgram(       referee::refereeParser::ProgramContext*     ctx)
{
    visitChildren(ctx);
    return module;
}

std::any Antlr2AST::visitStatement(     referee::refereeParser::StatementContext*   ctx)
{
    if (ctx->expression())
    {
        auto    expr    = std::any_cast<Expr*>(ctx->expression()->accept(this));

        TypeCalc::make(expr);
        std::cout << Color::Modifier(Color::FG_GREEN);
        Printer::output(std::cout, expr);
        std::cout << Color::Modifier(Color::FG_DEFAULT);

        auto    temp    = Rewrite::make(Canonic::make(expr));
        Printer::output(std::cout, temp);
    }

    if (ctx->declaraion())
    {
        ctx->declaraion()->accept(this);
    }

    if(ctx->specPattern())
    {
        auto    spec    = std::any_cast<Spec*>(ctx->specPattern()->accept(this));
        
        std::cout << Color::Modifier(Color::FG_GREEN);
        Printer::output(std::cout, spec);
        std::cout << Color::Modifier(Color::FG_DEFAULT);

        auto    temp    = Rewrite::make(spec);
        Printer::output(std::cout, temp);
    }

    return nullptr;
}

std::any Antlr2AST::visitTimeFull(      referee::refereeParser::TimeFullContext*    ctx)
{
    auto lo     = ctx->expression(0)->accept(this);
    auto hi     = ctx->expression(1)->accept(this);
    return static_cast<Time*>(build<Time>(ctx, std::any_cast<Expr*>(lo), std::any_cast<Expr*>(hi)));
}

std::any Antlr2AST::visitTimeLower(     referee::refereeParser::TimeLowerContext*   ctx)
{
    auto lo     = ctx->expression()->accept(this);
    return static_cast<Time*>(build<TimeMin>(ctx, std::any_cast<Expr*>(lo)));
}

std::any Antlr2AST::visitTimeUpper(     referee::refereeParser::TimeUpperContext*   ctx)
{
    auto hi     = ctx->expression()->accept(this);
    return static_cast<Time*>(build<TimeMax>(ctx, std::any_cast<Expr*>(hi)));
}

std::any Antlr2AST::visitTypeAlias(     referee::refereeParser::TypeAliasContext*   ctx)
{
    auto name   = ctx->typeID()->getText();

    return module->get_type(name);
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

std::any Antlr2AST::visitUnits(                 referee::refereeParser::UnitsContext*                   ctx)
{
    uint64_t    nsec    = 0;

    if(ctx->nanoseconds())  nsec=           1ull;
    if(ctx->microseconds()) nsec=        1000ull;
    if(ctx->milliseconds()) nsec=     1000000ull;
    if(ctx->seconds())      nsec=  1000000000ull;
    if(ctx->minutes())      nsec= 60000000000ull;

    return static_cast<Expr*>(Factory<ExprConstInteger>::create(nsec));
}

std::any Antlr2AST::visitSpecPattern(           referee::refereeParser::SpecPatternContext*             ctx)
{
    return  std::any_cast<Spec*>(ctx->psbody()->accept(this));
}

std::any Antlr2AST::visitSpecUniversality(      referee::refereeParser::SpecUniversalityContext*        ctx)
{
    auto    P   = std::any_cast<Expr*>(ctx->exprP()->accept(this));
    auto    t   = std::any_cast<Time*>(ctx->timeBound()->accept(this));

    return  static_cast<Spec*>(Factory<SpecUniversality>::create(P, t));
}

std::any Antlr2AST::visitSpecAbsence(           referee::refereeParser::SpecAbsenceContext*             ctx)
{
    auto    P   = std::any_cast<Expr*>(ctx->exprP()->accept(this));
    auto    t   = std::any_cast<Time*>(ctx->timeBound()->accept(this));

    return  static_cast<Spec*>(Factory<SpecAbsence>::create(P, t));
}

std::any Antlr2AST::visitSpecExistence(         referee::refereeParser::SpecExistenceContext*           ctx)
{
    auto    P   = std::any_cast<Expr*>(ctx->exprP()->accept(this));
    auto    t   = std::any_cast<Time*>(ctx->timeBound()->accept(this));

    return  static_cast<Spec*>(Factory<SpecExistence>::create(P, t));
}

std::any Antlr2AST::visitSpecTransientState(    referee::refereeParser::SpecTransientStateContext*      ctx)
{
    auto    P   = std::any_cast<Expr*>(ctx->exprP()->accept(this));
    auto    n   = std::any_cast<Expr*>(ctx->exprN()->accept(this));
    auto    u   = std::any_cast<Expr*>(ctx->units()->accept(this));
    auto    t   = Factory<TimeMin>::create(Factory<ExprMul>::create(n, u));

    return  static_cast<Spec*>(Factory<SpecTransientState>::create(P, t));
}

std::any Antlr2AST::visitSpecSteadyState(       referee::refereeParser::SpecSteadyStateContext*         ctx)
{
    auto    P   = std::any_cast<Expr*>(ctx->exprP()->accept(this));

    return  static_cast<Spec*>(Factory<SpecSteadyState>::create(P));
}

std::any Antlr2AST::visitSpecMinimunDuration(   referee::refereeParser::SpecMinimunDurationContext*     ctx)
{
    auto    P   = std::any_cast<Expr*>(ctx->exprP()->accept(this));
    auto    n   = std::any_cast<Expr*>(ctx->exprN()->accept(this));
    auto    u   = std::any_cast<Expr*>(ctx->units()->accept(this));
    auto    t   = Factory<TimeMin>::create(Factory<ExprMul>::create(n, u));

    return  static_cast<Spec*>(Factory<SpecMinimunDuration>::create(P, t));
}

std::any Antlr2AST::visitSpecMaximumDuration(   referee::refereeParser::SpecMaximumDurationContext*     ctx)
{
    auto    P   = std::any_cast<Expr*>(ctx->exprP()->accept(this));
    auto    n   = std::any_cast<Expr*>(ctx->exprN()->accept(this));
    auto    u   = std::any_cast<Expr*>(ctx->units()->accept(this));
    auto    t   = Factory<TimeMax>::create(Factory<ExprMul>::create(n, u));

    return  static_cast<Spec*>(Factory<SpecMaximumDuration>::create(P, t));
}

std::any Antlr2AST::visitSpecRecurrence(        referee::refereeParser::SpecRecurrenceContext*          ctx)
{
    auto    P   = std::any_cast<Expr*>(ctx->exprP()->accept(this));
    auto    n   = std::any_cast<Expr*>(ctx->exprN()->accept(this));
    auto    u   = std::any_cast<Expr*>(ctx->units()->accept(this));
    auto    t   = Factory<TimeMax>::create(Factory<ExprMul>::create(n, u));

    return  static_cast<Spec*>(Factory<SpecRecurrence>::create(P, t));
}

std::any Antlr2AST::visitSpecPrecedence(        referee::refereeParser::SpecPrecedenceContext*          ctx)
{
    auto    P   = std::any_cast<Expr*>(ctx->exprP()->accept(this));
    auto    S   = std::any_cast<Expr*>(ctx->exprS()->accept(this));
    auto    t   = std::any_cast<Time*>(ctx->intervalBound()->accept(this));

    return  static_cast<Spec*>(Factory<SpecPrecedence>::create(P, S, t));
}

std::any Antlr2AST::visitSpecPrecedenceChain12( referee::refereeParser::SpecPrecedenceChain12Context*   ctx)
{
    auto    S   = std::any_cast<Expr*>(ctx->exprS()->accept(this));
    auto    T   = std::any_cast<Expr*>(ctx->exprT()->accept(this));
    auto    P   = std::any_cast<Expr*>(ctx->exprP()->accept(this));
    auto    st  = std::any_cast<Time*>(ctx->upperTimeBound()->accept(this));
    auto    ps  = std::any_cast<Time*>(ctx->intervalBound()->accept(this));

    return  static_cast<Spec*>(Factory<SpecPrecedenceChain12>::create(S, T, P, st, ps));
}

std::any Antlr2AST::visitSpecPrecedenceChain21( referee::refereeParser::SpecPrecedenceChain21Context*   ctx)
{
    auto    P   = std::any_cast<Expr*>(ctx->exprP()->accept(this));
    auto    S   = std::any_cast<Expr*>(ctx->exprS()->accept(this));
    auto    T   = std::any_cast<Expr*>(ctx->exprT()->accept(this));
    auto    st  = std::any_cast<Time*>(ctx->upperTimeBound()->accept(this));
    auto    ps  = std::any_cast<Time*>(ctx->intervalBound()->accept(this));

    return  static_cast<Spec*>(Factory<SpecPrecedenceChain21>::create(P, S, T, st, ps));
}

std::any Antlr2AST::visitSpecResponse(          referee::refereeParser::SpecResponseContext*            ctx)
{
    auto    P   = std::any_cast<Expr*>(ctx->exprP()->accept(this));
    auto    S   = std::any_cast<Expr*>(ctx->exprS()->accept(this));
    auto    tPS = std::any_cast<Time*>(ctx->timeBound()->accept(this));
    auto    cPS = std::any_cast<Expr*>(ctx->constraint()->accept(this));

    return  static_cast<Spec*>(Factory<SpecResponse>::create(P, S, tPS, cPS));
}

std::any Antlr2AST::visitSpecResponseChain12(   referee::refereeParser::SpecResponseChain12Context*     ctx)
{
    auto    P   = std::any_cast<Expr*>(ctx->exprP()->accept(this));
    auto    S   = std::any_cast<Expr*>(ctx->exprS()->accept(this));
    auto    T   = std::any_cast<Expr*>(ctx->exprT()->accept(this));
    auto    tPS = std::any_cast<Time*>(ctx->timeBound(0)->accept(this));
    auto    tST = std::any_cast<Time*>(ctx->timeBound(1)->accept(this));
    auto    cPS = std::any_cast<Expr*>(ctx->constraint(0)->accept(this));
    auto    cST = std::any_cast<Expr*>(ctx->constraint(1)->accept(this));

    return  static_cast<Spec*>(Factory<SpecResponseChain12>::create(P, S, T, tPS, tST, cPS, cST));
}

std::any Antlr2AST::visitSpecResponseChain21(   referee::refereeParser::SpecResponseChain21Context*     ctx)
{
    auto    S   = std::any_cast<Expr*>(ctx->exprS()->accept(this));
    auto    T   = std::any_cast<Expr*>(ctx->exprT()->accept(this));
    auto    P   = std::any_cast<Expr*>(ctx->exprP()->accept(this));
    auto    tPS = std::any_cast<Time*>(ctx->timeBound(0)->accept(this));
    auto    tST = std::any_cast<Time*>(ctx->timeBound(1)->accept(this));
    auto    cPS = std::any_cast<Expr*>(ctx->constraint(0)->accept(this));
    auto    cST = std::any_cast<Expr*>(ctx->constraint(1)->accept(this));

    return  static_cast<Spec*>(Factory<SpecResponseChain21>::create(S, T, P, tPS, tST, cPS, cST));
}

std::any Antlr2AST::visitSpecResponseInvariance(referee::refereeParser::SpecResponseInvarianceContext*  ctx)
{
    auto    P   = std::any_cast<Expr*>(ctx->exprP()->accept(this));
    auto    S   = std::any_cast<Expr*>(ctx->exprS()->accept(this));
    auto    t   = std::any_cast<Time*>(ctx->timeBound()->accept(this));

    return  static_cast<Spec*>(Factory<SpecResponseInvariance>::create(P, S, t));
}

std::any Antlr2AST::visitSpecUntil(             referee::refereeParser::SpecUntilContext*               ctx)
{
    auto    P   = std::any_cast<Expr*>(ctx->exprP()->accept(this));
    auto    S   = std::any_cast<Expr*>(ctx->exprS()->accept(this));
    auto    t   = std::any_cast<Time*>(ctx->timeBound()->accept(this));

    return  static_cast<Spec*>(Factory<SpecUntil>::create(P, S, t));
}

std::any Antlr2AST::visitTimeBound(             referee::refereeParser::TimeBoundContext*               ctx)
{
    return  static_cast<Time*>(Factory<TimeMin>::create(Factory<ExprConstInteger>::create(5)));
}

std::any Antlr2AST::visitUpperTimeBound(        referee::refereeParser::UpperTimeBoundContext*          ctx)
{
    return  static_cast<Time*>(Factory<TimeMin>::create(Factory<ExprConstInteger>::create(5)));

}

std::any Antlr2AST::visitLowerTimeBound(        referee::refereeParser::LowerTimeBoundContext*          ctx)
{
    return  static_cast<Time*>(Factory<TimeMin>::create(Factory<ExprConstInteger>::create(5)));
}

std::any Antlr2AST::visitIntervalBound(         referee::refereeParser::IntervalBoundContext*           ctx)
{
    return  static_cast<Time*>(Factory<TimeMin>::create(Factory<ExprConstInteger>::create(5)));
}

std::any Antlr2AST::visitNoTimeBound(           referee::refereeParser::NoTimeBoundContext*             ctx)
{
    return  static_cast<Time*>(nullptr);
}

std::any Antlr2AST::visitConstraint(            referee::refereeParser::ConstraintContext*              ctx)
{
    if(ctx->exprZ())
    {
        return ctx->exprZ()->accept(this);
    }

    return static_cast<Expr*>(nullptr);
}
