#include "operations/ExValue_ifs.h"

#include "jit/IR_generator.h"



void ExValue_ifs::setBuffered() {
    if (isLargeArr(this)) {
        is_buffered_ = true;
    }
}

void ExValue_ifs::setBufferLength(uint64_t central_length) { buffer_length_ = central_length; }

void ExValue_ifs::setBufferLength(uint64_t left, uint64_t right) {
    if (isLargeArr(this)) {
        left_buffer_length_ = maxInt(left_buffer_length_, left);
        right_buffer_length_ = maxInt(right_buffer_length_, right);
    }
}

void ExValue_ifs::setBufferLength(ExValue_ifs* var) {
    if (isLargeArr(this)) {
        left_buffer_length_ = maxInt(left_buffer_length_, var->getLeftBufferLen());
        right_buffer_length_ = maxInt(right_buffer_length_, var->getRightBufferLen());
    }
}

void ExValue_ifs::setLevel(int64_t var) { level_ = maxInt(level_, var); }

std::string ExValue_ifs::getTxtDSType() const { return toString(data_structure_type_); }

double ExValue_ifs::getDoubleValue() const {
    double ret = 0.0;
#define OP(T) ret = (double)(*((T*)(&binary_value_)))
    SWITCH_TYPE_OP(type_, print_error("getDoubleValue error");)
#undef OP
    return ret;
}

bool ExValue_ifs::isVisited() const { return is_visited_; }

void ExValue_ifs::calculate() {
    double delta = 0.0;
    if (isInteger(type_)) {
        delta = (stop_ - start_) / length_;
    }

    buffer_ptr_ = calcSmallArrayAlloc(type_, (int)length_);

#define OP(T)                                                                      \
    {                                                                              \
        T* point = (T*)buffer_ptr_;                                                \
        for (uint64_t i = 0; i < length_; i++) *point++ = (T)(start_ + delta * i); \
    }

    SWITCH_TYPE_OP(type_, print_SA_error("samallarray range ");)
#undef OP
}

std::string ExValue_ifs::printSmallArray() {
    std::string ret = "array=[";
    if (length_ < 1) return ret;

#define OP(T)                                                                         \
    {                                                                                 \
        T* point = (T*)buffer_ptr_;                                                   \
        ret += std::to_string(point[0]);                                              \
        for (uint64_t i = 1; i < length_; i++) ret += "," + std::to_string(point[i]); \
    }
    SWITCH_TYPE_OP(type_, print_SA_error("printSmallArray");)
#undef OP
    ret += "];";
    return ret;
}

/// Implementation of Value, Operation, Line and Call  members,
/// which provide llvm IR generation.
///
///
llvm::Value* ExValue_ifs::getIRValue(IRGenerator& builder, int64_t parent_level) {
    llvm::Value* ret = nullptr;
    if (isBuffered() && (parent_level != level_)) {
        if (!builder.checkExistence(IR_buffer_base_ptr_)) {
            IR_buffer_ptr_ = builder.createPositionalInBoundsGep(IR_buffer_base_ptr_, builder.getCurrentOffsetValue(),
                                                                 "offset_val_incr_");
            IR_loaded_buffer_value_ = builder.createPositionalLoad(IR_buffer_ptr_, true, "buffer_val_");
            builder.addInitializedValue(IR_buffer_base_ptr_);
        }
        ret = IR_loaded_buffer_value_;
    } else ret = IR_value_;

    if ((ret == nullptr) && !builder.is_pure_function_) print_IR_error("IRValue - is nullptr :" + getUniqueName());

    return ret;
}

llvm::Value* ExValue_ifs::getIrValueBasePtr(IRGenerator& builder, int64_t parent_level) {
    if (isBuffered() && (parent_level != level_)) {
        if (!builder.checkExistence(IR_buffer_base_ptr_)) {
            IR_buffer_ptr_ = builder.createPositionalInBoundsGep(IR_buffer_base_ptr_, builder.getCurrentOffsetValue(),
                                                                 "offset_val_incr_");
            IR_loaded_buffer_value_ = builder.createPositionalLoad(IR_buffer_ptr_, true, "buffer_val_");
            builder.addInitializedValue(IR_buffer_base_ptr_);
        }
    }

    auto ret = isBuffered() ? IR_buffer_base_ptr_ : nullptr;
    if (ret == nullptr) print_IR_error("getIRValueBasePtr - is nullptr :" + getUniqueName());
    return ret;
}

llvm::Value* ExValue_ifs::getIrValuePtr(IRGenerator& builder, int64_t parent_level) {
    if (isBuffered() && (parent_level != level_)) {
        if (!builder.checkExistence(IR_buffer_base_ptr_)) {
            IR_buffer_ptr_ = builder.createPositionalInBoundsGep(IR_buffer_base_ptr_, builder.getCurrentOffsetValue(),
                                                                 "offset_val_incr_");
            IR_loaded_buffer_value_ = builder.createPositionalLoad(IR_buffer_ptr_, true, "buffer_val_");
            builder.addInitializedValue(IR_buffer_base_ptr_);
        }
    }

    auto ret = isBuffered() ? IR_buffer_ptr_ : nullptr;
    if (ret == nullptr) print_IR_error("getIRValuePtr - is nullptr :" + getUniqueName());
    return ret;
}

void ExValue_ifs::setupIR(IRGenerator& builder) { IR_value_ = builder.createConst(binary_value_, type_, ""); }
