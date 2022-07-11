

#include "jit/IR_generator.h"

#include "jit/table.h"

// using namespace llvm;

IRGenerator::IRGenerator(llvm::LLVMContext& context, Table* table, bool is_pure_function)
    : IRBuilder<>(context), is_pure_function_(is_pure_function), table_(table) {
    nextBufferGroup();
}

IRGenerator::~IRGenerator() {
    for (auto buffer_group : buffer_list_) {
        for (auto i : *buffer_group) delete i;
        delete buffer_group;
    }
}

llvm::Value* IRGenerator::createFPow(llvm::Value* a_operand, llvm::Value* b_operand, const std::string& name) {
    if (table_ == nullptr) return nullptr;

    if (a_operand->getType() == getFloatTy())
        return CreateCall(table_->getFloatBIFunc(OpCodeEn::fpow), {a_operand, b_operand}, name);
    else return CreateCall(table_->getDoubleBIFunc(OpCodeEn::fpow), {a_operand, b_operand}, name);
}

llvm::Value* IRGenerator::createConst(int64_t& binary_value, TypeEn target_ty, const std::string& name) {
    llvm::Value* ret = nullptr;

    auto& context = getContext();
    switch (target_ty) {
    case TypeEn::int1_jty:
        ret = getInt1(*((bool*)(&binary_value)));
        break;
    case TypeEn::int8_jty:
        ret = getInt8(*((int8_t*)(&binary_value)));
        break;
    case TypeEn::int16_jty:
        ret = getInt16(*((int16_t*)(&binary_value)));
        break;
    case TypeEn::int32_jty:
        ret = getInt32(*((int32_t*)(&binary_value)));
        break;
    case TypeEn::int64_jty:
        ret = getInt64(*((int64_t*)(&binary_value)));
        break;
    case TypeEn::float_jty:
        ret = llvm::ConstantFP::get(llvm::Type::getFloatTy(context), *((float*)(&binary_value)));
        break;
    case TypeEn::double_jty:
        ret = llvm::ConstantFP::get(llvm::Type::getDoubleTy(context), *((double*)(&binary_value)));
        break;
    case TypeEn::unknown_jty:
        ret = nullptr;
        break;
    default:
        ret = nullptr;
        break;
    }

    return ret;
}

llvm::Value* IRGenerator::createArithmetic(llvm::Value* a_operand, llvm::Value* b_operand, OpCodeEn op_code,
                                           const std::string& name) {
    llvm::Value* ret = nullptr;
    switch (op_code) {
    case OpCodeEn::add:
        ret = CreateAdd(a_operand, b_operand, name);
        break;
    case OpCodeEn::sub:
        ret = CreateSub(a_operand, b_operand, name);
        break;
    case OpCodeEn::mul:
        ret = CreateMul(a_operand, b_operand, name);
        break;
    case OpCodeEn::sdiv:
        ret = CreateSDiv(a_operand, b_operand, name);
        break;
    case OpCodeEn::srem:
        ret = CreateSRem(a_operand, b_operand, name);
        break;
    case OpCodeEn::pow:
        ret = nullptr;
        break;

    case OpCodeEn::fadd:
        ret = CreateFAdd(a_operand, b_operand, name);
        break;
    case OpCodeEn::fsub:
        ret = CreateFSub(a_operand, b_operand, name);
        break;
    case OpCodeEn::fmul:
        ret = CreateFMul(a_operand, b_operand, name);
        break;
    case OpCodeEn::fdiv:
        ret = CreateFDiv(a_operand, b_operand, name);
        break;
    case OpCodeEn::frem:
        ret = CreateFRem(a_operand, b_operand, name);
        break;
    case OpCodeEn::fpow:
        ret = createFPow(a_operand, b_operand, name);
        break;
    }
    return ret;
}
/*
sgt,//  signed greater than
sge,//  signed greater or equal
slt,//  signed less than
sle,//  signed less or equal
*/

