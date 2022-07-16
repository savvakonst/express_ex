#include "IntegrateOperation.h"

#include "ArithmeticOperation.h"
#include "TypeCastOperation.h"
#include "jit/IR_generator.h"
#include "parser/bodyTemplate.h"


ExValue_ifs* newIntegrateOperation(GarbageContainer* garbage_container, ExValue_ifs* value) {
    return garbage_container->add(new IntegrateOperation(value));
}

ExValue_ifs* newIntegrateOperation(BodyTemplate* body_template) {
    auto* arg = body_template->pop();
    body_template->is_operator_ = true;
    return newIntegrateOperation(body_template->getGarbageContainer(), arg);
}



IntegrateOperation::IntegrateOperation(ExValue_ifs* var)
    : Operation_ifs(type_, TypeEn::unknown_jty, OpCodeEn::integrate, var) {
    // TODO: to remove
    // commonSetup(OpCodeEn::integrate, var);
    // type_ = var->getType();
    level_ = var->getLevel();
    operand_.push_back(var);
}


void IntegrateOperation::visitEnterSetupBuffer(stack<ExValue_ifs*>* visitor_stack) {
    Operation_ifs::visitEnterSetupBuffer(visitor_stack);
}

void IntegrateOperation::visitEnterStackUpdate(stack<ExValue_ifs*>* visitor_stack) { visitor_stack->push(operand_[0]); }

void IntegrateOperation::genBodyVisitExit(BodyGenContext* context) {
    is_visited_ = false;

    GarbageContainer* garbage_container = context->getGarbageContainer();
    g_pos = pos_;

    auto* operand = context->pop();
    if (isBool(operand->type_)) print_error("IntegrateOperation::genBodyVisitExit: operand type is bool ");

    context->push(newIntegrateOperation(garbage_container, operand));
}

void IntegrateOperation::calculateConstRecursive(RecursiveGenContext* context) {
    print_error("IntegrateOperation::calculateConstRecursive: invalid operation");
}

void IntegrateOperation::printVisitExit(PrintBodyContext* context) {
    is_visited_ = false;

    auto op = context->pop();
    context->push(checkBuffer("integrate(" + op + ")"));
}

std::string IntegrateOperation::printUint() {
    is_visited_ = false;

    auto name_op_a = operand_[0]->getAssignedVal(true)->getUniqueName();

    return getUniqueName() + " = integrate( " + name_op_a + ")";
}

void IntegrateOperation::setupIR(IRGenerator& builder) {
    auto ir_op_a = operand_[0]->getAssignedVal(true)->getIRValue(builder, level_);

    if (local_buffer_ptr_ == nullptr) local_buffer_ptr_ = builder.createLocalBuffer(type_, 1);

    auto last_block = builder.setInitInsertPoint();


    builder.setLoadInsertPoint();
    auto temp_val = builder.CreateLoad(builder.getLLVMType(type_), local_buffer_ptr_);
    builder.setCalcInsertPoint();
    if (isFloating(type_)) IR_value_ = builder.CreateFAdd(ir_op_a, temp_val, "integral");
    else IR_value_ = builder.CreateAdd(ir_op_a, temp_val, "integral");

    builder.setStoreInsertPoint();
    builder.CreateStore(IR_value_, local_buffer_ptr_);

    builder.SetInsertPoint(last_block);
    finishSetupIR(builder);
}

void IntegrateOperation::calculate() {}
