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

NeighborPointOperation::NeighborPointOperation(ExValue_ifs *array, ExValue_ifs *shift)
    : Operation_ifs(array->getType(), TypeEn::unknown_jty, OpCodeEn::none_op, array) {
    // commonSetup(OpCodeEn::none_op, array);
    // type_ = array->getType();
    level_ = array->getLevel() + 1;
    operand_.push_back(array);
    operand_.push_back(shift);
}

void NeighborPointOperation::visitEnterSetupBuffer(stack<ExValue_ifs *> *visitor_stack) {
    Operation_ifs::visitEnterSetupBuffer(visitor_stack);
}

void NeighborPointOperation::visitEnterStackUpdate(stack<ExValue_ifs *> *visitor_stack) {
    Operation_ifs::visitEnterStackUpdate(visitor_stack);
}

void NeighborPointOperation::genBodyVisitExit(BodyGenContext *context) { Operation_ifs::genBodyVisitExit(context); }

void NeighborPointOperation::calculateConstRecursive(RecursiveGenContext *context) {}

void NeighborPointOperation::printVisitExit(PrintBodyContext *context) { Operation_ifs::printVisitExit(context); }

std::string NeighborPointOperation::printUint() { return Operation_ifs::printUint(); }

void NeighborPointOperation::setupIR(IRGenerator &builder) { finishSetupIR(builder); }

void NeighborPointOperation::calculate() {}