llvm::Value* IRGenerator::createComparison(llvm::Value* a_operand, llvm::Value* b_operand, OpCodeEn op_code,
                                           const std::string& name) {
    llvm::Value* ret = nullptr;
    switch (op_code) {
    case OpCodeEn::eq:
        ret = CreateICmpEQ(a_operand, b_operand, name);
        break;
    case OpCodeEn::ne:
        ret = CreateICmpNE(a_operand, b_operand, name);
        break;
    case OpCodeEn::sgt:
        ret = CreateICmpSGT(a_operand, b_operand, name);
        break;
    case OpCodeEn::sge:
        ret = CreateICmpSGE(a_operand, b_operand, name);
        break;
    case OpCodeEn::slt:
        ret = CreateICmpSLT(a_operand, b_operand, name);
        break;
    case OpCodeEn::sle:
        ret = CreateICmpSLT(a_operand, b_operand, name);
        break;

    case OpCodeEn::oeq:
        ret = CreateFCmpOEQ(a_operand, b_operand, name);
        break;
    case OpCodeEn::one:
        ret = CreateFCmpONE(a_operand, b_operand, name);
        break;
    case OpCodeEn::ogt:
        ret = CreateFCmpOGT(a_operand, b_operand, name);
        break;
    case OpCodeEn::oge:
        ret = CreateFCmpOGE(a_operand, b_operand, name);
        break;
    case OpCodeEn::olt:
        ret = CreateFCmpOLT(a_operand, b_operand, name);
        break;
    case OpCodeEn::ole:
        ret = CreateFCmpOLE(a_operand, b_operand, name);
        break;
    }
    return ret;
}

// builder.CreateCall(UpdateBufferF, ConstVars[0]);
llvm::Value* IRGenerator::createInv(llvm::Value* a_operand, OpCodeEn op_code, const std::string& name) {
    llvm::Value* ret = nullptr;

    if (op_code == OpCodeEn::neg) return CreateNeg(a_operand, name);
    else return CreateFNeg(a_operand, name);
}

llvm::Value* IRGenerator::createTypeConv(llvm::Value* a_operand, OpCodeEn op_code, TypeEn target_ty,
                                         const std::string& name) {
    llvm::Type* dest_ty = getLLVMType(target_ty);

    llvm::Value* ret = nullptr;
    switch (op_code) {
    case OpCodeEn::trunc:
        ret = CreateTrunc(a_operand, dest_ty, name);
        break;
    case OpCodeEn::fptrunc:
        ret = CreateFPTrunc(a_operand, dest_ty, name);
        break;
    case OpCodeEn::uitofp:
        ret = CreateUIToFP(a_operand, dest_ty, name);
        break;
    case OpCodeEn::sitofp:
        ret = CreateSIToFP(a_operand, dest_ty, name);
        break;
    case OpCodeEn::fptoi:
        ret = CreateFPToUI(a_operand, dest_ty, name);
        break;
    case OpCodeEn::fptosi:
        ret = CreateFPToSI(a_operand, dest_ty, name);
        break;
    case OpCodeEn::sext:
        ret = CreateSExt(a_operand, dest_ty, name);
        break;
    case OpCodeEn::zext:
        ret = CreateZExt(a_operand, dest_ty, name);
        break;
    case OpCodeEn::fpext:
        ret = CreateFPExt(a_operand, dest_ty, name);
        break;
    default:
        ret = a_operand;
    }
    return ret;
}

llvm::Value* IRGenerator::createBuiltInFunc(llvm::Value* a_operand, OpCodeEn op_code, const std::string& name) {
    if (table_ == nullptr) return nullptr;
    if (a_operand->getType() == getFloatTy()) {
        return CreateCall(table_->getFloatBIFunc(op_code), {a_operand}, name);
    } else {
        return CreateCall(table_->getDoubleBIFunc(op_code), {a_operand}, name);
    }
}

llvm::Value* IRGenerator::createConvolve(llvm::Value* a_operand, char* ptr, int64_t length, int64_t shift, TypeEn type,
                                         const std::string& name) {
    auto function = table_->getConvolveFunc(type);
    auto bOperand = CreateIntToPtr(llvm::ConstantInt::get(getInt64Ty(), uintptr_t(ptr)),
                                   llvm::PointerType::getUnqual(getLLVMType(type)));

    return CreateCall(function, {a_operand, bOperand, getInt64(length), getInt64(shift)}, name);
}

llvm::Value* IRGenerator::createCall(llvm::Value* callee, llvm::ArrayRef<llvm::Value*> args, const std::string& name) {
    llvm::MDNode* FPMathTag = nullptr;

    return CreateCall(llvm::cast<llvm::FunctionType>(callee->getType()->getPointerElementType()), callee, args, name,
                      FPMathTag);
}

