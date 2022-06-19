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
#include "rewrite.hpp"
#include "printer.hpp"
#include "typecalc.hpp"
#include "../factory.hpp"

#include <functional>
#include <vector>

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
             , ExprConf
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
            llvm::Function*     function,
            Module*             refmod);

    void    visit(ExprAdd*          expr) override;
    void    visit(ExprAnd*          expr) override;
    void    visit(ExprAt*           expr) override;
    void    visit(ExprChoice*       expr) override;
    void    visit(ExprConstBoolean* expr) override;
    void    visit(ExprConstInteger* expr) override;
    void    visit(ExprConstNumber*  expr) override;
    void    visit(ExprConstString*  expr) override;
    void    visit(ExprContext*      expr) override;
    void    visit(ExprConf*         expr) override;
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

    void    compare(
                llvm::CmpInst::Predicate    ipred, 
                llvm::CmpInst::Predicate    fpred, ExprBinary* expr);
    void    arithmetic(
                ExprBinary* expr,
                std::function<llvm::Value*(llvm::Value*, llvm::Value*)> ifunc,
                std::function<llvm::Value*(llvm::Value*, llvm::Value*)> ffunc);

    void    XY( ExprBinary*         expr,
                llvm::Value*        direction,
                llvm::Value*        endValue,
                std::string         name);

    void    UR( Temporal<ExprBinary>*   expr,
                llvm::Value*            rhsV,
                llvm::Value*            lhsV,
                llvm::Value*            endV,
                std::string             name);
    void    ST( Temporal<ExprBinary>*   expr,
                llvm::Value*            rhsV,
                llvm::Value*            lhsV,
                llvm::Value*            endV,
                std::string             name);

    llvm::Value*    make(Expr* expr);

private:
    llvm::Value*        add(llvm::Value* lhs, llvm::Value* rhs, std::string const& name);
    llvm::Value*        mul(llvm::Value* lhs, llvm::Value* rhs, std::string const& name);
    llvm::Value*        sub(llvm::Value* lhs, llvm::Value* rhs, std::string const& name);

private:
    llvm::LLVMContext*  m_context;
    llvm::Module*       m_module;
    llvm::Function*     m_function;
    llvm::IRBuilder<>*  m_builder;

    llvm::Value*        m_value;
    std::vector<llvm::Value*>
                        m_curr;
    std::vector<std::pair<std::string, llvm::Value*>>
                        m_name2value;

    Module*             m_refmod;
    llvm::Value*        m_0;
    llvm::Value*        m_p1;
    llvm::Value*        m_m1;
    llvm::Value*        m_T;
    llvm::Value*        m_F;
    llvm::Value*        m_frst;
    llvm::Value*        m_last;
    llvm::Value*        m_conf;
    llvm::Type*         m_propType;
    llvm::Type*         m_confType;
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
    m_type  = m_builder->getInt1Ty();
}

void    CompileTypeImpl::visit(TypeStruct*           type)
{
    std::vector<llvm::Type*>    elements;
    
    for(auto member: type->members)
    {
        elements.push_back(Compile::make(m_context, m_module, member.data, m_name + "::" + member.name));
    }
    m_type  = llvm::StructType::create(*m_context, elements, m_name);
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
            llvm::Function*     function,
            Module*             refmod)
    : m_context(context)
    , m_module(module)
    , m_function(function)
    , m_builder(builder)
    , m_refmod(refmod)
{
    m_0     = llvm::ConstantInt::getSigned(m_builder->getInt64Ty(), 0);
    m_p1    = llvm::ConstantInt::getSigned(m_builder->getInt64Ty(), +1);
    m_m1    = llvm::ConstantInt::getSigned(m_builder->getInt64Ty(), -1);
    m_T     = llvm::ConstantInt::getTrue(*m_context);
    m_F     = llvm::ConstantInt::getFalse(*m_context);

    auto    iter    = function->arg_begin();
    
    m_frst  = iter++;
    m_last  = iter++;
    m_conf  = iter;
    m_propType  = cast<llvm::PointerType>(m_frst->getType())->getPointerElementType();
    m_confType  = cast<llvm::PointerType>(m_conf->getType())->getPointerElementType();

    m_curr.push_back(m_builder->CreateGEP(m_propType, m_frst, m_p1, "curr"));
}
 
void    CompileExprImpl::visit(ExprAdd*          expr)
{
    arithmetic(
        expr, 
        [this](llvm::Value* lhs, llvm::Value* rhs) {return m_builder->CreateAdd(lhs, rhs);},
        [this](llvm::Value* lhs, llvm::Value* rhs) {return m_builder->CreateFAdd(lhs, rhs);});
}

void    CompileExprImpl::visit(ExprAnd*          expr)
{
    auto    lhs = make(expr->lhs);
    auto    rhs = make(expr->rhs);
    m_value = m_builder->CreateLogicalAnd(lhs, rhs);
}

void    CompileExprImpl::visit(ExprAt*           expr)
{
    m_name2value.push_back(std::make_pair(expr->name, m_curr.back()));
    m_value = make(expr->arg);
    m_name2value.pop_back();
}

void    CompileExprImpl::visit(ExprChoice*       expr)
{
}

void    CompileExprImpl::visit(ExprConstBoolean* expr)
{
    m_value = llvm::ConstantInt::getBool(*m_context, expr->value);
}

void    CompileExprImpl::visit(ExprConstInteger* expr)
{
    m_value = llvm::ConstantInt::getSigned(m_builder->getInt64Ty(), expr->value);
}

void    CompileExprImpl::visit(ExprConstNumber*  expr)
{
    m_value = llvm::ConstantFP::get(m_builder->getDoubleTy(), expr->value);
}

void    CompileExprImpl::visit(ExprConstString*  expr)
{
}

