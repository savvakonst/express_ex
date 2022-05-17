#include "operations/IntegrateOperation.h"

#include "jit/IR_generator.h"
#include "operations/ArithmeticOperation.h"
#include "operations/TypeCastOperation.h"

Value* newIntegrateOperation(GarbageContainer* garbage_container, Value* value) {
    return new IntegrateOperation(value);
}

void IntegrateOperation::visitEnterSetupBuffer(stack<Value*>* visitor_stack) {
    Operation_ifs::visitEnterSetupBuffer(visitor_stack);
}

void IntegrateOperation::visitEnterStackUpdate(stack<Value*>* visitor_stack) { visitor_stack->push(operand_[0]); }

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

void IntegrateOperation::setupIR(IRGenerator& builder) { Operation_ifs::setupIR(builder); }

void IntegrateOperation::calculate() {}
