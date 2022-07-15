//
// Created by SVK on 15.07.2022.
//

#include "NeighborPointOperation.h"

#include "ArithmeticOperation.h"
#include "TypeCastOperation.h"
#include "jit/IR_generator.h"
#include "parser/bodyTemplate.h"

ExValue *newNeighborPointOperation(GarbageContainer *garbage_container, TypeEn target_type, ExValue *arg_a,
                                   ExValue *arg_b, int64_t shift, OpCodeEn op_type) {
    return nullptr;
}
NeighborPointOperation::NeighborPointOperation(ExValue *large_arr, ExValue *small_arr, int64_t shift) {}
void NeighborPointOperation::visitEnterSetupBuffer(stack<ExValue *> *visitor_stack) {
    Operation_ifs::visitEnterSetupBuffer(visitor_stack);
}

void NeighborPointOperation::visitEnterStackUpdate(stack<ExValue *> *visitor_stack) {
    Operation_ifs::visitEnterStackUpdate(visitor_stack);
}

void NeighborPointOperation::genBodyVisitExit(BodyGenContext *context) { Operation_ifs::genBodyVisitExit(context); }

void NeighborPointOperation::calculateConstRecursive(RecursiveGenContext *context) {
    Operation_ifs::calculateConstRecursive(context);
}

void NeighborPointOperation::printVisitExit(PrintBodyContext *context) { Operation_ifs::printVisitExit(context); }

std::string NeighborPointOperation::printUint() { return Operation_ifs::printUint(); }

void NeighborPointOperation::setupIR(IRGenerator &builder) {
    finishSetupIR();
}

void NeighborPointOperation::calculate() { Operation_ifs::calculate(); }
