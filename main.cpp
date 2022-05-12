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

#include <iostream>
#include <string>
#include <memory>
#include <map>

#include "visitor.hpp"
#include "factory.hpp"
#include "canonic.hpp"
#include "syntax.hpp"

#include "antlr4-runtime/antlr4-runtime.h"
#include "refereeParser.h"
#include "refereeLexer.h"
#include "refereeBaseVisitor.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

static std::unique_ptr<llvm::LLVMContext>     TheContext;
static std::unique_ptr<llvm::Module>          TheModule;
static std::unique_ptr<llvm::IRBuilder<>>     Builder;
static std::map<std::string, llvm::Value *>   NamedValues;


class RefereeVisitor
    : public referee::refereeBaseVisitor
{
private:

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
    std::any visitExprRef(  referee::refereeParser::ExprRefContext*     ctx) override   { return static_cast<Expr*>(Factory<ExprTrue>::create()); }
    std::any visitExprConst(referee::refereeParser::ExprConstContext*   ctx) override   { return static_cast<Expr*>(Factory<ExprTrue>::create()); }

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
};

int main(int argc, const char * argv[]) {
    std::string                 filename    = argv[1];
    std::ifstream               stream(filename);

    antlr4::ANTLRInputStream    input(stream);
    referee::refereeLexer       lexer(&input);
    antlr4::CommonTokenStream   tokens(&lexer);
    referee::refereeParser      parser(&tokens);
    RefereeVisitor              visitor;

    auto*   tree    = parser.program();
    visitor.visitProgram(tree);

    TheContext  = std::make_unique<llvm::LLVMContext>();
    TheModule   = std::make_unique<llvm::Module>(filename, *TheContext);
    Builder     = std::make_unique<llvm::IRBuilder<>>(*TheContext);

    TheModule->setSourceFileName(filename);

    auto    funcType= llvm::FunctionType::get(Builder->getVoidTy(), false);
    auto    function= llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", TheModule.get());
    llvm::verifyFunction(*function);
    TheModule->dump();

    return 1;
}
