//
// Created by SVK on 15.07.2022.
//

#include "NeighborPointOperation.h"

#include "ArithmeticOperation.h"
#include "TypeCastOperation.h"
#include "jit/IR_generator.h"
#include "parser/bodyTemplate.h"

ExValue_ifs *newNeighborPointOperation(GarbageContainer *garbage_container, ExValue_ifs *arg, ExValue_ifs *shift) {
    if (!isConst(shift)) {
        print_error("neighbor point index is not integer");
        return nullptr;
    }
    return garbage_container->add(new NeighborPointOperation(arg, shift));
}

ExValue_ifs *newNeighborPointOperation(BodyTemplate *body_template) {
    ExValue_ifs *array_val = body_template->pop();
    ExValue_ifs *shift_val = body_template->pop();

    body_template->is_operator_ = true;

    return newNeighborPointOperation(body_template->getGarbageContainer(), array_val, shift_val);
}

NeighborPointOperation::NeighborPointOperation(ExValue_ifs *array, ExValue_ifs *shift)
    : Operation_ifs(array->type_, TypeEn::unknown, OpCodeEn::none_op, array) {
    // commonSetup(OpCodeEn::none_op, array);
    // type_ = array->getType();
    level_ = array->getLevel() + 1;
    operand_.push_back(array);
    operand_.push_back(shift);
}

void NeighborPointOperation::visitEnterSetupBuffer(stack<ExValue_ifs *> *visitor_stack) {
    auto shift_val = operand_[1];
    if (!(isConst(shift_val) && isInteger(shift_val)))
        print_error("NeighborPointOperation argument is not const integer");
    auto shift = typeCastConstValue<int64_t>(shift_val->type_, shift_val->getBinaryValue());


    auto left = left_buffer_length_ + ((shift < 0) ? -shift : 0);
    auto right = right_buffer_length_ + ((shift > 0) ? shift : 0);

    operand_[0]->getAssignedVal(true)->setBuffered();
    operand_[0]->setBufferBordersLength(left, right);
}

void NeighborPointOperation::visitEnterStackUpdate(stack<ExValue_ifs *> *visitor_stack) {
    visitor_stack->push(operand_[1]);
    visitor_stack->push(operand_[0]);
}

void NeighborPointOperation::genBodyVisitExit(BodyGenContext *context) {
    is_visited_ = false;

    GarbageContainer *garbage_container = context->getGarbageContainer();
    g_pos = pos_;


    auto shift_val = context->pop();
    auto array_val = context->pop();

    if (!(isConst(shift_val) && isInteger(shift_val)))
        print_error("NeighborPointOperation argument is not const integer");

    auto ret = newNeighborPointOperation(garbage_container, array_val, shift_val);

    context->push(ret);
}

void NeighborPointOperation::calculateConstRecursive(RecursiveGenContext *context) {
    print_SA_error("NeighborPointOperation::calculateConstRecursive invalid operation.");
}

void NeighborPointOperation::printVisitExit(PrintBodyContext *context) {
    is_visited_ = false;

    auto shift_val = context->pop();
    auto array_val = context->pop();
    context->push(checkBuffer(" " + array_val + "[" + shift_val + "]"));
}

std::string NeighborPointOperation::printUint() {
    is_visited_ = false;

    auto name_op_a = operand_[0]->getAssignedVal(true)->getUniqueName();
    auto name_op_b = operand_[1]->getAssignedVal(true)->getUniqueName();

    std::string u_name = getUniqueName();

    return u_name + " =  " + name_op_a + "[" + name_op_b + "]";
}

void NeighborPointOperation::setupIR(IRGenerator &builder) {
    auto current_ptr = operand_[0]->getAssignedVal(true)->getIrValuePtr(builder, level_);

    auto ptr =
        builder.createPositionalInBoundsGep(current_ptr, operand_[1]->getAssignedVal(true)->getIRValue(builder, level_), "neighbor_ptr");

    IR_value_ = builder.createPositionalLoad(ptr, true, "neighbor");

    finishSetupIR(builder);
}

void NeighborPointOperation::calculate() { print_IR_error("NeighborPointOperation::calculate is not supported yet"); }
