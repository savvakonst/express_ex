//
// Created by svk on 21.07.2022.
//

#include "RecurrenceRelationTemplate.h"

#include "ArithmeticOperation.h"
#include "RecursiveNeighborPointOperation.h"
#include "jit/IR_generator.h"
#include "parser/bodyTemplate.h"

ExValue_ifs *newRecurrenceRelationTemplate(GarbageContainer *garbage_container, ExValue_ifs *ret_value) {
    if (isUnknownTy(ret_value))
        print_error("you need to determine type of function which implement recurrence relation");
    return nullptr;
}

RecurrenceRelationTemplate::RecurrenceRelationTemplate(ExValue_ifs *ret_value) {}

void RecurrenceRelationTemplate::visitEnterSetupBuffer(stack<ExValue_ifs *> *visitor_stack) {
    Operation_ifs::visitEnterSetupBuffer(visitor_stack);
}

void RecurrenceRelationTemplate::visitEnterStackUpdate(stack<ExValue_ifs *> *visitor_stack) {
    // Operation_ifs::visitEnterStackUpdate(visitor_stack);
}

void RecurrenceRelationTemplate::genBodyVisitExit(BodyGenContext *context) {
    // Operation_ifs::genBodyVisitExit(context);

    stack<ExValue_ifs *> path_stack;
    stack<ExValue_ifs *> temp_stack;
    stack<ExValue_ifs *> visitor_stack;

    visitor_stack.push(operand_[0]->getAssignedVal());
    do {
        auto var = visitor_stack.pop();
        if (var->getNodeType() == NodeTypeEn::kRecursiveNeighborPoint) {
            // var->
        }

        var->reverseTraversalVisitEnter(&temp_stack);
        path_stack.push_back(temp_stack.front());
        temp_stack.pop_front();
        visitor_stack.emplace_back(temp_stack);


    } while (!visitor_stack.empty());
}

void RecurrenceRelationTemplate::calculateConstRecursive(RecursiveGenContext *context) {
    // Operation_ifs::calculateConstRecursive(context);
}

void RecurrenceRelationTemplate::printVisitExit(PrintBodyContext *context) { Operation_ifs::printVisitExit(context); }

std::string RecurrenceRelationTemplate::printUint() { return Operation_ifs::printUint(); }

void RecurrenceRelationTemplate::setupIR(IRGenerator &builder) { Operation_ifs::setupIR(builder); }

void RecurrenceRelationTemplate::calculate() {
    // Operation_ifs::calculate();
}