void    CompileExprImpl::visit(ExprContext*      expr)
{
    if(expr->name == "__curr__")
    {
        m_value = m_curr.back();
    }
    else if(expr->name == "__conf__")
    {
        m_value = m_conf;
    }
    else
    {
        for(auto it = m_name2value.rbegin(); it != m_name2value.rend(); it++)
        {
            if(expr->name == it->first)
            {
                m_value = it->second;
                return;
            }
        }
    
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
}

void    CompileExprImpl::visit(ExprConf*         expr)
{
    auto    ctxtPtr         = make(expr->ctxt);
    auto    ctxtPtrType     = cast<llvm::PointerType>(ctxtPtr->getType());
    auto    ctxtType        = ctxtPtrType->getPointerElementType();

    auto    confPtrPtr      = m_builder->CreateStructGEP(ctxtType, ctxtPtr, dynamic_cast<TypeContext*>(expr->ctxt->type())->index(expr->name));
    auto    confPtrPtrType  = cast<llvm::PointerType>(confPtrPtr->getType());
    auto    confPtrType     = cast<llvm::PointerType>(confPtrPtrType->getPointerElementType());

    auto    confType        = confPtrType->getPointerElementType();

    m_value = m_builder->CreateLoad(confPtrType, confPtrPtr, false, "ptr_" + expr->name);

    if(dynamic_cast<TypePrimitive*>(expr->type()))
    {
        m_value = m_builder->CreateLoad(confPtrType->getPointerElementType(), m_value, false, "val_" + expr->name);
    }        
}

void    CompileExprImpl::visit(ExprData*         expr)
{
    auto    ctxtPtr     = make(expr->ctxt);
    auto    ctxtPtrType = cast<llvm::PointerType>(ctxtPtr->getType());
    auto    ctxtType    = ctxtPtrType->getPointerElementType();

    if(expr->name == "__time__")
    {
        auto    propPtr         = m_builder->CreateStructGEP(ctxtType, ctxtPtr, 0);     //  skip __time__
        auto    propPtrType     = cast<llvm::PointerType>(propPtr->getType());
        auto    propType        = propPtrType->getPointerElementType();

        m_value = m_builder->CreateLoad(propType, propPtr, false, "__time__");
    }
    else
    {
        auto    propPtrPtr      = m_builder->CreateStructGEP(ctxtType, ctxtPtr, dynamic_cast<TypeContext*>(expr->ctxt->type())->index(expr->name) + 1); //  +1 to skip __time__
        auto    propPtrPtrType  = cast<llvm::PointerType>(propPtrPtr->getType());
        auto    propPtrType     = cast<llvm::PointerType>(propPtrPtrType->getPointerElementType());

        auto    propType        = propPtrType->getPointerElementType();

        m_value = m_builder->CreateLoad(propPtrType, propPtrPtr, false, "ptr_" + expr->name);

        if(dynamic_cast<TypePrimitive*>(expr->type()))
        {
            m_value = m_builder->CreateLoad(propPtrType->getPointerElementType(), m_value, false, "val_" + expr->name);
        }
    }
}

void    CompileExprImpl::visit(ExprDiv*          expr)
{
    arithmetic(
        expr, 
        [this](llvm::Value* lhs, llvm::Value* rhs) {return m_builder->CreateSDiv(lhs, rhs);},
        [this](llvm::Value* lhs, llvm::Value* rhs) {return m_builder->CreateFDiv(lhs, rhs);});
}

void    CompileExprImpl::compare(
            llvm::CmpInst::Predicate    ipred, 
            llvm::CmpInst::Predicate    fpred, ExprBinary* expr)
{
    auto    lhs     = make(expr->lhs);
    auto    rhs     = make(expr->rhs);
    auto    lhsT    = expr->lhs->type();
    auto    rhsT    = expr->rhs->type();

    if(     lhsT == Factory<TypeNumber>::create() 
        && rhsT == Factory<TypeInteger>::create())
    {
        rhs = m_builder->CreateSIToFP(rhs, lhs->getType());
        m_value = m_builder->CreateFCmp(fpred, lhs, rhs);
    }
    else if(lhsT == Factory<TypeInteger>::create() 
        &&  rhsT == Factory<TypeNumber>::create())
    {
        lhs = m_builder->CreateSIToFP(lhs, rhs->getType());
        m_value = m_builder->CreateFCmp(fpred, lhs, rhs);
    }
    else if(lhsT == Factory<TypeInteger>::create() 
        &&  rhsT == Factory<TypeInteger>::create())
    {
        m_value = m_builder->CreateICmp(ipred, lhs, rhs);
    }
    else if(lhsT == Factory<TypeNumber>::create() 
        &&  rhsT == Factory<TypeNumber>::create())
    {
        m_value = m_builder->CreateFCmp(fpred, lhs, rhs);
    }
    else if(lhsT == Factory<TypeString>::create()
        &&  rhsT == Factory<TypeString>::create())
    {
        m_value = m_builder->CreateICmp(ipred, lhs, rhs);   //  TODO: check
    }
    else if(lhsT == Factory<TypeBoolean>::create()
        &&  rhsT == Factory<TypeBoolean>::create())
    {
        m_value = m_builder->CreateICmp(ipred, lhs, rhs);   //  TODO: check
    }
    else
    {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
}

void    CompileExprImpl::arithmetic(
                ExprBinary* expr,
                std::function<llvm::Value*(llvm::Value*, llvm::Value*)> ifunc,
                std::function<llvm::Value*(llvm::Value*, llvm::Value*)> ffunc)
{
    auto    lhs     = make(expr->lhs);
    auto    rhs     = make(expr->rhs);
    auto    lhsT    = expr->lhs->type();
    auto    rhsT    = expr->rhs->type();

    if(     lhsT == Factory<TypeNumber>::create() 
        && rhsT == Factory<TypeInteger>::create())
    {
        rhs = m_builder->CreateSIToFP(rhs, lhs->getType());
        m_value = ffunc(lhs, rhs);
    }
    else if(lhsT == Factory<TypeInteger>::create() 
        &&  rhsT == Factory<TypeNumber>::create())
    {
        lhs = m_builder->CreateSIToFP(lhs, rhs->getType());
        m_value = ffunc(lhs, rhs);
    }
    else if(lhsT == Factory<TypeInteger>::create() 
        &&  rhsT == Factory<TypeInteger>::create())
    {
        m_value = ifunc(lhs, rhs);
    }
    else if(lhsT == Factory<TypeNumber>::create() 
        &&  rhsT == Factory<TypeNumber>::create())
    {
        m_value = ffunc(lhs, rhs);
    }
    else
    {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }    
}

void    CompileExprImpl::visit(ExprEq*           expr)
{
    compare(llvm::CmpInst::Predicate::ICMP_EQ, llvm::CmpInst::Predicate::FCMP_OEQ, expr);
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
    compare(llvm::CmpInst::Predicate::ICMP_SGE, llvm::CmpInst::Predicate::FCMP_OGE, expr);
}

void    CompileExprImpl::visit(ExprGt*           expr)
{
    compare(llvm::CmpInst::Predicate::ICMP_SGT, llvm::CmpInst::Predicate::FCMP_OGT, expr);
}

void    CompileExprImpl::visit(ExprH*            expr)
{
}

void    CompileExprImpl::visit(ExprImp*          expr)
{
    auto    lhs = make(expr->lhs);
    auto    rhs = make(expr->rhs);
    m_value = m_builder->CreateOr(m_builder->CreateNot(lhs), rhs);
}

void    CompileExprImpl::visit(ExprIndx*         expr)
{
    auto    exprType    = expr->type();
    auto    basePtr     = make(expr->lhs);
    auto    indx        = make(expr->rhs);
    auto    basePtrType = cast<llvm::PointerType>(basePtr->getType());
    auto    baseType    = cast<llvm::ArrayType>(basePtrType->getPointerElementType());
    auto    elemType    = baseType->getElementType();

    m_value = m_builder->CreateGEP(baseType, basePtr, {m_0, indx}, "ptr_[]");
    m_value->print(llvm::outs()); std::cout << std::endl;

    if(dynamic_cast<TypePrimitive*>(exprType))
    {
        m_value = m_builder->CreateLoad(elemType, m_value, false, "val_[]");
    }
}

llvm::Value*    CompileExprImpl::add(llvm::Value* lhs, llvm::Value* rhs, std::string const& name)
{
    auto    typeInteger = m_builder->getInt64Ty();
    auto    typeNumber  = m_builder->getDoubleTy();

    if(lhs->getType() == typeInteger && rhs->getType() == typeInteger)
    {
        return  m_builder->CreateAdd(lhs, rhs, name);
    }

    if(lhs->getType() == typeInteger)
    {
        lhs = m_builder->CreateSIToFP(lhs, typeNumber);
    }

    if(rhs->getType() == typeInteger)
    {
        rhs = m_builder->CreateSIToFP(rhs, typeNumber);
    }
    
    return  m_builder->CreateFAdd(lhs, rhs, name);
}

llvm::Value*    CompileExprImpl::mul(llvm::Value* lhs, llvm::Value* rhs, std::string const& name)
{
    auto    typeInteger = m_builder->getInt64Ty();
    auto    typeNumber  = m_builder->getDoubleTy();

    if(lhs->getType() == typeInteger && rhs->getType() == typeInteger)
    {
        return  m_builder->CreateMul(lhs, rhs, name);
    }

    if(lhs->getType() == typeInteger)
    {
        lhs = m_builder->CreateSIToFP(lhs, typeNumber);
    }

    if(rhs->getType() == typeInteger)
    {
        rhs = m_builder->CreateSIToFP(rhs, typeNumber);
    }
    
    return  m_builder->CreateFMul(lhs, rhs, name);
}

llvm::Value*    CompileExprImpl::sub(llvm::Value* lhs, llvm::Value* rhs, std::string const& name)
{
    auto    typeInteger = m_builder->getInt64Ty();
    auto    typeNumber  = m_builder->getDoubleTy();

    if(lhs->getType() == typeInteger && rhs->getType() == typeInteger)
    {
        return  m_builder->CreateSub(lhs, rhs, name);
    }

    if(lhs->getType() == typeInteger)
    {
        lhs = m_builder->CreateSIToFP(lhs, typeNumber);
    }

    if(rhs->getType() == typeInteger)
    {
        rhs = m_builder->CreateSIToFP(rhs, typeNumber);
    }
    
    return  m_builder->CreateFSub(lhs, rhs, name);
}

void    CompileExprImpl::visit(ExprInt*          expr)
{
/*
typedef long long uint64_t;
typedef struct prop_t
{
    uint64_t    __time__;
    uint64_t    a;
} prop_t;

bool        boolean(prop_t const* curr);
bool        integer(prop_t const* curr);

uint64_t    integral(uint64_t lo, uint64_t hi, prop_t const* curr, prop_t const* frst, prop_t const* last)
{
    prop_t const*   next    = curr + 1;
    uint64_t        result  = 0;

    lo  = lo + curr->__time__;
    hi  = hi + curr->__time__;

    while(next <= last)
    {
        if(hi <= curr->__time__)
        {
            break;
        }

        uint64_t    _lo = curr->__time__ < lo ? lo : curr->__time__;
        uint64_t    _hi = hi < next->__time__ ? hi : next->__time__;

        if(_lo < _hi)
        {
            if(boolean(curr))
            {
                uint64_t height = eval(curr);
                uint64_t length = (_hi - _lo);
                uint64_t volume = height * length;
                result  +=  volume;
            }
        }

        curr    = next;
        next    = next + 1;
    }

    return  result;
}

uint64_t    integral(prop_t const* curr, prop_t const* frst, prop_t const* last)
{
    prop_t const*   next    = curr + 1;
    uint64_t        result  = 0;

    while(next <= last)
    {
        uint64_t    _lo = curr->__time__;
        uint64_t    _hi = next->__time__;

        if(_lo < _hi)
        {
            if(boolean(curr))
            {
                uint64_t height = eval(curr);
                uint64_t length = (_hi - _lo);
                uint64_t volume = height * length;
                result  +=  volume;
            }
        }

        curr    = next;
        next    = next + 1;
    }

    return  result;
}
*/
    auto    bbHead  = llvm::BasicBlock::Create(*m_context, "I-head", m_function);

    auto    bbTime  = llvm::BasicBlock::Create(*m_context, "I-time", m_function);
    auto    bbCondHi= llvm::BasicBlock::Create(*m_context, "I-cond", m_function);
    auto    bbCondLo = bbCondHi;
    auto    bbBodyHi= llvm::BasicBlock::Create(*m_context, "I-body", m_function);
    auto    bbBodyLo= bbBodyHi;
    auto    bbNext  = llvm::BasicBlock::Create(*m_context, "I-next", m_function);
    auto    bbTail  = llvm::BasicBlock::Create(*m_context, "I-tail", m_function);

    auto    frst    = m_frst;
    auto    curr0   = m_curr.back();
    auto    next0   = m_builder->CreateGEP(m_propType, curr0, m_p1, "next");
    auto    last    = m_last;

    llvm::Value*    timeLo  = nullptr;
    llvm::Value*    timeHi  = nullptr;
    auto    now     = m_builder->CreateLoad(m_builder->getInt64Ty(), m_builder->CreateStructGEP(m_propType, curr0, 0), false, "now");

    if(expr->time && expr->time->lo)
    {
        timeLo  = m_builder->CreateAdd(now, make(expr->time->lo), "now + lo");
    }

    if(expr->time && expr->time->hi)
    {
        timeHi  = m_builder->CreateAdd(now, make(expr->time->hi), "now + hi");
    }

    auto    bbEntry = m_builder->GetInsertBlock();
    m_builder->CreateBr(bbHead);

    auto    type    = expr->rhs->type() == Factory<TypeInteger>::create()
                    ? m_builder->getInt64Ty()
                    : m_builder->getDoubleTy();
    auto    zero    = expr->rhs->type() == Factory<TypeInteger>::create()
                    ? llvm::ConstantInt::getSigned(m_builder->getInt64Ty(), 0)
                    : llvm::ConstantFP::get(m_builder->getDoubleTy(), 0.0);

    //  head
    m_builder->SetInsertPoint(bbHead);
    auto    sumHead = m_builder->CreatePHI(type, 2, "total@head");

    auto    curr    = m_builder->CreatePHI(frst->getType(), 2, "curr");
    auto    next    = m_builder->CreatePHI(frst->getType(), 2, "next");
    auto    cont    = m_builder->CreateICmpSLE(next, last, "next <= last");
    if(timeHi)
    {
        auto    currT   = m_builder->CreateLoad(m_builder->getInt64Ty(), m_builder->CreateStructGEP(m_propType, curr, 0), false, "curr->__time__");
        auto    timeHiLEcurrT
                        = m_builder->CreateICmpSGT(timeHi, currT, "timeHi > currT");
        cont    = m_builder->CreateAnd(cont, timeHiLEcurrT);
    }
    m_builder->CreateCondBr(cont, bbTime, bbTail);

    //  outer
    m_builder->SetInsertPoint(bbTime);

    auto    currT   = m_builder->CreateLoad(m_builder->getInt64Ty(), m_builder->CreateStructGEP(m_propType, curr, 0), false, "curr->__time__");
    auto    nextT   = m_builder->CreateLoad(m_builder->getInt64Ty(), m_builder->CreateStructGEP(m_propType, next, 0), false, "next->__time__");

    auto    lo      = timeLo 
                    ? m_builder->CreateSelect(m_builder->CreateICmpSLT(currT, timeLo), timeLo, currT)
                    : currT;
    auto    hi      = timeHi
                    ? m_builder->CreateSelect(m_builder->CreateICmpSLT(timeHi, nextT), timeHi, nextT)
                    : nextT;
    auto    loLThi  = m_builder->CreateICmpSLT(lo, hi);

    m_builder->CreateCondBr(loLThi, bbCondHi, bbNext);

    //  inner
    m_builder->SetInsertPoint(bbCondHi);
    m_curr.push_back(curr);
    auto    cond    = make(expr->lhs);
    m_curr.pop_back();
    bbCondLo       = m_builder->GetInsertBlock();
    m_builder->CreateCondBr(cond, bbBodyHi, bbNext);

    //  body
    m_builder->SetInsertPoint(bbBodyHi);
    m_curr.push_back(curr);
    auto    height  = make(expr->rhs);
    auto    length  = sub(hi, lo, "length");
    auto    volume  = mul(height, length, "volume");
    auto    sumBody = add(sumHead, volume, "total@body");
    m_curr.pop_back();

    bbBodyLo        = m_builder->GetInsertBlock();
    m_builder->CreateBr(bbNext);

    //  next
    m_builder->SetInsertPoint(bbNext);
    auto    sumNext = m_builder->CreatePHI(type, 3, "total@next");
    auto    curr1   = next;
    auto    next1   = m_builder->CreateGEP(m_propType, curr1, m_p1, "next");
    m_builder->CreateBr(bbHead);

    //  tail
    m_builder->SetInsertPoint(bbTail);

    auto    result  = m_builder->CreatePHI(type, 1, "value");

    //  link
    curr->addIncoming(curr0, bbEntry);
    curr->addIncoming(curr1, bbNext);

    next->addIncoming(next0, bbEntry);
    next->addIncoming(next1, bbNext);

    result->addIncoming(sumHead, bbHead);

    sumNext->addIncoming(sumBody, bbBodyLo);
    sumNext->addIncoming(sumHead, bbCondLo);
    sumNext->addIncoming(sumHead, bbTime);

    sumHead->addIncoming(zero,    bbEntry);
    sumHead->addIncoming(sumNext, bbNext);

    result->addIncoming( sumHead, bbHead);

    m_value = result;
}

void    CompileExprImpl::visit(ExprLe*           expr)
{
    compare(llvm::CmpInst::Predicate::ICMP_SLE, llvm::CmpInst::Predicate::FCMP_OLE, expr);
}

void    CompileExprImpl::visit(ExprLt*           expr)
{
    compare(llvm::CmpInst::Predicate::ICMP_SLT, llvm::CmpInst::Predicate::FCMP_OLT, expr);
}

void    CompileExprImpl::visit(ExprMmbr*         expr)
{
    auto    exprType    = expr->type();
    auto    basePtr     = make(expr->arg);
    auto    basePtrType = cast<llvm::PointerType>(basePtr->getType());
    auto    baseType    = basePtrType->getPointerElementType();

    if(dynamic_cast<TypeComposite*>(exprType) || dynamic_cast<TypeArray*>(exprType))
    {
        auto    temp        = dynamic_cast<TypeComposite*>(expr->arg->type());

        m_value = m_builder->CreateStructGEP(baseType, basePtr, temp->index(expr->mmbr), "ptr_" + expr->mmbr);
    }
    else if(auto type = dynamic_cast<TypePrimitive*>(exprType))
    {
        auto    temp        = dynamic_cast<TypeComposite*>(expr->arg->type());

        if(dynamic_cast<TypeEnum*>(expr->arg->type()))
        {
            auto    data        = m_builder->CreateLoad(baseType, basePtr, false, "val_" + expr->mmbr);
            m_value = m_builder->CreateICmp(llvm::CmpInst::Predicate::ICMP_EQ, data, llvm::ConstantInt::getSigned(baseType, temp->index(expr->mmbr)));
        }
        else
        {
            auto    dataPtr     = m_builder->CreateStructGEP(baseType, basePtr, temp->index(expr->mmbr), "ptr_" + expr->mmbr);
            auto    dataPtrType = cast<llvm::PointerType>(dataPtr->getType());
            auto    dataType    = dataPtrType->getPointerElementType();
            
            m_value = m_builder->CreateLoad(dataType, dataPtr, false, "val_" + expr->mmbr);
        }
    }
    else
    {
        throw std::runtime_error(__PRETTY_FUNCTION__);    
    }
}

void    CompileExprImpl::visit(ExprMod*          expr)
{
}

void    CompileExprImpl::visit(ExprMul*          expr)
{
    arithmetic(
        expr, 
        [this](llvm::Value* lhs, llvm::Value* rhs) {return m_builder->CreateMul(lhs, rhs);},
        [this](llvm::Value* lhs, llvm::Value* rhs) {return m_builder->CreateFMul(lhs, rhs);});
}

void    CompileExprImpl::visit(ExprNe*           expr)
{
    compare(llvm::CmpInst::Predicate::ICMP_NE, llvm::CmpInst::Predicate::FCMP_ONE, expr);
}

void    CompileExprImpl::visit(ExprNeg*          expr)
{
}

void    CompileExprImpl::visit(ExprNot*          expr)
{
    auto    arg = make(expr->arg);
    m_value = m_builder->CreateNot(arg);
}

void    CompileExprImpl::visit(ExprO*            expr)
{
}

void    CompileExprImpl::visit(ExprOr*           expr)
{
    auto    lhs = make(expr->lhs);
    auto    rhs = make(expr->rhs);
    m_value = m_builder->CreateLogicalOr(lhs, rhs);
}

void    CompileExprImpl::visit(ExprParen*        expr)
{
    m_value = make(expr->arg);
}

void    CompileExprImpl::visit(ExprRs*           expr)
{
    UR(expr, m_F, m_T, m_F, "Rs");
}

void    CompileExprImpl::visit(ExprRw*           expr)
{
    UR(expr, m_F, m_T, m_T, "Rw");
}

void    CompileExprImpl::visit(ExprSs*           expr)
{
    ST(expr, m_T, m_F, m_F, "Ss");
}

void    CompileExprImpl::visit(ExprSub*          expr)
{
    arithmetic(
        expr, 
        [this](llvm::Value* lhs, llvm::Value* rhs) {return m_builder->CreateSub(lhs, rhs);},
        [this](llvm::Value* lhs, llvm::Value* rhs) {return m_builder->CreateFSub(lhs, rhs);});
}

void    CompileExprImpl::visit(ExprSw*           expr)
{
    ST(expr, m_T, m_F, m_T, "Sw");
}

void    CompileExprImpl::visit(ExprTs*           expr)
{
    ST(expr, m_F, m_T, m_F, "Ts");
}

void    CompileExprImpl::visit(ExprTw*           expr)
{
    ST(expr, m_F, m_T, m_T, "Tw");
}

void    CompileExprImpl::visit(ExprUs*           expr)
{
    UR(expr, m_T, m_F, m_F, "Us");
}

void    CompileExprImpl::visit(ExprUw*           expr)
{
    UR(expr, m_T, m_F, m_T, "Uw");
}

void    CompileExprImpl::visit( ExprXor*        expr)
{
    auto    lhs = make(expr->lhs);
    auto    rhs = make(expr->rhs);
    m_value = m_builder->CreateXor(lhs, rhs);   //  TODO: fix
}

void    CompileExprImpl::XY(ExprBinary*     expr,
                            llvm::Value*    direction,
                            llvm::Value*    endV,
                            std::string     name)
{
    auto    bbHeadHi= llvm::BasicBlock::Create(*m_context, name + "-head", m_function);
    auto    bbHeadLo= bbHeadHi;
    auto    bbBodyHi= llvm::BasicBlock::Create(*m_context, name + "-body", m_function);
    auto    bbBodyLo= bbBodyHi;
    auto    bbTail  = llvm::BasicBlock::Create(*m_context, name + "-tail", m_function);
    auto    frst    = m_frst;
    auto    curr    = m_curr.back();
    auto    last    = m_last;

    m_builder->CreateBr(bbHeadHi);

    //  head
    m_builder->SetInsertPoint(bbHeadHi);
    auto    incr        = m_builder->CreateMul(make(expr->lhs), direction, "incr");
    auto    next        = m_builder->CreateGEP(m_propType, curr, incr, "next");
    auto    nextGElast  = m_builder->CreateICmpUGE(next, last, "next >= last");
    auto    nextLEfrst  = m_builder->CreateICmpULE(next, frst, "next <= frst");
    auto    outside     = m_builder->CreateOr(nextGElast, nextLEfrst);
    bbHeadLo            = m_builder->GetInsertBlock();
    m_builder->CreateCondBr(outside, bbTail, bbBodyHi);

    //  body
    m_builder->SetInsertPoint(bbBodyHi);
    m_curr.push_back(next);
    auto    body        = make(expr->rhs);
    m_curr.pop_back();
    bbBodyLo            = m_builder->GetInsertBlock();
    m_builder->CreateBr(bbTail);

    //  tail
    m_builder->SetInsertPoint(bbTail);
    auto    result      = m_builder->CreatePHI(m_builder->getInt1Ty(), 2, name);

    //  link
    result->addIncoming(body, bbBodyLo);
    result->addIncoming(endV, bbHeadLo);

    m_value = result;    
}

void    CompileExprImpl::UR(Temporal<ExprBinary>*   expr,
                            llvm::Value*            rhsV,
                            llvm::Value*            lhsV,
                            llvm::Value*            endV,
                            std::string             name)
{
/*
typedef long long uint64_t;
typedef struct prop_t
{
    uint64_t    __time__;
    uint64_t    a;
} prop_t;

bool        boolean(prop_t const* curr);
bool        integer(prop_t const* curr);

uint64_t    UR(uint64_t lo, uint64_t hi, prop_t const* curr, prop_t const* frst, prop_t const* last, bool lhsV, bool rhsV, bool endV)
{
    prop_t const*   next    = curr + 1;

    lo  = lo + curr->__time__;
    hi  = hi + curr->__time__;

    while(next <= last)
    {
        if(hi <= curr->__time__)
        {
            break;
        }

        uint64_t    _lo = curr->__time__ < lo ? lo : curr->__time__;
        uint64_t    _hi = hi < next->__time__ ? hi : next->__time__;

        if(_lo < _hi)
        {
            if(eval(rhs) == rhsV)
                return rshV;

            if(eval(lhs) == lhsV)
                return lshV;   
        }

        curr    = next;
        next    = next + 1;
    }

    return  endV;
}

uint64_t    UR(prop_t const* curr, prop_t const* frst, prop_t const* last, bool lhsV, bool rhsV, bool endV)
{
    prop_t const*   next    = curr + 1;
    uint64_t        result  = 0;

    while(next <= last)
    {
        uint64_t    _lo = curr->__time__;
        uint64_t    _hi = next->__time__;

        if(_lo < _hi)
        {
            if(eval(rhs) == rhsV)
                return rshV;

            if(eval(lhs) == lhsV)
                return lshV;  
        }

        curr    = next;
        next    = next + 1;
    }

    return  endV;
}
*/
    auto    debug   = m_module->getFunction("debug");

    auto    bbWhile = llvm::BasicBlock::Create(*m_context, name + "-while", m_function);
    auto    bbOuter = llvm::BasicBlock::Create(*m_context, name + "-outer", m_function);
    auto    bbInner = llvm::BasicBlock::Create(*m_context, name + "-inner", m_function);
    auto    bbLhsHi = llvm::BasicBlock::Create(*m_context, name + "-body-lhs", m_function);
    auto    bbLhsLo = bbLhsHi;
    auto    bbRhsHi = llvm::BasicBlock::Create(*m_context, name + "-body-rhs", m_function);
    auto    bbRhsLo = bbRhsHi;
    auto    bbNext  = llvm::BasicBlock::Create(*m_context, name + "-next", m_function);
    auto    bbTail  = llvm::BasicBlock::Create(*m_context, name + "-tail", m_function);

    auto    frst    = m_frst;
    auto    curr0   = m_curr.back();
    auto    next0   = m_builder->CreateGEP(m_propType, curr0, m_p1, "next");
    auto    last    = m_last;

    llvm::Value*    timeLo  = nullptr;
    llvm::Value*    timeHi  = nullptr;
    auto    now     = m_builder->CreateLoad(m_builder->getInt64Ty(), m_builder->CreateStructGEP(m_propType, curr0, 0), false, "now");

    if(expr->time && expr->time->lo)
    {
        timeLo  = m_builder->CreateAdd(now, make(expr->time->lo), "now + lo");
    }

    if(expr->time && expr->time->hi)
    {
        timeHi  = m_builder->CreateAdd(now, make(expr->time->hi), "now + hi");
    }
    auto    bbEntry = m_builder->GetInsertBlock();
    m_builder->CreateBr(bbWhile);

    //  while
    m_builder->SetInsertPoint(bbWhile);
    auto    curr    = m_builder->CreatePHI(frst->getType(), 2, "curr");
    auto    next    = m_builder->CreatePHI(frst->getType(), 2, "next");
    auto    nextLElast    = m_builder->CreateICmpSLE(next, last, "next <= last");
    m_builder->CreateCondBr(nextLElast, bbOuter, bbTail);

    //  outer
    m_builder->SetInsertPoint(bbOuter);
    if(timeHi)
    {
        auto    currT           = m_builder->CreateLoad(m_builder->getInt64Ty(), m_builder->CreateStructGEP(m_propType, curr, 0), false, "curr->__time__");
        auto    timeHiLTcurrT   = m_builder->CreateICmpSGT(timeHi, currT, "timeHi > currT");
        m_builder->CreateCondBr(timeHiLTcurrT, bbInner, bbTail);
    }
    else if(timeLo)
    {
        m_builder->CreateBr(bbInner);
    }
    else
    {
        m_builder->CreateBr(bbRhsHi);
    }

    //  inner
    m_builder->SetInsertPoint(bbInner);
    auto    currT   = m_builder->CreateLoad(m_builder->getInt64Ty(), m_builder->CreateStructGEP(m_propType, curr, 0), false, "curr->__time__");
    auto    nextT   = m_builder->CreateLoad(m_builder->getInt64Ty(), m_builder->CreateStructGEP(m_propType, next, 0), false, "next->__time__");

    auto    lo      = timeLo 
                    ? m_builder->CreateSelect(m_builder->CreateICmpSLT(currT, timeLo), timeLo, currT)
                    : currT;
    auto    hi      = timeHi
                    ? m_builder->CreateSelect(m_builder->CreateICmpSLT(timeHi, nextT), timeHi, nextT)
                    : nextT;
    auto    loLThi  = m_builder->CreateICmpSLT(lo, hi);

    m_builder->CreateCondBr(loLThi, bbRhsHi, bbNext);


    //  rhs
    m_builder->SetInsertPoint(bbRhsHi);
    m_curr.push_back(curr);
    auto    rhs     = make(expr->rhs);
    m_curr.pop_back();
    auto    rhsCond = m_builder->CreateCmp(llvm::CmpInst::Predicate::ICMP_EQ, rhs, rhsV);
    m_builder->CreateCondBr(rhsCond, bbTail, bbLhsHi);
    bbRhsLo = m_builder->GetInsertBlock();

    //  lhs
    m_builder->SetInsertPoint(bbLhsHi);
    m_curr.push_back(curr);
    auto    lhs     = make(expr->lhs);
    m_curr.pop_back();
    auto    lhsCond = m_builder->CreateCmp(llvm::CmpInst::Predicate::ICMP_EQ, lhs, lhsV);
    m_builder->CreateCondBr(rhsCond, bbTail, bbNext);
    bbLhsLo = m_builder->GetInsertBlock();

    //  next
    m_builder->SetInsertPoint(bbNext);
    auto    curr1   = next;
    auto    next1   = m_builder->CreateGEP(m_propType, curr1, m_p1, "next");
    m_builder->CreateBr(bbWhile);

    //  tail
    m_builder->SetInsertPoint(bbTail);
    auto    result  = m_builder->CreatePHI(m_builder->getInt1Ty(), 4, "result");

    //  link
    result->addIncoming(lhsV, bbLhsLo);
    result->addIncoming(rhsV, bbRhsLo);
    result->addIncoming(endV, bbWhile);
    result->addIncoming(endV, bbOuter);

    curr->addIncoming(curr0, bbEntry);
    curr->addIncoming(curr1, bbNext);

    next->addIncoming(next0, bbEntry);
    next->addIncoming(next1, bbNext);

    m_value = result;
}

void    CompileExprImpl::ST(Temporal<ExprBinary>*   expr,
                            llvm::Value*            rhsV,
                            llvm::Value*            lhsV,
                            llvm::Value*            endV,
                            std::string             name)
{
/*
typedef long long uint64_t;
typedef struct prop_t
{
    uint64_t    __time__;
    uint64_t    a;
} prop_t;

bool        boolean(prop_t const* curr);
bool        integer(prop_t const* curr);

uint64_t    ST(uint64_t lo, uint64_t hi, prop_t const* curr, prop_t const* frst, prop_t const* last, bool lhsV, bool rhsV, bool endV)
{
    prop_t const*   prev    = curr - 1;

    lo  = curr->__time__ - lo;
    hi  = curr->__time__ - hi;
    swap(lo, hi);

    while(frst <= prev)
    {
        ifcurr->__time__ < lo)
        {
            break;
        }

        uint64_t    _lo = lo > curr->__time__ ? lo : curr->__time__;
        uint64_t    _hi = prev->__time__ < hi ? hi : prev->__time__;

        if(_lo < _hi)
        {
            if(eval(rhs) == rhsV)
                return rshV;

            if(eval(lhs) == lhsV)
                return lshV;   
        }

        curr    = prev;
        prev    = prev - 1;
    }

    return  endV;
}

uint64_t    ST(prop_t const* curr, prop_t const* frst, prop_t const* last, bool lhsV, bool rhsV, bool endV)
{
    prop_t const*   prev    = curr - 1;
    uint64_t        result  = 0;

    while(frst <= prev)
    {
        uint64_t    _hi = curr->__time__;
        uint64_t    _lo = prev->__time__;

        if(_lo < _hi)
        {
            if(eval(rhs) == rhsV)
                return rshV;

            if(eval(lhs) == lhsV)
                return lshV;  
        }

        curr    = prev;
        prev    = prev + 1;
    }

    return  endV;
}
*/
    auto    debug   = m_module->getFunction("debug");

    auto    bbWhile = llvm::BasicBlock::Create(*m_context, name + "-while", m_function);
    auto    bbOuter = llvm::BasicBlock::Create(*m_context, name + "-outer", m_function);
    auto    bbInner = llvm::BasicBlock::Create(*m_context, name + "-inner", m_function);
    auto    bbRhsHi = llvm::BasicBlock::Create(*m_context, name + "-body-rhs", m_function);
    auto    bbRhsLo = bbRhsHi;
    auto    bbLhsHi = llvm::BasicBlock::Create(*m_context, name + "-body-lhs", m_function);
    auto    bbLhsLo = bbLhsHi;
    auto    bbPrev  = llvm::BasicBlock::Create(*m_context, name + "-prev", m_function);
    auto    bbTail  = llvm::BasicBlock::Create(*m_context, name + "-tail", m_function);

    auto    frst    = m_frst;
    auto    curr0   = m_curr.back();
    auto    prev0   = m_builder->CreateGEP(m_propType, curr0, m_m1, "prev");
    auto    last    = m_last;

    llvm::Value*    timeLo  = nullptr;
    llvm::Value*    timeHi  = nullptr;
    auto    now     = m_builder->CreateLoad(m_builder->getInt64Ty(), m_builder->CreateStructGEP(m_propType, curr0, 0), false, "now");

    if(expr->time && expr->time->lo)
    {
        timeHi  = m_builder->CreateSub(now, make(expr->time->lo), "now - lo");
    }

    if(expr->time && expr->time->hi)
    {
        timeLo  = m_builder->CreateSub(now, make(expr->time->hi), "now - hi");
    }
    auto    bbEntry = m_builder->GetInsertBlock();
    m_builder->CreateBr(bbWhile);

    //  while
    m_builder->SetInsertPoint(bbWhile);
    auto    curr    = m_builder->CreatePHI(frst->getType(), 2, "curr");
    auto    prev    = m_builder->CreatePHI(frst->getType(), 2, "prev");
    auto    frstLEprev    = m_builder->CreateICmpSLE(frst, prev, "frst <= prev");
    m_builder->CreateCondBr(frstLEprev, bbOuter, bbTail);

    //  outer
    m_builder->SetInsertPoint(bbOuter);
    if(timeHi)
    {
        auto    currT           = m_builder->CreateLoad(m_builder->getInt64Ty(), m_builder->CreateStructGEP(m_propType, curr, 0), false, "curr->__time__");
        auto    timeLoLEcurrT   = m_builder->CreateICmpSLE(timeLo, currT, "timeLo <= currT");
        m_builder->CreateCondBr(timeLoLEcurrT, bbInner, bbTail);
    }
    else if(timeLo)
    {
        m_builder->CreateBr(bbInner);
    }
    else
    {
        m_builder->CreateBr(bbRhsHi);
    }

    //  inner
    m_builder->SetInsertPoint(bbInner);
    auto    currT   = m_builder->CreateLoad(m_builder->getInt64Ty(), m_builder->CreateStructGEP(m_propType, curr, 0), false, "curr->__time__");
    auto    prevT   = m_builder->CreateLoad(m_builder->getInt64Ty(), m_builder->CreateStructGEP(m_propType, prev, 0), false, "prev->__time__");

    auto    lo      = timeHi
                    ? m_builder->CreateSelect(m_builder->CreateICmpSLT(prevT, timeLo), timeLo, prevT)
                    : prevT;
    auto    hi      = timeLo 
                    ? m_builder->CreateSelect(m_builder->CreateICmpSLT(timeHi, currT), timeHi, currT)
                    : currT;

    auto    loLThi  = m_builder->CreateICmpSLT(lo, hi);

    m_builder->CreateCondBr(loLThi, bbRhsHi, bbPrev);


    //  rhs
    m_builder->SetInsertPoint(bbRhsHi);
    m_curr.push_back(curr);
    auto    rhs     = make(expr->rhs);
    m_curr.pop_back();
    auto    rhsCond = m_builder->CreateCmp(llvm::CmpInst::Predicate::ICMP_EQ, rhs, rhsV);
    m_builder->CreateCondBr(rhsCond, bbTail, bbLhsHi);
    bbRhsLo = m_builder->GetInsertBlock();

    //  lhs
    m_builder->SetInsertPoint(bbLhsHi);
    m_curr.push_back(curr);
    auto    lhs     = make(expr->lhs);
    m_curr.pop_back();
    auto    lhsCond = m_builder->CreateCmp(llvm::CmpInst::Predicate::ICMP_EQ, lhs, lhsV);
    m_builder->CreateCondBr(rhsCond, bbTail, bbPrev);
    bbLhsLo = m_builder->GetInsertBlock();

    //  next
    m_builder->SetInsertPoint(bbPrev);
    auto    curr1   = prev;
    auto    prev1   = m_builder->CreateGEP(m_propType, curr1, m_m1, "prev");
    m_builder->CreateBr(bbWhile);

    //  tail
    m_builder->SetInsertPoint(bbTail);
    auto    result  = m_builder->CreatePHI(m_builder->getInt1Ty(), 4, "result");

    //  link
    result->addIncoming(lhsV, bbLhsLo);
    result->addIncoming(rhsV, bbRhsLo);
    result->addIncoming(endV, bbWhile);
    result->addIncoming(endV, bbOuter);

    curr->addIncoming(curr0, bbEntry);
    curr->addIncoming(curr1, bbPrev);

    prev->addIncoming(prev0, bbEntry);
    prev->addIncoming(prev1, bbPrev);

    m_value = result;
}

void    CompileExprImpl::visit( ExprXs*         expr)
{
    XY(expr, m_p1, m_F, "Xs");
}

void    CompileExprImpl::visit(ExprXw*           expr)
{
    XY(expr, m_p1, m_T, "Xw");
}

void    CompileExprImpl::visit(ExprYs*           expr)
{
    XY(expr, m_m1, m_F, "Ys");
}

void    CompileExprImpl::visit(ExprYw*           expr)
{
    XY(expr, m_m1, m_T, "Yw");
}

llvm::Value*    CompileExprImpl::make(Expr* expr)
{
    auto    save    = m_value;

    m_value = llvm::ConstantInt::getTrue(*m_context);

    expr->accept(*this);

    auto    result  = m_value;
    m_value = save;
    
    return  result;
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

void Compile::make(llvm::LLVMContext* context, llvm::Module* module, Module* refmod)
{
    auto    builder = std::make_unique<llvm::IRBuilder<>>(*context);

    //  create __conf__
    std::vector<llvm::Type*>    confTypes;
    auto    confNames   = refmod->getConfNames();
    for(auto name: confNames)
    {
        auto    type    = refmod->getConf(name);
        confTypes.push_back(llvm::PointerType::get(make(context, module, type, name), 0));
    }
    auto    confType    = llvm::StructType::create(*context, confTypes, "__conf_t");
    auto    confPtrType = llvm::PointerType::get(confType, 0);
    module->getOrInsertGlobal("__conf__", confType);

    //  create __prop__
    auto    propNames   = refmod->getPropNames();
    std::vector<llvm::Type*>    propTypes;
    propTypes.push_back(builder->getInt64Ty()); //  __time__
    for(auto name: propNames)
    {
        if(name == "__time__")
            continue;
            
        std::cout << "prop: " << name << std::endl;
        auto    type    = refmod->getProp(name);
        propTypes.push_back(llvm::PointerType::get(make(context, module, type, name), 0));
    }
    auto    propType    = llvm::StructType::create(*context, propTypes, "__prop_t");
    auto    propPtrType = llvm::PointerType::get(propType, 0);
    module->getOrInsertGlobal("__prop__", propPtrType);

    auto    exprs   = refmod->getExprs();
    for(auto expr: exprs)
    {
        auto    pos         = expr->where();
        auto    funcName    = std::to_string(pos.beg.row) + ":" + std::to_string(pos.beg.col) + " .. " + std::to_string(pos.end.row) + ":" + std::to_string(pos.end.col);
        auto    funcType    = llvm::FunctionType::get(builder->getInt1Ty(), {propPtrType, propPtrType, confPtrType}, false);
        auto    funcBody    = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, funcName, module);
        auto    funcArgs    = funcBody->args().begin();

        funcArgs->setName("frst");  funcArgs++;
        funcArgs->setName("last");  funcArgs++;
        funcArgs->setName("conf");

        auto    bb          = llvm::BasicBlock::Create(*context, "entry", funcBody);
        builder->SetInsertPoint(bb);

        CompileExprImpl compExpr(context, module, builder.get(), funcBody, refmod);

        auto    temp        = Rewrite::make(expr);
        TypeCalc::make(refmod, temp);
        Printer::output(std::cout, temp) << std::endl;
        builder->CreateRet(compExpr.make(temp));

        llvm::verifyFunction(*funcBody);
    }
}
