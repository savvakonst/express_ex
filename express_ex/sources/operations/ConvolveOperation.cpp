#include "operations/ConvolveOperation.h"

#include "jit/IR_generator.h"
#include "operations/ArithmeticOperation.h"
#include "operations/TypeCastOperation.h"

Value* newConvolveOperation(GarbageContainer* garbage_container, TypeEn target_type, Value* arg_a, Value* arg_b,
                            int64_t shift, OpCodeEn op_type) {
    if (op_type != OpCodeEn::convolve) print_error("convolve_f operation is not supported yet");

    if ((isConst(arg_a) || isConst(arg_b)) && !(isUnknownTy(arg_a) || isUnknownTy(arg_b))) {
        return garbage_container->add(
            newArithmeticOperation(garbage_container, target_type, arg_a, arg_b, OpCodeEn::mul));
    }
    if (isSmallArr(arg_a) && isSmallArr(arg_b)) {
        return garbage_container->add(new ConvolveOperation(op_type, arg_a, arg_b, shift));
        // print_error("convolve(SmallArr_t,SmallArr_t) - is not supported yet");
    }
    if (isSmallArr(arg_a) != isSmallArr(arg_b)) {
        if (isSmallArr(arg_b)) return garbage_container->add(new ConvolveOperation(op_type, arg_a, arg_b, shift));
        return garbage_container->add(new ConvolveOperation(op_type, arg_b, arg_a, shift));
    }
    if (isLargeArr(arg_a) && isLargeArr(arg_b)) {
        print_error("convolve(LargeArr_t,LargeArr_t) - is not supported");
    }

    print_error("convolve(xxx,xxx) - unknown case");
    return nullptr;
}

void ConvolveOperation::visitEnterSetupBuffer(stack<Value*>* visitor_stack) {
    auto small_array = operand_[1];
    auto shift = shift_parameter_;

    int64_t len = small_array->getLength();
    int64_t le = len / 2;
    int64_t ri = (len - 1) / 2;

    auto left = this->getLeftBufferLen() + ((shift < 0) ? 0 : shift) + le;
    auto right = this->getRightBufferLen() + ((shift > 0) ? 0 : -shift) + ri;

    operand_[0]->getAssignedVal(true)->setBuffered();
    operand_[0]->setBufferLength(left, right);
}

void ConvolveOperation::visitEnterStackUpdate(stack<Value*>* visitor_stack) {
    visitor_stack->push(operand_[1]);
    visitor_stack->push(operand_[0]);
}

void ConvolveOperation::genBodyVisitExit(BodyGenContext* context) {
    is_visited_ = false;

    GarbageContainer* garbage_container = context->getGarbageContainer();
    g_pos = pos;

    auto op2 = context->pop();
    auto op1 = context->pop();
    const TypeEn target_type = maxTypeVar(op1, op2)->getType();
    auto ret = newConvolveOperation(garbage_container, target_type, newTypeConvOp(garbage_container, target_type, op1),
                                    newTypeConvOp(garbage_container, target_type, op2), shift_parameter_, op_code_);

    context->push(ret);
}

void ConvolveOperation::calculateConstRecursive(RecursiveGenContext* context) {
    print_IR_error("ConvolveOperation::calculateConstRecursive invalid operation.");
}

void ConvolveOperation::printVisitExit(PrintBodyContext* context) {
    is_visited_ = false;

    auto op2 = context->pop();
    auto op1 = context->pop();
    context->push(checkBuffer("convolve(" + op1 + ", " + op2 + "," + std::to_string(shift_parameter_) + ")"));
}

std::string ConvolveOperation::printUint() {
    auto name_op_a = operand_[0]->getAssignedVal(true)->getUniqueName();
    auto name_op_b = operand_[1]->getAssignedVal(true)->getUniqueName();
    is_visited_ = false;
    std::string u_name = getUniqueName();

    return u_name + " = convolve( " + name_op_a + ", " + name_op_b + ", " + std::to_string(shift_parameter_) + ")";
}

void ConvolveOperation::setupIR(IRGenerator& builder) {
    auto ir_op_a_ptr = operand_[0]->getAssignedVal(true)->getIRValuePtr(builder, level_);

    auto second_op = operand_[1]->getAssignedVal(true);
    auto length = second_op->getLength();

    IR_value_ = builder.createConvolve(ir_op_a_ptr, second_op->getBufferPtr(), length, -(length / 2 + shift_parameter_),
                                       type_, getUniqueName());
}

void ConvolveOperation::calculate() {
    auto op_a = operand_[0]->getAssignedVal(true);
    auto op_b = operand_[1]->getAssignedVal(true);
    buffer_ptr_ = calcConvolveSmallArray(type_, buffer_ptr_, op_a->getBufferPtr(), op_b->getBufferPtr(),
                                         (int)op_a->getLength(), (int)op_b->getLength());
}
