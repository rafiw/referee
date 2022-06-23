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
#include <numeric>

#include "visitor.hpp"
#include "factory.hpp"
#include "syntax.hpp"
#include "visitors/canonic.hpp"
#include "visitors/compile.hpp"
#include "visitors/csvHeaders.hpp"

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
#include "llvm/IR/TypeFinder.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ExecutionEngine/JITSymbol.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/ExecutionUtils.h"
#include "llvm/ExecutionEngine/Orc/ExecutorProcessControl.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/IRTransformLayer.h"
#include "llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

#include "antlr2ast.hpp"

#include <spdlog/spdlog.h>
#include "spdlog/fmt/fmt.h"
#include <spdlog/fmt/ostr.h>

#include <CLI/App.hpp>
#include "CLI/Formatter.hpp"
#include "CLI/Config.hpp"

static std::unique_ptr<llvm::LLVMContext>     TheContext;
static std::unique_ptr<llvm::Module>          TheModule;
static std::unique_ptr<llvm::IRBuilder<>>     Builder;
static std::map<std::string, llvm::Value *>   NamedValues;


int main(int argc, char * argv[])
{
    CLI::App    app("referee");
    
    std::string refFilename = "default";
    bool        fCsvHeaders = false;
    bool        fLlvmTypes  = false;

    app.add_option( "--file",       refFilename,    "REF file to parse")
        ->check(CLI::ExistingFile);
    app.add_flag(   "--csv-headers",fCsvHeaders,    "Generate CSV headers");
    app.add_flag(   "--llvm-types", fLlvmTypes,     "Dump LLVM types");
    
    try {
        app.parse(argc, argv);

        std::ifstream               stream(refFilename);

        antlr4::ANTLRInputStream    input(stream);
        referee::refereeLexer       lexer(&input);
        antlr4::CommonTokenStream   tokens(&lexer);
        referee::refereeParser      parser(&tokens);
        Antlr2AST                   antlr2ast;

        auto*   tree    = parser.program();
        auto*   module  = std::any_cast<Module*>(antlr2ast.visitProgram(tree));
        
        for(auto name: module->getTypeNames())
        {
            std::cout << "type: " <<  name << std::endl;
        }

        if(fCsvHeaders)
        {
            for(auto name: module->getPropNames())
            {
                auto type   = module->getProp(name);
                auto cols   = CsvHeaders::make(name, type);

                auto init   = std::string("__time__");
                auto headers= std::accumulate(cols.begin(), cols.end(), init, [](std::string a, std::string b) { return a + "," + b; });

                std::cout << headers << std::endl;
            }

            for(auto name: module->getConfNames())
            {
                auto type   = module->getConf(name);
                auto cols   = CsvHeaders::make(name, type);

                auto init   = cols.front();
                auto headers= std::accumulate(std::next(cols.begin()), cols.end(), init, [](std::string a, std::string b) { return a + "," + b; });

                std::cout << headers << std::endl;
            }
        }

        if(fLlvmTypes)
        {
            TheContext  = std::make_unique<llvm::LLVMContext>();
            TheModule   = std::make_unique<llvm::Module>(refFilename, *TheContext);
            Builder     = std::make_unique<llvm::IRBuilder<>>(*TheContext);       

            TheModule->setSourceFileName(refFilename);

            Compile::make(TheContext.get(), TheModule.get(), module);
/*
            for(auto name: module->getPropNames())
            {
                auto type = module->getProp(name);
                TheModule->getOrInsertGlobal(name, Compile::make(TheContext.get(), TheModule.get(), type, name));
            }
            */
/*
            for(auto name: module->getTypeNames())
            {
                auto    type    = module->getType(name);
                Compile::make(TheContext.get(), TheModule.get(), type, name)->print(llvm::outs());
                std::cout << std::endl;
            } 
            std::cout << std::endl;
            */

            llvm::TypeFinder    StructTypes;
            StructTypes.run(*TheModule.get(), false);

            for (auto *STy : StructTypes)
                STy->dump();//print(llvm::outs());
                std::cout << std::endl;
            }

            std::cout << std::endl << std::endl;
            std::cout << "non optimized" << std::endl;
            TheModule->dump();
/*
            auto EPC    = llvm::orc::SelfExecutorProcessControl::Create();
            auto ES     = std::make_unique<llvm::orc::ExecutionSession>(std::move(*EPC));
            auto JTMB   = llvm::orc::JITTargetMachineBuilder(ES->getExecutorProcessControl().getTargetTriple());
            auto DL     = JTMB.getDefaultDataLayoutForTarget();
*/
              // Create a new pass manager attached to it.
            auto    TheFPM = std::make_unique<llvm::legacy::FunctionPassManager>(TheModule.get());

            TheFPM->add(llvm::createLoopStrengthReducePass());
            TheFPM->add(llvm::createLoopLoadEliminationPass());
            TheFPM->add(llvm::createLoopDataPrefetchPass());
            TheFPM->add(llvm::createLoopSimplifyCFGPass());
            TheFPM->add(llvm::createLoopGuardWideningPass());
            TheFPM->add(llvm::createLoopDistributePass());
            TheFPM->add(llvm::createInstructionCombiningPass());
            TheFPM->add(llvm::createReassociatePass());
            TheFPM->add(llvm::createGVNPass());
            TheFPM->add(llvm::createCFGSimplificationPass());

            TheFPM->doInitialization();
            auto& functions = TheModule->getFunctionList();

            for(auto iter = functions.begin(); iter != functions.end(); iter++)
                TheFPM->run(*iter);
            std::cout << std::endl << std::endl;
            std::cout << "optimized" << std::endl;
            TheModule->dump();

#if 0
        TheContext  = std::make_unique<llvm::LLVMContext>();
        TheModule   = std::make_unique<llvm::Module>(refFilename, *TheContext);
        Builder     = std::make_unique<llvm::IRBuilder<>>(*TheContext);

        TheModule->setSourceFileName(refFilename);
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
#endif
    }
    catch (const CLI::ParseError &e)
    {
        return app.exit(e);
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
