//
// Created by SVK on 15.07.2022.
//

#include "operations/ExConstValue.h"

#include <utility>

#include "jit/IR_generator.h"


ExConstValue::ExConstValue(const std::string &text, TypeEn type)
    : ExValue_ifs(type, TypeEn::unknown_jty, DataStructureTypeEn::kConstant, 1), text_value_(text) {
    binary_value_ = 0;

    switch (type) {
    case TypeEn::int1_jty:
        *((bool *)(&binary_value_)) = (bool)stoi(text_value_);
        break;
    case TypeEn::int8_jty:
        *((int8_t *)(&binary_value_)) = stoi(text_value_);
        break;
    case TypeEn::int16_jty:
        *((int16_t *)(&binary_value_)) = stoi(text_value_);
        break;
    case TypeEn::int32_jty:
        *((int32_t *)(&binary_value_)) = stoi(text_value_);
        break;
    case TypeEn::int64_jty:
        *((int64_t *)(&binary_value_)) = stol(text_value_);
        break;
    case TypeEn::float_jty:
        *((float *)(&binary_value_)) = stof(text_value_);
        break;
    case TypeEn::double_jty:
        *((double *)(&binary_value_)) = stod(text_value_);
        break;
    case TypeEn::unknown_jty:
        break;
    default:
        print_error("constant reading error");
        break;
    }
}



static std::string getTxtConstValue(untyped_t binary_value, TypeEn type) {
    std::string text_value;
#define OP(T) text_value = std::to_string(*((T *)(&binary_value)))
    SWITCH_TYPE_OP(type, ;);
#undef OP
    return text_value;
}

ExConstValue::ExConstValue(untyped_t binary_value, TypeEn type)
    : ExValue_ifs(type, TypeEn::unknown_jty, DataStructureTypeEn::kConstant, 1),
      text_value_(getTxtConstValue(binary_value, type)) {
    binary_value_ = binary_value;
};


double ExConstValue::getDoubleValue() const {
    double ret = 0.0;
#define OP(T) ret = (double)(*((T *)(&binary_value_)))
    SWITCH_TYPE_OP(type_, print_error("getDoubleValue error");)
#undef OP
    return ret;
}

void ExConstValue::setupIR(IRGenerator &builder) { IR_value_ = builder.createConst(binary_value_, type_, ""); }

void ExConstValue::visitEnter(stack<ExValue_ifs *> *visitor_stack) {
    visitor_stack->push(this);
    is_visited_ = true;
}

void ExConstValue::markUnusedVisitEnter(stack<ExValue_ifs *> *visitor_stack) {
    commonMarkUnusedVisitEnter(visitor_stack);
    is_unused_ = false;
}

void ExConstValue::genBodyVisitExit(BodyGenContext *context) {
    context->push(context->getGarbageContainer()->add(new ExConstValue(text_value_, type_)));
    is_visited_ = false;
}

void ExConstValue::genBlocksVisitExit(TableGenContext *context) {
    unique_name_ = "c" + std::to_string(context->getUniqueIndex());
    context->setUint(this);
    is_visited_ = false;
}

void ExConstValue::printVisitExit(PrintBodyContext *context) {
    context->push(text_value_);
    is_visited_ = false;
}

void ExConstValue::genRecursiveVisitExit(RecursiveGenContext *context) {
    // context->setUint(this);  //not used. this should be removed.
    if (!context->hide_const_values_) context->setUint(this);
    is_visited_ = false;
};