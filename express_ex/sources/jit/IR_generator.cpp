
#include "llvm/ADT/STLExtras.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"
#include "IR_generator.h"
#include "table.h"

using namespace llvm;

IRGenerator::IRGenerator(LLVMContext & context,Table * table_):IRBuilder<>(context){
    table=table_;
}

IRGenerator::~IRGenerator(){
    for (auto i : buffer_list)
        delete i;
}

Value * IRGenerator::CreateFPow(Value *aOperand, Value *bOperand, const std::string &name) {
    if (table == NULL) 
        return NULL;

    if (aOperand->getType() == getFloatTy())
        return CreateCall(table->getFloatBIFunc(OpCodeEn::fpow),  { aOperand, bOperand }, name);
    else
        return CreateCall(table->getDoubleBIFunc(OpCodeEn::fpow), { aOperand, bOperand }, name);
}


Value * IRGenerator::CreateConst(uint64_t &binaryValue, TypeEn targetTy, const std::string &name) {
    Value * ret = NULL;

    auto &context = getContext();
    switch (targetTy)
    {
    case TypeEn::int1_jty:   ret = getInt1(*((bool    *)(&binaryValue)));   break;
    case TypeEn::int8_jty:   ret = getInt8(*((int8_t  *)(&binaryValue)));   break;
    case TypeEn::int16_jty:  ret = getInt16(*((int16_t*)(&binaryValue)));   break;
    case TypeEn::int32_jty:  ret = getInt32(*((int32_t*)(&binaryValue)));   break;
    case TypeEn::int64_jty:  ret = getInt64(*((int64_t*)(&binaryValue)));   break;
    case TypeEn::float_jty:  ret = ConstantFP::get(Type::getFloatTy(context), *((float  *)(&binaryValue)));  break;
    case TypeEn::double_jty: ret = ConstantFP::get(Type::getDoubleTy(context), *((double *)(&binaryValue))); break;
    case TypeEn::unknown_jty:ret = NULL;                                    break;
    default:                 ret = NULL;                                    break;
    }

    return ret;
}

Value * IRGenerator::CreateArithmetic(Value *aOperand, Value *bOperand, OpCodeEn opCode, const std::string &name) {
    Value * ret = NULL;
    switch (opCode)
    {
    case OpCodeEn::add:  ret = CreateAdd(aOperand, bOperand, name);  break;
    case OpCodeEn::sub:  ret = CreateSub(aOperand, bOperand, name);  break;
    case OpCodeEn::mul:  ret = CreateMul(aOperand, bOperand, name);  break;
    case OpCodeEn::sdiv: ret = CreateSDiv(aOperand, bOperand, name); break;
    case OpCodeEn::srem: ret = CreateSRem(aOperand, bOperand, name); break;
    case OpCodeEn::pow:  ret = NULL; break;

    case OpCodeEn::fadd: ret = CreateFAdd(aOperand, bOperand, name); break;
    case OpCodeEn::fsub: ret = CreateFSub(aOperand, bOperand, name); break;
    case OpCodeEn::fmul: ret = CreateFMul(aOperand, bOperand, name); break;
    case OpCodeEn::fdiv: ret = CreateFDiv(aOperand, bOperand, name); break;
    case OpCodeEn::frem: ret = CreateFRem(aOperand, bOperand, name); break;
    case OpCodeEn::fpow: ret = CreateFPow(aOperand, bOperand, name); break;
    }
    return ret;
}


    //builder.CreateCall(UpdateBufferF, ConstVars[0]);
Value * IRGenerator::CreateInv(Value *aOperand, OpCodeEn opCode, const std::string &name) {
    Value * ret = NULL;

    if (opCode == OpCodeEn::neg)
        return CreateNeg(aOperand, name);
    else
        return CreateFNeg(aOperand,name);
}

