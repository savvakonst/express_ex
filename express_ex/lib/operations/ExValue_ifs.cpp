#include "operations/ExValue_ifs.h"

#include "jit/IR_generator.h"



void ExValue_ifs::setBuffered() {
    if (isLargeArr(this)) {
        is_buffered_ = true;
    }
}

void ExValue_ifs::setBufferLength(uint64_t central_length) { buffer_length_ = central_length; }


void ExValue_ifs::setBufferBordersLength(uint64_t left, uint64_t right) {
    if (isLargeArr(this)) {
        left_buffer_length_ = maxInt(left_buffer_length_, left);
        right_buffer_length_ = maxInt(right_buffer_length_, right);
    }
}

void ExValue_ifs::setBufferBordersLength(ExValue_ifs* var) {
    if (isLargeArr(this)) {
        left_buffer_length_ = maxInt(left_buffer_length_, var->getLeftBufferLen());
        right_buffer_length_ = maxInt(right_buffer_length_, var->getRightBufferLen());
    }
}



bool ExValue_ifs::isVisited() const { return is_visited_; }


void ExValue_ifs::calculate() { print_SA_error("ExValue_ifs::calculate "); }


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
