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

    llvm::Type*     make(Type* type, std::string name);
    llvm::Value*    make(Expr* expr);

    void    visit(TypeInteger*          type) override;
    void    visit(TypeNumber*           type) override;
    void    visit(TypeString*           type) override;
    void    visit(TypeBoolean*          type) override;
    void    visit(TypeStruct*           type) override;
    void    visit(TypeEnum*             type) override;
    void    visit(TypeArray*            type) override;

    llvm::Type*         m_type  = nullptr;
    std::string         m_name;

private:
    llvm::LLVMContext*  m_context;
    llvm::Module*       m_module;
    llvm::Function*     m_func;
    std::unique_ptr<llvm::IRBuilder<>>
                        m_builder;
};

struct CompileExprImpl
    : Visitor< ExprAdd
             , ExprAnd
             , ExprAt
             , ExprChoice
             , ExprConstBoolean
             , ExprConstInteger
             , ExprConstNumber
             , ExprConstString
             , ExprContext
             , ExprData
             , ExprDiv
             , ExprEq
             , ExprEqu
             , ExprF
             , ExprG
             , ExprGe
             , ExprGt
             , ExprH
             , ExprImp
             , ExprIndx
             , ExprInt
             , ExprLe
             , ExprLt
             , ExprMmbr
             , ExprMod
             , ExprMul
             , ExprNe
             , ExprNeg
             , ExprNot
             , ExprO
             , ExprOr
             , ExprParen
             , ExprRs
             , ExprRw
             , ExprSs
             , ExprSub
             , ExprSw
             , ExprTs
             , ExprTw
             , ExprUs
             , ExprUw
             , ExprXor
             , ExprXs
             , ExprXw
             , ExprYs
             , ExprYw>
{
    CompileExprImpl(
            llvm::LLVMContext*  context, 
            llvm::Module*       module,
            llvm::IRBuilder<>*  builder,
            llvm::Function*     function);

    void    visit(ExprAdd*          expr) override;
    void    visit(ExprAnd*          expr) override;
    void    visit(ExprAt*           expr) override;
    void    visit(ExprChoice*       expr) override;
    void    visit(ExprConstBoolean* expr) override;
    void    visit(ExprConstInteger* expr) override;
    void    visit(ExprConstNumber*  expr) override;
    void    visit(ExprConstString*  expr) override;
    void    visit(ExprContext*      expr) override;
    void    visit(ExprData*         expr) override;
    void    visit(ExprDiv*          expr) override;
    void    visit(ExprEq*           expr) override;
    void    visit(ExprEqu*          expr) override;
    void    visit(ExprF*            expr) override;
    void    visit(ExprG*            expr) override;
    void    visit(ExprGe*           expr) override;
    void    visit(ExprGt*           expr) override;
    void    visit(ExprH*            expr) override;
    void    visit(ExprImp*          expr) override;
    void    visit(ExprIndx*         expr) override;
    void    visit(ExprInt*          expr) override;
    void    visit(ExprLe*           expr) override;
    void    visit(ExprLt*           expr) override;
    void    visit(ExprMmbr*         expr) override;
    void    visit(ExprMod*          expr) override;
    void    visit(ExprMul*          expr) override;
    void    visit(ExprNe*           expr) override;
    void    visit(ExprNeg*          expr) override;
    void    visit(ExprNot*          expr) override;
    void    visit(ExprO*            expr) override;
    void    visit(ExprOr*           expr) override;
    void    visit(ExprParen*        expr) override;
    void    visit(ExprRs*           expr) override;
    void    visit(ExprRw*           expr) override;
    void    visit(ExprSs*           expr) override;
    void    visit(ExprSub*          expr) override;
    void    visit(ExprSw*           expr) override;
    void    visit(ExprTs*           expr) override;
    void    visit(ExprTw*           expr) override;
    void    visit(ExprUs*           expr) override;
    void    visit(ExprUw*           expr) override;
    void    visit(ExprXor*          expr) override;
    void    visit(ExprXs*           expr) override;
    void    visit(ExprXw*           expr) override;
    void    visit(ExprYs*           expr) override;
    void    visit(ExprYw*           expr) override;

    llvm::Value*    make(Expr* expr);

private:
    llvm::LLVMContext*  m_context;
    llvm::Module*       m_module;
    llvm::Function*     m_function;
    llvm::IRBuilder<>*  m_builder;

    llvm::Value*        m_value;
};


CompileTypeImpl::CompileTypeImpl(
            llvm::LLVMContext*  context, 
            llvm::Module*       module)
    : m_context(context)
    , m_module(module)
    , m_builder(std::make_unique<llvm::IRBuilder<>>(*m_context))
{
}

void    CompileTypeImpl::visit(TypeInteger*          type)
{
    m_type  = m_builder->getInt64Ty();
}

void    CompileTypeImpl::visit(TypeNumber*           type)
{
    m_type  = m_builder->getDoubleTy();
}

void    CompileTypeImpl::visit(TypeString*           type)
{
    m_type  = m_builder->getInt8PtrTy();
}

