#include "parser/call.h"


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


CallRecursiveFunction::CallRecursiveFunction(Body* body, const stack<ExValue*>& args) : CallI_ifs() {
    body_ = body;
    args_ = args;

    if (body_->getRet().empty()) return;


    auto ret = body_->getRet().front();

    std::vector<ParameterIfs*> p_list;


    for (auto i : args_) {
        p_list.push_back(i->getAssignedVal(true)->getParameter());
    }

    ExValue* temp = nullptr;
    for (auto i : args_) {
        level_ = maxInt(level_, i->getLevel());
        if (i->isArray()) temp = i;
    }

    type_ = ret->getType();

    if (temp == nullptr) print_error("CallRecursiveFunction::CallRecursiveFunction : temp == nullptr");

    length_ = temp->getLength();
    data_structure_type_ = temp->getDSType();


    for (auto i : args_)
        if (i->getLevel() < level_) i->getAssignedVal(true)->setBuffered();


    if (isConst(ret)) {
        binary_value_ = ret->getBinaryValue();
        text_value_ = ret->getTextValue();
    }
}


void CallRecursiveFunction::markUnusedVisitEnter(stack<ExValue*>* visitor_stack) {
    commonMarkUnusedVisitEnter(visitor_stack);
    for (int64_t i = ((int64_t)args_.size() - 1); i >= 0; i--) {
        auto arg = args_[i];
        visitor_stack->push(arg);
        if (arg->isArray()) arg->setBufferLength(this);
    }
    is_unused_ = false;
}

void CallRecursiveFunction::genBlocksVisitExit(TableGenContext* context) {
    unique_name_ = (isLargeArr(this) ? "Rec_fb" : "Rec_fs") + std::to_string(context->getUniqueIndex());
    context->setUint(this);
    is_visited_ = false;


    PrmTypesEn RPMType = JITType2PRMType(type_);

    for (auto i : args_)
        if (i->isArray()) {
            parameter_ = retyping(i->getAssignedVal(true)->getParameter(), RPMType, "");
            break;
        }

    context->setParameter(parameter_);
    if (isReturned()) {
        parameter_ = parameter_->newParameter();
        // parameter_ =new SyncParameter("", parameter_->getMainTimeInterval(), parameter_->getDataIntervalList(),
        // false);
        context->setParameter(parameter_);
    }
}