Value * IRGenerator::CreateTypeConv(llvm::Value * aOperand, OpCodeEn opCode, TypeEn targetTy, const std::string &name)
{
    Type* destTy = getLLVMType(targetTy);

    Value * ret=NULL;
    switch (opCode)
    {
    case OpCodeEn::trunc:   ret = CreateTrunc(aOperand, destTy, name);    break;
    case OpCodeEn::fptrunc: ret = CreateFPTrunc(aOperand, destTy, name);  break;
    case OpCodeEn::uitofp:  ret = CreateUIToFP(aOperand, destTy, name);   break;
    case OpCodeEn::sitofp:  ret = CreateSIToFP(aOperand, destTy, name);   break;
    case OpCodeEn::fptoi:   ret = CreateFPToUI(aOperand, destTy, name);   break;
    case OpCodeEn::fptosi:  ret = CreateFPToSI(aOperand, destTy, name);   break;
    case OpCodeEn::sext:    ret = CreateSExt(aOperand, destTy, name);     break;
    case OpCodeEn::zext:    ret = CreateZExt(aOperand, destTy, name);     break;
    case OpCodeEn::fpext:   ret = CreateFPExt(aOperand, destTy, name);    break;
    }
    return ret;
}

Value * IRGenerator::CreateBuiltInFunc(llvm::Value * aOperand, OpCodeEn opCode, const std::string &name)
{
    if (table == NULL) return NULL;
    if (aOperand->getType() == getFloatTy()) {
        return CreateCall(table->getFloatBIFunc(opCode), { aOperand }, name);
    }
    else {
        return CreateCall(table->getDoubleBIFunc(opCode), { aOperand }, name);
    }
}

Value * IRGenerator::CreatePositionalAlloca(llvm::Type * ty,unsigned int i, const std::string &name)
{
    SetInitInsertPoint();
    Value* ret = CreateAlloca(ty, 0, name);
    SetCalcInsertPoint();
    return ret;
}

Value * IRGenerator::CreatePositionalOffset( std::string name,uint64_t startValue)
{
    Value* ret=CreatePositionalOffsetAlloca();
    SetLoadInsertPoint();
    currentOffsetValue=CreateLoad(ret, "offset" );
    SetCalcInsertPoint();
    return ret;
}


Value * IRGenerator::CreatePositionalOffsetAlloca(std::string name, uint64_t startValue)
{
    auto ty=getInt64Ty();
    SetInitInsertPoint();
    Value* ret = CreateAlloca(ty, 0, "offset_alloca_");
    currentOffsetValueAlloca=ret;
    CreateStore(getInt64(startValue), ret);
    return ret;
}

llvm::Value * IRGenerator::CreatePositionalInBoundsGEP(llvm::Value * Ptr, llvm::ArrayRef<llvm::Value*> IdxList, const std::string & name)
{
    SetLoadInsertPoint();
    Value* ret = CreateInBoundsGEP(Ptr, IdxList, name);
    SetCalcInsertPoint();
    return ret;
}

Value * IRGenerator::CreatePositionalLoad(llvm::Value * aOperand, const std::string &name)
{
    SetLoadInsertPoint();
    Value* ret = CreateLoad(aOperand, name);
    SetCalcInsertPoint();
    return ret;
}

Value * IRGenerator::CreatePositionalLoad(llvm::Value * aOperand, bool isVolatile, const std::string &name)
{
    SetLoadInsertPoint();
    Value* ret = CreateLoad(aOperand, isVolatile, name);
    SetCalcInsertPoint();
    return ret;
}

llvm::Value * IRGenerator::CreateLoadOffset( const std::string & name)
{
    currentOffsetValue =CreateLoad(currentOffsetValueAlloca, name);
    return currentOffsetValue;
}

void    IRGenerator::CreatePositionalStore(llvm::Value * value, llvm::Value * ptr, bool isVolatile )
{
    SetStoreInsertPoint();
    CreateStore(value, ptr, isVolatile);
    SetCalcInsertPoint();
}

