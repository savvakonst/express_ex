//
// Created by SVK on 15.07.2022.
//

#include "RecursiveNeighborPointOperation.h"

#include "ArithmeticOperation.h"
#include "TypeCastOperation.h"
#include "jit/IR_generator.h"
#include "parser/bodyTemplate.h"

ExValue_ifs *newRecursiveNeighborPointOperation(GarbageContainer *garbage_container, ExValue_ifs *arg,
                                                ExValue_ifs *shift) {
    if (!isConst(shift)) {
        print_error("neighbor point index is not integer");
        return nullptr;
    }
    return garbage_container->add(new RecursiveNeighborPointOperation(arg, shift));
}

RecursiveNeighborPointOperation::RecursiveNeighborPointOperation(ExValue_ifs *array, ExValue_ifs *shift)
    : Operation_ifs(array->type_, TypeEn::unknown_jty, OpCodeEn::none_op, array) {
    // commonSetup(OpCodeEn::none_op, array);
    // type_ = array->getType();
    level_ = array->getLevel() + 1;
    operand_.push_back(array);
    operand_.push_back(shift);
}

void RecursiveNeighborPointOperation::visitEnterSetupBuffer(stack<ExValue_ifs *> *visitor_stack) {
    auto shift_val = operand_[1];
    if (!(isConst(shift_val) && isInteger(shift_val)))
        print_error("NeighborPointOperation argument is not const integer");
    auto shift = typeCastConstValue<int64_t>(shift_val->type_, shift_val->getBinaryValue());


    auto left = left_buffer_length_ + ((shift < 0) ? 0 : shift);
    auto right = right_buffer_length_ + ((shift > 0) ? 0 : -shift);

    operand_[0]->getAssignedVal(true)->setBuffered();
    operand_[0]->setBufferBordersLength(left, right);
}

void RecursiveNeighborPointOperation::visitEnterStackUpdate(stack<ExValue_ifs *> *visitor_stack) {
    visitor_stack->push(operand_[1]);
    visitor_stack->push(operand_[0]);
}

void RecursiveNeighborPointOperation::genBodyVisitExit(BodyGenContext *context) {
    is_visited_ = false;

    GarbageContainer *garbage_container = context->getGarbageContainer();
    g_pos = pos_;

    auto op2 = context->pop();
    auto op1 = context->pop();

    auto ret = newRecursiveNeighborPointOperation(garbage_container, op1, op2);

    context->push(ret);
}

void RecursiveNeighborPointOperation::calculateConstRecursive(RecursiveGenContext *context) {
    print_SA_error("RecursiveNeighborPointOperation::calculateConstRecursive invalid operation.");
}

void RecursiveNeighborPointOperation::printVisitExit(PrintBodyContext *context) {
    auto op2 = context->pop();
    auto op1 = context->pop();
    context->push(checkBuffer("(" + op1 + ")[" + op2 + "]"));
}

std::string RecursiveNeighborPointOperation::printUint() {
    is_visited_ = false;

    auto name_op_a = operand_[0]->getAssignedVal(true)->getUniqueName();
    auto name_op_b = operand_[1]->getAssignedVal(true)->getUniqueName();

    std::string u_name = getUniqueName();

    return u_name + " =  " + name_op_a + "[" + name_op_b + "]";
}

void RecursiveNeighborPointOperation::setupIR(IRGenerator &builder) {
    print_IR_error("NeighborPointOperation::setupIR is not supported yet");
    finishSetupIR(builder);
}

void RecursiveNeighborPointOperation::calculate() {
    print_IR_error("NeighborPointOperation::calculate is not supported yet");
}
