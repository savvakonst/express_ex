//
// Created by svk on 21.07.2022.
//

#include "RecurrenceRelationTemplate.h"

#include "ArithmeticOperation.h"
#include "RecursiveNeighborPointOperation.h"
#include "jit/IR_generator.h"
#include "parser/bodyTemplate.h"


ExValue_ifs *newRecurrenceRelationTemplate(BodyTemplate *body_template) {  // TODO: add type matching
    ExValue_ifs *arg_a = body_template->pop();
    return body_template->getGarbageContainer()->add(new RecurrenceRelationTemplate(arg_a));
}


RecurrenceRelationTemplate::RecurrenceRelationTemplate(ExValue_ifs *ret_value)
    : Operation_ifs(ret_value->type_, ret_value->time_type_, OpCodeEn::none_op, ret_value) {
    if (isUnknownTy(ret_value))
        print_error("you need to determine type of function which implement recurrence relation");
    level_ = ret_value->getLevel();
    operand_.push_back(ret_value);
}


void RecurrenceRelationTemplate::visitEnterSetupBuffer(stack<ExValue_ifs *> *visitor_stack) {
    Operation_ifs::visitEnterSetupBuffer(visitor_stack);
    this->setBuffered();
}

void RecurrenceRelationTemplate::visitEnterStackUpdate(stack<ExValue_ifs *> *visitor_stack) {
    visitor_stack->push(operand_[0]);
}

bool isOperator(const stack<ExValue_ifs *> &temp_stack) {
    auto it = temp_stack.begin();
    auto ref_level = (*(it++))->getLevel();
    int64_t max_input_level = 0;
    for (; it != temp_stack.end(); it++) {
        max_input_level = std::max(max_input_level, (*it)->getLevel());
    }

    if (ref_level > max_input_level) {
        return true;
    }
    return false;
}

void RecurrenceRelationTemplate::genBodyVisitExit(BodyGenContext *context) {
    stack<ExValue_ifs *> values_to_change_level;

    stack<std::pair<ExValue_ifs *, bool>> path_stack;
    stack<ExValue_ifs *> temp_stack;
    stack<ExValue_ifs *> visitor_stack;


    auto operand = context->pop();
    visitor_stack.push(operand);

    uint32_t operator_cnt = 0;

    std::list<RecursiveNeighborPointOperation *> ref_list;
    visitor_stack.push(operand);
    do {
        auto var = visitor_stack.pop();

        if (!path_stack.empty() && path_stack.back().first == var) {
            // is visited
            operator_cnt -= path_stack.back().second ? 1 : 0;
            path_stack.pop_back();
            continue;
        }
        // determine terminal nodes

        auto n_ty = var->getNodeType();
        if (n_ty == NodeTypeEn::kRecursiveNeighborPoint) {
            if (operator_cnt) print_error("operator application inside recursion is prohibited");

            ref_list.push_back((RecursiveNeighborPointOperation *)var);
            for (auto i : path_stack) values_to_change_level.push(i.first);
            values_to_change_level.push(var);
            temp_stack.push(var);
        } else if (n_ty == NodeTypeEn::kArgument) {
            temp_stack.push(var);
        } else if (n_ty == NodeTypeEn::kCall) {
            temp_stack.push(var);
        } else var->visitEnter(&temp_stack, false);

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
    print_IR_error("RecurrenceRelationTemplate::calculateConstRecursive invalid operation.");
}

void RecurrenceRelationTemplate::printVisitExit(PrintBodyContext *context) {
    is_visited_ = false;
    auto op_1 = context->pop();
    context->push(checkBuffer("rec_relation(" + op_1 + ")"));
}

std::string RecurrenceRelationTemplate::printUint() {
    is_visited_ = false;
    auto name_op_a = operand_[0]->getAssignedVal(true)->getUniqueName();
    return getUniqueName() + " = rec_relation(" + name_op_a + ")";
}

void RecurrenceRelationTemplate::setupIR(IRGenerator &builder) {
    IR_value_ = operand_[0]->getAssignedVal(true)->getIRValue(builder, level_);
    finishSetupIR(builder);

    /** this is not an obvious behaviour, however finishSetupIR() tries to store IR_value_ to IR_buffer_ptr_ but
     * if IR_value_ == nullptr this step is skipped and the developer is responsible for this step.
     */
    if (IR_value_) builder.createPositionalStore(IR_value_, IR_buffer_ptr_);
    // print_IR_error("RecurrenceRelationTemplate::setupIR is not supported yet");
}

void RecurrenceRelationTemplate::calculate() {
    // Operation_ifs::calculate();
}
