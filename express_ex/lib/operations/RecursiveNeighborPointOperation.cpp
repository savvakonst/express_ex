//
// Created by SVK on 15.07.2022.
//

#include "RecursiveNeighborPointOperation.h"

#include "ArithmeticOperation.h"
#include "RecurrenceRelationTemplate.h"
#include "jit/IR_generator.h"
#include "operations/ExLine.h"
#include "parser/bodyTemplate.h"


ExValue_ifs *newRecursiveNeighborPointOperation(GarbageContainer *garbage_container, TypeEn target_type,
                                                ExValue_ifs *shift) {
    if (!isConst(shift)) {
        print_error("neighbor point index is not integer");
        return nullptr;
    }
    return garbage_container->add(new RecursiveNeighborPointOperation(target_type, shift));
}

ExValue_ifs *newRecursiveNeighborPointOperation(BodyTemplate *body_template) {
    ExValue_ifs *arg_b = body_template->pop();


    body_template->is_operator_ = true;
    body_template->is_recurrence_relation_ = true;
    return newRecursiveNeighborPointOperation(body_template->getGarbageContainer(), TypeEn::unknown_jty, arg_b);
}


RecursiveNeighborPointOperation::RecursiveNeighborPointOperation(TypeEn target_type, ExValue_ifs *shift)
    : Operation_ifs(target_type, TypeEn::unknown_jty, DataStructureTypeEn::kLargeArr, 1, OpCodeEn::none_op) {
    operand_.push_back(shift);
}

void RecursiveNeighborPointOperation::visitEnterSetupBuffer(stack<ExValue_ifs *> *visitor_stack) {
    auto shift_val = operand_[0];
    if (!(isConst(shift_val) && isInteger(shift_val)))
        print_error("NeighborPointOperation argument is not const integer");
    shift_parameter_ = typeCastConstValue<int64_t>(shift_val->type_, shift_val->getBinaryValue());

    if (shift_parameter_ >= 0) print_error("NeighborPointOperation argument must be negative");

    int64_t buffer_increment = shift_parameter_ - int64_t(left_buffer_length_);

    if (buffer_increment > 0) ref_->setBufferBordersLength(left_buffer_length_ + buffer_increment, 0);
}

void RecursiveNeighborPointOperation::visitEnterStackUpdate(stack<ExValue_ifs *> *visitor_stack) {
    visitor_stack->push(operand_[0]);
}

void RecursiveNeighborPointOperation::genBodyVisitExit(BodyGenContext *context) {
    is_visited_ = false;
    // context->GarbageContainer *garbage_container = context->getGarbageContainer();
    g_pos = pos_;

    auto type = context->getSourceBodyTemplate()->getRet().front()->type_;
    auto op_a = context->pop();


    // TODO add check for different lengths, or best add add graph pass from root and made length mutable
    // length_t len = 0;
    // for (auto line : context->getNamespace())
    //    if (line->isArg() && isLargeArr(line)) {
    //        if (len == 0) len = line->getLength();
    //        else if (len != line->getLength()) print_error("can't determine length, use arguments with same length");
    //    }

    auto ret = newRecursiveNeighborPointOperation(context->getGarbageContainer(), type, op_a);

    context->push(ret);
}

void RecursiveNeighborPointOperation::calculateConstRecursive(RecursiveGenContext *context) {
    print_SA_error("RecursiveNeighborPointOperation::calculateConstRecursive invalid operation.");
}

void RecursiveNeighborPointOperation::printVisitExit(PrintBodyContext *context) {
    is_visited_ = false;
    context->push(checkBuffer("return[" + context->pop() + "]"));
}

std::string RecursiveNeighborPointOperation::printUint() {
    is_visited_ = false;
    auto name_op_a = operand_[0]->getAssignedVal(true)->getUniqueName();
    return getUniqueName() + " =  return [" + name_op_a + "]";
}

void RecursiveNeighborPointOperation::setupIR(IRGenerator &builder) {
    ref_->finishSetupIR(builder);

    //finishSetupIR(builder);
    print_IR_error("NeighborPointOperation::setupIR is not supported yet");
}

void RecursiveNeighborPointOperation::calculate() {
    print_IR_error("NeighborPointOperation::calculate is not supported yet");
}
