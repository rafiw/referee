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
#include "syntax.hpp"
#include "visitors/canonic.hpp"
#include "visitors/compile.hpp"

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

#include "antlr2ast.hpp"

static std::unique_ptr<llvm::LLVMContext>     TheContext;
static std::unique_ptr<llvm::Module>          TheModule;
static std::unique_ptr<llvm::IRBuilder<>>     Builder;
static std::map<std::string, llvm::Value *>   NamedValues;


int main(int argc, const char * argv[]) {
    try {
        std::string                 filename    = argv[1];
        std::ifstream               stream(filename);

        antlr4::ANTLRInputStream    input(stream);
        referee::refereeLexer       lexer(&input);
        antlr4::CommonTokenStream   tokens(&lexer);
        referee::refereeParser      parser(&tokens);
        Antlr2AST                   antlr2ast;

        auto*   tree    = parser.program();
        auto*   module  = std::any_cast<Module*>(antlr2ast.visitProgram(tree));

        TheContext  = std::make_unique<llvm::LLVMContext>();
        TheModule   = std::make_unique<llvm::Module>(filename, *TheContext);
        Builder     = std::make_unique<llvm::IRBuilder<>>(*TheContext);

        TheModule->setSourceFileName(filename);
#if 0
        //llvm::StructType::create(llvm::getGlobalContext(), members_array_ref, struct_name, false)
        auto intType            = llvm::IntegerType::get(*TheContext, 32); // 32 bits integer
        auto myStructType       = llvm::StructType::create(*TheContext, "myStruct"); // Create opaque type
        auto myStructPtrType    = llvm::PointerType::get(myStructType, 0); // Initialise the pointer type now
        myStructType->setBody({ intType, myStructPtrType }, /* packed */ false);
        myStructType->print(llvm::errs());
#endif
        auto    funcType= llvm::FunctionType::get(Builder->getVoidTy(), false);
        auto    function= llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", TheModule.get());
        llvm::verifyFunction(*function);
        TheModule->dump();


        for(auto name: module->getTypeNames())
        {
            auto    type    = module->getType(name);
            std::cout << name << std::endl << "    ";
            Compile::make(TheContext.get(), TheModule.get(), type)->print(llvm::outs());
            std::cout << std::endl;
        }
    }
    catch(Exception& e)
    {
        std::cerr << "exception: " << e.what() << std::endl;
    }
    catch(std::exception& e)
    {
        std::cerr << "exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
