#include <iostream>
#include <string>
#include <memory>
#include <map>

#include "visitor.hpp"
#include "factory.hpp"
#include "canonic.hpp"

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
public:
    std::any visitExprUs(referee::refereeParser::ExprUsContext *ctx) override {
    
        std::cout << "Us " << std::endl;
        return visitChildren(ctx);
    }

    std::any visitExprF(referee::refereeParser::ExprFContext *ctx) override {
        std::cout << "F " << std::endl;
        return visitChildren(ctx);
    }
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

    return 0;
}
