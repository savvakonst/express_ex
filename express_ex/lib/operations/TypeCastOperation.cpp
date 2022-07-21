#include "TypeCastOperation.h"

#include "jit/IR_generator.h"
#include "jit/TableGenContext.h"
#include "operations/ExConstValue.h"
#include "parser/bodyTemplate.h"


static std::string txtTypeCastOp(OpCodeEn op_code) { return kOpCodesStr[(int)op_code]; }

ExValue_ifs* newTypeConvOp(GarbageContainer* garbage_container, TypeEn target_type, ExValue_ifs* arg) {
    const TypeEn arg_type = arg->type_;
    ExValue_ifs* ret_val = nullptr;

    if (!isUnknownTy(arg_type) && isConst(arg)) {
        ret_val = new ExConstValue(calcTypeConvConst(target_type, arg_type, arg->getBinaryValue()), target_type);
        return garbage_container->add(ret_val);
    }

    if (target_type == arg_type) {
        return arg;
    } else if (isUnknownTy(arg_type)) {
        ret_val = new TypeCastOperation(OpCodeEn::common_cast, arg, target_type);
    } else if (isFloating(target_type) && isFloating(arg_type)) {
        if (target_type < arg_type) ret_val = new TypeCastOperation(OpCodeEn::fptrunc, arg, target_type);
        else ret_val = new TypeCastOperation(OpCodeEn::fpext, arg, target_type);
    } else if (isFloating(target_type) && isInteger(arg_type)) {
        ret_val = new TypeCastOperation(OpCodeEn::sitofp, arg, target_type);
        ;
    } else if (isInteger(target_type) && isFloating(arg_type)) {
        ret_val = new TypeCastOperation(OpCodeEn::fptosi, arg, target_type);
    } else if (isInteger(target_type) && isInteger(arg_type)) {
        if (target_type < arg_type) ret_val = new TypeCastOperation(OpCodeEn::trunc, arg, target_type);
        else ret_val = new TypeCastOperation(OpCodeEn::sext, arg, target_type);
    } else {
        print_error("newTypeConvOp");
    }

    return garbage_container->add(ret_val);
}

ExValue_ifs* newTypeConvOp(BodyTemplate* body_template, TypeEn target_type) {
    auto arg = body_template->pop();
    return newTypeConvOp(body_template->getGarbageContainer(), target_type, arg);
}



TypeCastOperation::TypeCastOperation(OpCodeEn op, ExValue_ifs* var, TypeEn target_type)
    : Operation_ifs(target_type, TypeEn::unknown_jty, op, var) {
    level_ = var->getLevel();

    operand_.push_back(var);
}

void TypeCastOperation::visitEnterStackUpdate(stack<ExValue_ifs*>* visitor_stack) { visitor_stack->push(operand_[0]); }

void TypeCastOperation::genBodyVisitExit(BodyGenContext* context) {
    is_visited_ = false;
    GarbageContainer* garbage_container = context->getGarbageContainer();
    g_pos = pos_;

    auto ret = newTypeConvOp(garbage_container, type_, context->pop());

    context->push(ret);
}

void TypeCastOperation::calculateConstRecursive(RecursiveGenContext* context) {
    auto op_a = operand_[0];
    temp_type_ = type_;
    binary_value_ = calcTypeConvConst(temp_type_, op_a->getTempType(), op_a->getBinaryValue());
}

void TypeCastOperation::printVisitExit(PrintBodyContext* context) {
    is_visited_ = false;
    context->push(checkBuffer(txtTypeCastOp(op_code_) + "(" + context->pop() + ")"));
}

void TypeCastOperation::genBlocksVisitExit(TableGenContext* context) {
    unique_name_ = (isLargeArr(this) ? "vb" : "vs") + std::to_string(context->getUniqueIndex());
    context->setUint(this);
    is_visited_ = false;

    PrmTypesEn prm_type = JITType2PRMType(type_);

    parameter_ = retyping(operand_.front()->getAssignedVal(true)->getParameter(), prm_type, "");

    context->setParameter(parameter_);

    if (isReturned()) {
        parameter_ = parameter_->newParameter();
        context->setParameter(parameter_);
    }

    if (parameter_ != nullptr) context->setParameter(parameter_);
}

std::string TypeCastOperation::printUint() {
    is_visited_ = false;

    auto name_op_a = operand_[0]->getAssignedVal(true)->getUniqueName();

    return getUniqueName() + " = " + txtTypeCastOp(op_code_) + "( " + name_op_a + ")";
}

void TypeCastOperation::setupIR(IRGenerator& builder) {
    auto ir_op_a = operand_[0]->getAssignedVal(true)->getIRValue(builder, level_);

    IR_value_ = builder.createTypeConv(ir_op_a, op_code_, type_, getUniqueName());

    finishSetupIR(builder);
}

void TypeCastOperation::calculate() {
    int length = (int)length_;
    auto op_a = operand_[0]->getAssignedVal(true);

    buffer_ptr_ = calcTypeConvSmallArray(type_, op_a->type_, buffer_ptr_, op_a->getBufferPtr(), length);
}
