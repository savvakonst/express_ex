#include "operations/ExValue.h"

#include "jit/IR_generator.h"

ExValue::ExValue(std::string text, TypeEn type) : SmallArr() {
    text_value_ = text;
    type_ = type;

    binary_value_ = 0;

    switch (type) {
    case TypeEn::int1_jty:
        *((bool*)(&binary_value_)) = (bool)stoi(text_value_);
        break;
    case TypeEn::int8_jty:
        *((int8_t*)(&binary_value_)) = stoi(text_value_);
        break;
    case TypeEn::int16_jty:
        *((int16_t*)(&binary_value_)) = stoi(text_value_);
        break;
    case TypeEn::int32_jty:
        *((int32_t*)(&binary_value_)) = stoi(text_value_);
        break;
    case TypeEn::int64_jty:
        *((int64_t*)(&binary_value_)) = stol(text_value_);
        break;
    case TypeEn::float_jty:
        *((float*)(&binary_value_)) = stof(text_value_);
        break;
    case TypeEn::double_jty:
        *((double*)(&binary_value_)) = stod(text_value_);
        break;
    case TypeEn::unknown_jty:
        break;
    default:
        print_error("constant reading error");
        break;
    }
};

ExValue::ExValue(untyped_t binary_value, TypeEn type) : SmallArr() {
    binary_value_ = binary_value;
    type_ = type;
#define OP(T) text_value_ = std::to_string(*((T*)(&binary_value_)))
    SWITCH_TYPE_OP(type, ;);
#undef OP
};

ExValue::ExValue(ExValue* arg_1, ExValue* arg_2, ExValue* arg_3) : SmallArr() {
    if (!(isConst(arg_1) && isConst(arg_2) && isConst(arg_3) && isInteger(arg_3))) {
        print_error("range args must be a constant");
        return;
        length_ = arg_3->getBinaryValue();
        type_ = TypeEn::double_jty;
        start_ = arg_1->getDoubleValue();
        stop_ = arg_2->getDoubleValue();
    } else {
        print_error("range(start_num,stop_num,length) -signature is not supported yet");
        return;
    }
};

ExValue::ExValue(ExValue* arg_1, ExValue* arg_2) : SmallArr() {
    if (isConst(arg_1) && isConst(arg_2) && isInteger(arg_1) && isInteger(arg_2)) {
        length_ = arg_2->getBinaryValue() - arg_1->getBinaryValue();
        data_structure_type_ = DataStructureTypeEn::kSmallArr;
        type_ = TypeEn::int64_jty;
        text_value_ =
            "range(" + std::to_string(arg_1->getBinaryValue()) + "," + std::to_string(arg_2->getBinaryValue()) + ")";
        start_ = (double)arg_1->getBinaryValue();
        stop_ = (double)arg_2->getBinaryValue();
    } else {
        print_error("range(start_num,stop_num) - arg must be integer consant");
    }
};

ExValue::ExValue(ExValue* arg_1) : SmallArr() {
    if (isConst(arg_1) && isInteger(arg_1)) {
        length_ = arg_1->getBinaryValue();
        data_structure_type_ = DataStructureTypeEn::kSmallArr;
        type_ = TypeEn::int64_jty;
        text_value_ = "range(" + std::to_string(arg_1->getBinaryValue()) + ")";
        start_ = 0.0;
        stop_ = (double)length_;
    } else {
        print_error("range(len) - arg must be integer consant");
    }
};

void ExValue::setBuffered() {
    if (isLargeArr(this)) {
        is_buffered_ = true;
    }
}

void ExValue::setBufferLength(uint64_t central_length) { buffer_length_ = central_length; }

void ExValue::setBufferLength(uint64_t left, uint64_t right) {
    if (isLargeArr(this)) {
        left_buffer_length_ = maxInt(left_buffer_length_, left);
        right_buffer_length_ = maxInt(right_buffer_length_, right);
    }
}

void ExValue::setBufferLength(ExValue* var) {
    if (isLargeArr(this)) {
        left_buffer_length_ = maxInt(left_buffer_length_, var->getLeftBufferLen());
        right_buffer_length_ = maxInt(right_buffer_length_, var->getRightBufferLen());
    }
}

void ExValue::setLevel(int64_t var) { level_ = maxInt(level_, var); }

std::string ExValue::getTxtDSType() const { return toString(data_structure_type_); }

double ExValue::getDoubleValue() const {
    double ret = 0.0;
#define OP(T) ret = (double)(*((T*)(&binary_value_)))
    SWITCH_TYPE_OP(type_, print_error("getDoubleValue error");)
#undef OP
    return ret;
}

bool ExValue::isVisited() const { return is_visited_; }

void ExValue::calculate() {
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

std::string ExValue::printSmallArray() {
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
llvm::Value* ExValue::getIRValue(IRGenerator& builder, int64_t parent_level) {
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

llvm::Value* ExValue::getIrValueBasePtr(IRGenerator& builder, int64_t parent_level) {
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

llvm::Value* ExValue::getIrValuePtr(IRGenerator& builder, int64_t parent_level) {
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

void ExValue::setupIR(IRGenerator& builder) { IR_value_ = builder.createConst(binary_value_, type_, ""); }