llvm::Value* IRGenerator::createConvolve(llvm::Value* a_operand, llvm::Value* b_operand, const std::string& name) {
    llvm::Value *ret = nullptr, *convolveFunction = nullptr;
    llvm::Type* type = a_operand->getType()->getPointerElementType();

    if (type == getDoubleTy()) convolveFunction = convolve_double_function_;
    else if (type == getFloatTy()) convolveFunction = convolve_float_function_;
    else if (type == getInt64Ty()) convolveFunction = convolve_i64_function_;
    else if (type == getInt32Ty()) convolveFunction = convolve_i32_function_;
    else return ret;
    // if (ret == nullptr) print_error("CreateConvolve :" );

    return createCall(convolveFunction, {a_operand, b_operand}, name);
}

llvm::Value* IRGenerator::createGpuConvolve(llvm::Value* a_operand, char* ptr, int64_t length, int64_t shift,
                                            TypeEn type, const std::string& name) {
    print_IR_error("CreateGPUConvolve is not supported yet");
    auto function = table_->getGPUConvolveFunc(type);
    return nullptr;
}

llvm::Value* IRGenerator::createPositionalAlloca(llvm::Type* ty, int64_t i, const std::string& name) {
    auto last_block = GetInsertBlock();
    setInitInsertPoint();
    llvm::Value* ret = CreateAlloca(ty, nullptr, name);
    SetInsertPoint(last_block);
    return ret;
}


llvm::Value* IRGenerator::createPositionalOffsetAlloca(const std::string& name, int64_t start_value) {
    auto ty = getInt64Ty();
    auto last_block = GetInsertBlock();
    setInitInsertPoint();
    current_offset_value_alloca_ = CreateAlloca(ty, 0, name);
    CreateStore(getInt64(start_value), current_offset_value_alloca_);
    SetInsertPoint(last_block);
    return current_offset_value_alloca_;
}

llvm::Value* IRGenerator::createPositionalInBoundsGep(llvm::Value* ptr, llvm::ArrayRef<llvm::Value*> idx_list,
                                                      const std::string& name) {
    auto last_block = GetInsertBlock();
    setLoadInsertPoint();
    llvm::Value* ret = CreateInBoundsGEP(ptr, idx_list, name);
    SetInsertPoint(last_block);
    return ret;
}

llvm::Value* IRGenerator::createPositionalLoad(llvm::Value* a_operand, const std::string& name) {
    auto last_block = GetInsertBlock();
    setLoadInsertPoint();
    llvm::Value* ret = CreateLoad(a_operand, name);
    SetInsertPoint(last_block);
    return ret;
}

llvm::Value* IRGenerator::createPositionalLoad(llvm::Value* a_operand, bool is_volatile, const std::string& name) {
    auto last_block = GetInsertBlock();
    setLoadInsertPoint();
    llvm::Value* ret = CreateLoad(a_operand, is_volatile, name);
    SetInsertPoint(last_block);
    return ret;
}

llvm::Value* IRGenerator::createLoadOffset(const std::string& name) {
    auto last_block = GetInsertBlock();
    setLoadInsertPoint();
    current_offset_value_ = CreateLoad(current_offset_value_alloca_, name);
    SetInsertPoint(last_block);
    return current_offset_value_;
}

void IRGenerator::createPositionalStore(llvm::Value* a_operand, llvm::Value* ptr, bool is_volatile) {
    auto last_block = GetInsertBlock();
    setStoreInsertPoint();
    CreateStore(a_operand, ptr, is_volatile);
    SetInsertPoint(last_block);
}

llvm::Value* IRGenerator::createBufferInit(TypeEn target_ty, const std::string& name) {
    int number_of_buffer = (int)buffers_List_.size();
    std::string number_of_buffer_txt = std::to_string(number_of_buffer);

    auto last_block = GetInsertBlock();
    setInitInsertPoint();
    llvm::Value* arg = current_function_->getArg(0);

    llvm::Value* untyped_buffer_ptr = CreateInBoundsGEP(getInt64Ty()->getPointerTo(), arg, getInt32(number_of_buffer),
                                                        name + "untyped_buffer_ptr_" + number_of_buffer_txt);
    llvm::Value* untyped_buffer = CreateLoad(untyped_buffer_ptr, true, name + "untyped_buffer_" + number_of_buffer_txt);
    llvm::Value* buffer =
        CreateBitCast(untyped_buffer, getLLVMType(target_ty)->getPointerTo(), name + "buffer_" + number_of_buffer_txt);

    buffers_List_.push_back(buffer);

    SetInsertPoint(last_block);
    return buffer;
}

