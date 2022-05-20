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

#include "compile.hpp"

struct CompileTypeImpl
    : Visitor<  TypeInteger
             ,  TypeNumber
             ,  TypeString
             ,  TypeBoolean
             ,  TypeStruct
             ,  TypeEnum
             ,  TypeArray>
{
    CompileTypeImpl(
            llvm::LLVMContext*  context, 
            llvm::Module*       module);

    llvm::Type* make(Type*  type);

    void    visit(TypeInteger*          expr) override;
    void    visit(TypeNumber*           expr) override;
    void    visit(TypeString*           expr) override;
    void    visit(TypeBoolean*          expr) override;
    void    visit(TypeStruct*           expr) override;
    void    visit(TypeEnum*             expr) override;
    void    visit(TypeArray*            expr) override;

    llvm::Type*         type    = nullptr;

private:
    llvm::LLVMContext*  m_context;
    llvm::Module*       m_module;
};


CompileTypeImpl::CompileTypeImpl(
            llvm::LLVMContext*  context, 
            llvm::Module*       module)
    : m_context(context)
    , m_module(module)
{
}

void    CompileTypeImpl::visit(TypeInteger*          expr)
{
    auto    builder = std::make_unique<llvm::IRBuilder<>>(*m_context);

    type    = builder->getInt64Ty();
}

void    CompileTypeImpl::visit(TypeNumber*           expr)
{
    auto    builder = std::make_unique<llvm::IRBuilder<>>(*m_context);


    type    = builder->getDoubleTy();
}

void    CompileTypeImpl::visit(TypeString*           expr)
{
    auto    builder = std::make_unique<llvm::IRBuilder<>>(*m_context);


    type    = builder->getInt8PtrTy();
}

void    CompileTypeImpl::visit(TypeBoolean*          expr)
{
    auto    builder = std::make_unique<llvm::IRBuilder<>>(*m_context);

    type    = builder->getInt8Ty();
}

void    CompileTypeImpl::visit(TypeStruct*           expr)
{
    auto    builder = std::make_unique<llvm::IRBuilder<>>(*m_context);
    std::vector<llvm::Type*>    elements;
    
    for(auto member: expr->members)
    {
        elements.push_back(Compile::make(m_context, m_module, member.data));
    }
    type    = llvm::StructType::create(elements);
}

void    CompileTypeImpl::visit(TypeEnum*             expr)
{
    auto    builder = std::make_unique<llvm::IRBuilder<>>(*m_context);

    type    = builder->getInt8Ty();
}

void    CompileTypeImpl::visit(TypeArray*            expr)
{
    auto    builder = std::make_unique<llvm::IRBuilder<>>(*m_context);

    type    = builder->getInt8Ty();
}


llvm::Type* CompileTypeImpl::make(Type*  type)
{
    type->accept(*this);

    return  this->type;
}

llvm::Type* Compile::make(llvm::LLVMContext* context, llvm::Module* module, Type* type)
{
    CompileTypeImpl impl(context, module);

    return  impl.make(type);
}

void Compile::make(llvm::LLVMContext* theContext, llvm::Module* theModule, Expr* expr)
{

}