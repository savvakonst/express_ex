#include "call.h"

#include "jit/IR_generator.h"

/// //////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// //////////////////////////////////////////////////////////////////////////////////////////////////

Call::Call(Body* body, const stack<ExValue*>& args) : CallI_ifs() {
    body_ = body;
    args_ = args;

    if (body_->getRet().empty()) return;
    auto ret = body_->getRet().front();

    level_ = ret->getLevel();
    type_ = ret->getType();
    data_structure_type_ = ret->getDSType();
    length_ = ret->getLength();

    if (isConst(ret)) {
        binary_value_ = ret->getBinaryValue();
        text_value_ = ret->getTextValue();
    }
}

void Call::markUnusedVisitEnter(stack<ExValue*>* visitor_stack) {
    commonMarkUnusedVisitEnter(visitor_stack);

    if (body_->getRet().empty()) print_error("markUnusedVisitEnter: body_->getRet().empty() == true ");

    auto ret = body_->getRet().front();

    visitor_stack->push(ret);
    ret->setBufferLength(this);

    is_unused_ = false;
}

void Call::genBlocksVisitExit(TableGenContext* context) {
    body_->genTable(context);
    unique_name_ = (isLargeArr(this) ? "fb" : "fs") + std::to_string(context->getUniqueIndex());
    context->setUint(this);
    is_visited_ = false;
}


/// //////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// //////////////////////////////////////////////////////////////////////////////////////////////////