Value * IRGenerator::CreateBufferInit(TypeEn targetTy, const std::string &name)
{
    int numberOfBuffer = buffersList.size();
    std::string numberOfBufferTxt=std::to_string(numberOfBuffer);

    SetInitInsertPoint();
    Value* arg              = currentFunction->getArg(0);

    Value* untypedBufferPtr =CreateInBoundsGEP(
        getInt64Ty()->getPointerTo(),
        arg,
        getInt32(numberOfBuffer),
        name + "untyped_buffer_ptr_"+ numberOfBufferTxt);

    Value* untypedBuffer    = CreateLoad(
        untypedBufferPtr,
        true,
        name+"untyped_buffer_"+ numberOfBufferTxt);


    Value* buffer           = CreateBitCast(
        untypedBuffer,
        getLLVMType(targetTy)->getPointerTo(),
        name + "buffer_"+ numberOfBufferTxt);
    
    buffersList.push_back(buffer);

    SetCalcInsertPoint();
    return buffer;
}

void IRGenerator::CreateStartBRs()
{
    SetLoadInsertPoint();
    CreateBr(getCalcBlock());
    SetCalcInsertPoint();
    CreateBr(getStoreBlock());
    //SetStoreInsertPoint();
    SetLastInsertPoint();
}

void IRGenerator::CreateMidleBRs()
{

    SetIntermediateInsertPoint();
    CreateBr(getLoadBlock());
    SetLoadInsertPoint();
    CreateBr(getCalcBlock());
    SetCalcInsertPoint();
    CreateBr(getStoreBlock());
    //SetStoreInsertPoint();
    SetLastInsertPoint();
}





void * IRGenerator::AddBufferAlloca(Buffer *s)
{
    buffer_list.push_back(s);
    return NULL;
}


Value * IRGenerator::CreateConvolve(Value * aOperand, Value * bOperand, const std::string &name)
{
    Value * ret =NULL,* convolveFunction=NULL;
    Type * type=aOperand->getType()->getPointerElementType();

    if (type == getDoubleTy())
        convolveFunction=convolveDoubleFunction;
    else if (type == getFloatTy())
        convolveFunction=convolveFloatFunction;
    else if (type == getInt64Ty())
        convolveFunction=convolveI64Function;
    else if (type == getInt32Ty())
        convolveFunction=convolveI32Function;
    else
        return ret;
    //if (ret == NULL) print_error("CreateConvolve :" );
    return CreateCall(convolveFunction, { aOperand ,bOperand }, name);
}



void IRGenerator::SetDeclareConvolve(llvm::Type * type, uintptr_t addr)
{
    typedef Value * (*lambla_ty)(llvm::Type * type, uintptr_t addr);

    auto  lambda =[this](llvm::Type * type, uintptr_t addr) {
        return CreateIntToPtr(
            ConstantInt::get(getInt64Ty(), uintptr_t(addr)),
            PointerType::getUnqual(
                FunctionType::get(type,
                    { type->getPointerTo(), type->getPointerTo() },
                    false)));
    };

    if (type == getDoubleTy())
        convolveDoubleFunction=lambda(type, addr);
    else if (type == getFloatTy())
        convolveFloatFunction=lambda(type, addr);
    else if (type == getInt64Ty())
        convolveI64Function=lambda(type, addr);
    else if (type == getInt32Ty())
        convolveI32Function=lambda(type, addr);
    
}

void IRGenerator::SetOffsetToZero()
{   
    BasicBlock*  tmp=BB;
    CreateStore(getInt64(0), currentOffsetValueAlloca);

}

llvm::Type * IRGenerator::getLLVMType(TypeEn targetTy) {
    Type * ret = NULL;
    //this->SetInsertPoint
    switch (targetTy)
    {
    case TypeEn::int1_jty:   ret = getInt1Ty();   break;
    case TypeEn::int8_jty:   ret = getInt8Ty();   break;
    case TypeEn::int16_jty:  ret = getInt16Ty();  break;
    case TypeEn::int32_jty:  ret = getInt32Ty();  break;
    case TypeEn::int64_jty:  ret = getInt64Ty();  break;
    case TypeEn::float_jty:  ret = getFloatTy();  break;
    case TypeEn::double_jty: ret = getDoubleTy(); break;
    case TypeEn::unknown_jty:ret = NULL;          break;
    default:                 ret = NULL;          break;
    }
    
    return ret;
}