void IRGenerator::createStartBRs() { createMiddleBRs(); }

void IRGenerator::createMiddleBRs() {
    setIntermediateInsertPoint();
    CreateBr(getLoadBlock());
    setLoadInsertPoint();
    CreateBr(getCalcBlock());
    setCalcInsertPoint();
    CreateBr(getStoreBlock());
    // SetStoreInsertPoint();
    setLastInsertPoint();
}

void* IRGenerator::addBuffer(Buffer* s) {
    buffer_list_.back()->push_back(s);
    return nullptr;
}

llvm::Value* IRGenerator::createLocalBuffer(TypeEn target_ty, size_t len, const std::string& name) {
    size_t size = sizeOfTy(target_ty) * len;
    auto local_buffer = new char[size];
    std::memset(local_buffer, 0, size);
    local_buffer_list_.push_back(local_buffer);


    auto last_block = setInitInsertPoint();

    llvm::Value* arg = current_function_->getArg(1);
    auto number_of_buffer_txt = std::to_string(local_buffer_list_.size());

    llvm::Value* untyped_buffer_ptr =
        CreateInBoundsGEP(getInt64Ty()->getPointerTo(), arg, getInt32(local_buffer_list_.size()),
                          name + "untyped_local_buffer_ptr_" + std::to_string(local_buffer_list_.size()));
    llvm::Value* untyped_buffer =
        CreateLoad(untyped_buffer_ptr, false, name + "untyped_local_buffer_" + number_of_buffer_txt);

    llvm::Value* buffer = CreateBitCast(untyped_buffer, getLLVMType(target_ty)->getPointerTo(),
                                        name + "local_buffer_" + number_of_buffer_txt);



    SetInsertPoint(last_block);
    return buffer;
}



void IRGenerator::setDeclareConvolve(llvm::Type* type, uintptr_t addr)  // atavism
{
    typedef llvm::Value* (*lambla_ty)(llvm::Type * type, uintptr_t addr);

    auto lambda = [this](llvm::Type* type, uintptr_t addr) {
        return CreateIntToPtr(llvm::ConstantInt::get(getInt64Ty(), uintptr_t(addr)),
                              llvm::PointerType::getUnqual(
                                  llvm::FunctionType::get(type, {type->getPointerTo(), type->getPointerTo()}, false)));
    };

    if (type == getDoubleTy()) convolve_double_function_ = lambda(type, addr);
    else if (type == getFloatTy()) convolve_float_function_ = lambda(type, addr);
    else if (type == getInt64Ty()) convolve_i64_function_ = lambda(type, addr);
    else if (type == getInt32Ty()) convolve_i32_function_ = lambda(type, addr);
}

void IRGenerator::setOffsetToZero() {
    llvm::BasicBlock* tmp = BB;
    CreateStore(getInt64(0), current_offset_value_alloca_);
}

void IRGenerator::setOffsetTo(int64_t val) {
    llvm::BasicBlock* tmp = BB;
    CreateStore(getInt64(val), current_offset_value_alloca_);
}

llvm::Type* IRGenerator::getLLVMType(TypeEn target_ty) {
    llvm::Type* ret = nullptr;
    // this->SetInsertPoint
    switch (target_ty) {
    case TypeEn::int1_jty:
        ret = getInt1Ty();
        break;
    case TypeEn::int8_jty:
        ret = getInt8Ty();
        break;
    case TypeEn::int16_jty:
        ret = getInt16Ty();
        break;
    case TypeEn::int32_jty:
        ret = getInt32Ty();
        break;
    case TypeEn::int64_jty:
        ret = getInt64Ty();
        break;
    case TypeEn::float_jty:
        ret = getFloatTy();
        break;
    case TypeEn::double_jty:
        ret = getDoubleTy();
        break;
    case TypeEn::unknown_jty:
        ret = nullptr;
        break;
    default:
        ret = nullptr;
        break;
    }

    return ret;
}

llvm::Module* IRGenerator::getCurrentModule() { return table_->getModule(); }

llvm::BasicBlock* IRGenerator::createNewBb(basicBlockPtr_t& prot, const std::string& name) {
    prot = llvm::BasicBlock::Create(Context, name, getCurrentFunction());
    bb_List_.push_back(prot);
    return prot;
}