void    CompileTypeImpl::visit(TypeBoolean*          type)
{
    m_type  = m_builder->getInt8Ty();
}

void    CompileTypeImpl::visit(TypeStruct*           type)
{
    std::vector<llvm::Type*>    elements;
    
    for(auto member: type->members)
    {
        elements.push_back(Compile::make(m_context, m_module, member.data, m_name + "::" + member.name));
    }
    m_type  = llvm::StructType::create(*m_context, elements, m_name);
    m_type->print(llvm::outs()); std::cout << std::endl;
}

void    CompileTypeImpl::visit(TypeEnum*             type)
{
    m_type  = m_builder->getInt8Ty();
}

void    CompileTypeImpl::visit(TypeArray*            type)
{
    std::vector<llvm::Type*>    elements;

    elements.push_back(m_builder->getInt16Ty());
    auto    base    = Compile::make(m_context, m_module, type->type, m_name + "[]");
    auto    size    = type->size;

    if(size == 0)
    {
        elements.push_back(llvm::PointerType::get(base, 0));

        m_type  = llvm::StructType::create(*m_context, elements, m_name);
    }
    else
    {
        m_type  = llvm::ArrayType::get(base, size);
    }
}

llvm::Type* CompileTypeImpl::make(Type*  type, std::string name)
{
    m_name = name;
    
    type->accept(*this);

    return  this->m_type;
}

CompileExprImpl::CompileExprImpl(
            llvm::LLVMContext*  context, 
            llvm::Module*       module,
            llvm::IRBuilder<>*  builder,
            llvm::Function*     function)
    : m_context(context)
    , m_module(module)
    , m_function(function)
    , m_builder(builder)
{
}

void    CompileExprImpl::visit(ExprAdd*          expr)
{
}

void    CompileExprImpl::visit(ExprAnd*          expr)
{
}

void    CompileExprImpl::visit(ExprAt*           expr)
{
}

void    CompileExprImpl::visit(ExprChoice*       expr)
{
}

void    CompileExprImpl::visit(ExprConstBoolean* expr)
{
}

void    CompileExprImpl::visit(ExprConstInteger* expr)
{
    printf("%s\n", __func__);
    m_value = llvm::ConstantInt::getSigned(m_builder->getInt64Ty(), expr->value);
}

void    CompileExprImpl::visit(ExprConstNumber*  expr)
{
}

void    CompileExprImpl::visit(ExprConstString*  expr)
{
}

void    CompileExprImpl::visit(ExprContext*      expr)
{
    if(expr->name == "__curr__")
        m_value = m_function->arg_begin();
    else
        throw std::runtime_error("implement");
}

void    CompileExprImpl::visit(ExprData*         expr)
{
    printf("%s\n", __func__);

    auto    ctxtPtr     = make(expr->ctxt);
    auto    ctxtPtrType = cast<llvm::PointerType>(ctxtPtr->getType());
    auto    ctxtType    = ctxtPtrType->getPointerElementType();

    auto    propPtrPtr      = m_builder->CreateStructGEP(ctxtType, ctxtPtr, 1);
    auto    propPtrPtrType  = cast<llvm::PointerType>(propPtrPtr->getType());
    auto    propPtrType     = cast<llvm::PointerType>(propPtrPtrType->getPointerElementType());
    auto    propType        = propPtrType->getPointerElementType();
    
    auto    propPtr = m_builder->CreateLoad(propPtrType, propPtrPtr, "load_prop");
}

void    CompileExprImpl::visit(ExprDiv*          expr)
{
}

void    CompileExprImpl::visit(ExprEq*           expr)
{
    printf("%s\n", __func__);

    auto    lhs     = make(expr->lhs);
    auto    rhs     = make(expr->rhs);

    lhs->print(llvm::outs());  std::cout << std::endl;
    rhs->print(llvm::outs());  std::cout << std::endl;

    m_value = m_builder->CreateICmp(llvm::CmpInst::Predicate::ICMP_NE, lhs, rhs);
}

void    CompileExprImpl::visit(ExprEqu*          expr)
{
}

void    CompileExprImpl::visit(ExprF*            expr)
{
}

void    CompileExprImpl::visit(ExprG*            expr)
{
}

void    CompileExprImpl::visit(ExprGe*           expr)
{
}

void    CompileExprImpl::visit(ExprGt*           expr)
{
}

void    CompileExprImpl::visit(ExprH*            expr)
{
}

void    CompileExprImpl::visit(ExprImp*          expr)
{
}

void    CompileExprImpl::visit(ExprIndx*         expr)
{
}

void    CompileExprImpl::visit(ExprInt*          expr)
{
}

void    CompileExprImpl::visit(ExprLe*           expr)
{
}

void    CompileExprImpl::visit(ExprLt*           expr)
{
}

