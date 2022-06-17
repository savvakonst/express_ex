#include "operations/BuiltInCallOperation.h"

#include "jit/IR_generator.h"
#include "operations/TypeCastOperation.h"

const std::string ar_built_in_[6] = {"log", "log2", "log10", "cos", "sin", "exp"};

static std::string txtBuiltInOp(OpCodeEn op_code) {
    return ar_built_in_[((int)op_code - (int)TypeOpCodeEn::builtInFunc)];
}

Value* newBuiltInFuncOperation(GarbageContainer* garbage_container, TypeEn target_type, Value* arg, OpCodeEn op_type) {
    Value* var = arg;
    if (TypeEn::float_jty > target_type) {
        var = newTypeConvOp(garbage_container, TypeEn::double_jty, arg);
        target_type = var->getType();
    }

    if (isConst(var) && !isUnknownTy(target_type)) {
        if (!isFloating(target_type)) print_error("type is not float");

        return garbage_container->add(
            new Value(calcBuiltInFuncConst(op_type, target_type, var->getBinaryValue()), target_type));
    }

    return garbage_container->add(new BuiltInCallOperation(op_type, var, target_type));
}

void BuiltInCallOperation::visitEnterStackUpdate(stack<Value*>* visitor_stack) { visitor_stack->push(operand_[0]); }

void BuiltInCallOperation::genBodyVisitExit(BodyGenContext* context) {
    is_visited_ = false;

    GarbageContainer* garbage_container = context->getGarbageContainer();
    g_pos = pos;

    auto* op1 = context->pop();
    if (TypeEn::float_jty > op1->getType()) {
        op1 = newTypeConvOp(garbage_container, TypeEn::float_jty, op1);
    }
    TypeEn target_type = op1->getType();

    Value* ret = newBuiltInFuncOperation(garbage_container, target_type, op1, op_code_);

    context->push(ret);
}

void BuiltInCallOperation::calculateConstRecursive(RecursiveGenContext* context) {
    auto op = operand_[0];
    untyped_t temp_var = 0;
    temp_type_ = op->getTempType();  // was TypeEn temp_type_
    if (TypeEn::float_jty > temp_type_) {
        temp_var = calcTypeConvConst(TypeEn::float_jty, op->getTempType(), op->getBinaryValue());
        temp_type_ = TypeEn::float_jty;
    }
    binary_value_ = calcBuiltInFuncConst(op_code_, temp_type_, op->getBinaryValue());
}

void BuiltInCallOperation::printVisitExit(PrintBodyContext* context) {
    is_visited_ = false;

    std::string txt_operation = ar_built_in_[((int)op_code_ - (int)TypeOpCodeEn::builtInFunc)];
    context->push(checkBuffer(txtBuiltInOp(op_code_) + "(" + context->pop() + ")"));
}

std::string BuiltInCallOperation::printUint() {
    is_visited_ = false;

    auto name_op_a = operand_[0]->getAssignedVal(true)->getUniqueName();

    return getUniqueName() + " = " + txtBuiltInOp(op_code_) + "( " + name_op_a + ")";
}

void BuiltInCallOperation::setupIR(IRGenerator& builder) {
    auto op_a = operand_[0]->getAssignedVal(true)->getIRValue(builder, level_);
    IR_value_ = builder.createBuiltInFunc(op_a, op_code_, getUniqueName());

    finishSetupIR(builder);
}

void BuiltInCallOperation::calculate() {
    auto op_a = operand_[0]->getAssignedVal(true);
    int length = (int)length_;
    buffer_ptr_ = calcBuiltInFuncSmallArray(op_code_, type_, buffer_ptr_, op_a->getBufferPtr(), length);
}