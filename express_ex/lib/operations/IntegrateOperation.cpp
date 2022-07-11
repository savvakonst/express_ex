#include "operations/IntegrateOperation.h"

#include "jit/IR_generator.h"
#include "operations/ArithmeticOperation.h"
#include "operations/TypeCastOperation.h"

ExValue* newIntegrateOperation(GarbageContainer* garbage_container, ExValue* value) {
    return new IntegrateOperation(value);
}

void IntegrateOperation::visitEnterSetupBuffer(stack<ExValue*>* visitor_stack) {
    Operation_ifs::visitEnterSetupBuffer(visitor_stack);
}

void IntegrateOperation::visitEnterStackUpdate(stack<ExValue*>* visitor_stack) { visitor_stack->push(operand_[0]); }

void IntegrateOperation::genBodyVisitExit(BodyGenContext* context) {
    is_visited_ = false;

    GarbageContainer* garbage_container = context->getGarbageContainer();
    g_pos = pos;

    auto* operand = context->pop();
    if (isBool(operand->getType())) print_error("IntegrateOperation::genBodyVisitExit: operand type is bool ");

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

    auto pointer = builder.createLocalBuffer(type_, 1);

    auto last_block = builder.setInitInsertPoint();
    auto temp_val = builder.CreateLoad(builder.getLLVMType(type_), pointer);

    builder.setCalcInsertPoint();
    if (isFloating(type_)) IR_value_ = builder.CreateFAdd(ir_op_a, temp_val, "integral");
    else IR_value_ = builder.CreateAdd(ir_op_a, temp_val, "integral");

    builder.setStoreInsertPoint();
    builder.CreateStore(IR_value_, pointer);

    builder.SetInsertPoint(last_block);
    finishSetupIR(builder);
}

void IntegrateOperation::calculate() {}