void    CompileExprImpl::visit(ExprMmbr*         expr)
{
    auto    basePtr     = make(expr->arg);
    auto    basePtrType = cast<llvm::PointerType>(basePtr->getType());
    auto    baseType    = basePtrType->getPointerElementType();

    auto    dataPtr     = m_builder->CreateStructGEP(baseType, basePtr, 0);   //  TODO: get field name
    auto    dataPtrType = cast<llvm::PointerType>(dataPtr->getType());
    auto    dataType    = dataPtrType->getPointerElementType();

    m_value = m_builder->CreateLoad(dataType, dataPtr, "load_data");
        m_value->print(llvm::outs());  std::cout << std::endl;
}

void    CompileExprImpl::visit(ExprMod*          expr)
{
}

void    CompileExprImpl::visit(ExprMul*          expr)
{
}

void    CompileExprImpl::visit(ExprNe*           expr)
{
        printf("%s\n", __func__);

    auto    lhs     = make(expr->lhs);
    auto    rhs     = make(expr->rhs);
    m_value = m_builder->CreateICmp(llvm::CmpInst::Predicate::ICMP_NE, lhs, rhs);
}

void    CompileExprImpl::visit(ExprNeg*          expr)
{
}

void    CompileExprImpl::visit(ExprNot*          expr)
{
}

void    CompileExprImpl::visit(ExprO*            expr)
{
}

void    CompileExprImpl::visit(ExprOr*           expr)
{
}

void    CompileExprImpl::visit(ExprParen*        expr)
{
}

void    CompileExprImpl::visit(ExprRs*           expr)
{
}

void    CompileExprImpl::visit(ExprRw*           expr)
{
}

void    CompileExprImpl::visit(ExprSs*           expr)
{
}

void    CompileExprImpl::visit(ExprSub*          expr)
{
}

void    CompileExprImpl::visit(ExprSw*           expr)
{
}

void    CompileExprImpl::visit(ExprTs*           expr)
{
}

void    CompileExprImpl::visit(ExprTw*           expr)
{
}

void    CompileExprImpl::visit(ExprUs*           expr)
{
}

void    CompileExprImpl::visit(ExprUw*           expr)
{
}

void    CompileExprImpl::visit(ExprXor*          expr)
{
}

void    CompileExprImpl::visit(ExprXs*           expr)
{
}

void    CompileExprImpl::visit(ExprXw*           expr)
{
}

void    CompileExprImpl::visit(ExprYs*           expr)
{
}

void    CompileExprImpl::visit(ExprYw*           expr)
{
}

llvm::Value*    CompileExprImpl::make(Expr* expr)
{
    m_value = llvm::ConstantInt::getTrue(*m_context);

    expr->accept(*this);
    
    return  m_value;
}

llvm::Type* Compile::make(llvm::LLVMContext* context, llvm::Module* module, Type* type, std::string name)
{
    CompileTypeImpl impl(context, module);

    return impl.make(type, name);
}

llvm::Value*Compile::make(llvm::LLVMContext* context, llvm::Module* module, Expr* expr)
{
    return nullptr;
}

void Compile::make(llvm::LLVMContext* context, llvm::Module* module, Module* mod)
{
    auto    builder = std::make_unique<llvm::IRBuilder<>>(*context);

    //  create __conf__
    std::vector<llvm::Type*>    confTypes;
    auto    confNames   = mod->getConfNames();
    for(auto name: confNames)
    {
        auto    type    = mod->getConf(name);
        confTypes.push_back(llvm::PointerType::get(make(context, module, type, name), 0));
    }
    auto    confType    = llvm::StructType::create(*context, confTypes, "__conf__");
    auto    confPtrType = llvm::PointerType::get(confType, 0);
    module->getOrInsertGlobal("__conf__", confType);

    //  create __prop__
    auto    propNames   = mod->getPropNames();
    std::vector<llvm::Type*>    propTypes;
    propTypes.push_back(builder->getInt64Ty()); //  __time__
    for(auto name: propNames)
    {
        if(name == "__time__")
            continue;
            
        std::cout << "prop: " << name << std::endl;
        auto    type    = mod->getProp(name);
        propTypes.push_back(llvm::PointerType::get(make(context, module, type, name), 0));
    }
    auto    propType    = llvm::StructType::create(*context, propTypes, "__prop__");
    auto    propPtrType = llvm::PointerType::get(propType, 0);
    module->getOrInsertGlobal("__prop__", propPtrType);


    auto    exprs   = mod->getExprs();
    for(auto expr: exprs)
    {
        auto    pos         = expr->where();
        auto    funcName    = std::to_string(pos.beg.row) + ":" + std::to_string(pos.beg.col) + " .. " + std::to_string(pos.end.row) + ":" + std::to_string(pos.end.col);
        auto    funcType    = llvm::FunctionType::get(builder->getInt8Ty(), {propPtrType, confPtrType}, false);
        auto    funcBody    = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, funcName, module);
        auto    funcArgs    = funcBody->args().begin();

        funcArgs->setName("prop");    funcArgs++;
        funcArgs->setName("conf");

        auto    bb          = llvm::BasicBlock::Create(*context, "entry", funcBody);
        builder->SetInsertPoint(bb);

        CompileExprImpl compExpr(context, module, builder.get(), funcBody);

        builder->CreateRet(compExpr.make(expr));
    }
}
