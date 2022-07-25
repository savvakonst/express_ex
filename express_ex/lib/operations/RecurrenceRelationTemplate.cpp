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

    return garbage_container->add(new RecurrenceRelationTemplate(ret_value));
}

RecurrenceRelationTemplate::RecurrenceRelationTemplate(ExValue_ifs *ret_value)
    : Operation_ifs(ret_value->type_, ret_value->time_type_, OpCodeEn::none_op, ret_value) {
    if (isUnknownTy(ret_value))
        print_error("you need to determine type of function which implement recurrence relation");
    level_ = ret_value->getLevel();
}



void RecurrenceRelationTemplate::visitEnterSetupBuffer(stack<ExValue_ifs *> *visitor_stack) {
    Operation_ifs::visitEnterSetupBuffer(visitor_stack);
}

void RecurrenceRelationTemplate::visitEnterStackUpdate(stack<ExValue_ifs *> *visitor_stack) {
    // Operation_ifs::visitEnterStackUpdate(visitor_stack);
}

bool isOperator(const stack<ExValue_ifs *> &temp_stack) {
    auto it = temp_stack.begin();
    auto ref_level = (*it++)->getLevel();
    int64_t max_input_level = 0;
    for (; it != temp_stack.end(); it++) {
        max_input_level = std::max(max_input_level, (*it++)->getLevel());
    }

    if (ref_level > max_input_level) {
        return true;
    }
    return false;
}

void RecurrenceRelationTemplate::genBodyVisitExit(BodyGenContext *context) {
    // Operation_ifs::genBodyVisitExit(context);

    stack<ExValue_ifs *> values_to_change_level;

    stack<std::pair<ExValue_ifs *, bool>> path_stack;
    stack<ExValue_ifs *> temp_stack;
    stack<ExValue_ifs *> visitor_stack;
    auto operand = context->pop();
    visitor_stack.push(operand);

    uint32_t operator_cnt = 0;

    std::list<RecursiveNeighborPointOperation *> ref_list;
    do {
        auto var = visitor_stack.pop();

        if (path_stack.front().first == var) {
            // is visited
            operator_cnt -= path_stack.front().second ? 1 : 0;
            path_stack.pop_front();
            continue;
        }

        if (var->getNodeType() == NodeTypeEn::kRecursiveNeighborPoint) {
            if (operator_cnt) print_error("operator application inside recursion is prohibited");

            ref_list.push_back((RecursiveNeighborPointOperation *)var);
            for (auto i : path_stack) values_to_change_level.push(i.first);
            values_to_change_level.push(var);
        }

        // determine terminal node
        if (var->getNodeType() == NodeTypeEn::kArgument) temp_stack.push(var);
        else var->visitEnter(&temp_stack, false);

        bool is_operator = isOperator(temp_stack);


        operator_cnt += is_operator ? 1 : 0;
        path_stack.push_back({temp_stack.front(), is_operator});
        visitor_stack.splice(visitor_stack.end(), temp_stack);
    } while (!visitor_stack.empty());

    int64_t max_level = 0;
    for (auto i : values_to_change_level) max_level = std::max(max_level, i->getLevel());

    for (auto i : values_to_change_level) {
        i->length_ = operand->getLength();
        i->level_ = max_level;
    }

    GarbageContainer *garbage_container = context->getGarbageContainer();
    auto ret = new RecurrenceRelationTemplate(operand);

    for (auto i : ref_list) i->setRef(ret);

    context->push(garbage_container->add(ret));
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
