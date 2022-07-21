//
// Created by SVK on 13.07.2022.
//

#include "operations/callTemplate.h"

#include "CallRecursiveFunction.h"
#include "operations/operations.h"



CallTemplate::CallTemplate(BodyTemplate *body, const stack<ExValue_ifs *> &args)
    : CallTemplate_ifs(body->getRet().front()->type_, TypeEn::unknown_jty, body->getRet().front()) {
    corresponding_body_template_ = body;
    args_ = args;

    if (corresponding_body_template_->getRet().empty()) return;
    auto ret = corresponding_body_template_->getRet().front();

    level_ = ret->getLevel();
    // type_ = ret->getType();
    // ds_ty_ = ret->getDSType();
    // length_ = ret->getLength();

    if (isConst(ret)) {
        binary_value_ = ret->getBinaryValue();
    }
}


void CallTemplate::genBodyVisitExit(BodyGenContext *context) {
    stack<ExValue_ifs *> a;
    for (auto &i : args_) a.push(context->pop());

    Body *body =
        corresponding_body_template_->genBodyByTemplate(context->current_body_, a, context->is_recursive_function_);

    // TODO check it
    if (context->is_recursive_function_) {
        context->setFunctionBody(body);
        context->push(context->getGarbageContainer()->add(new CallRecursiveFunction(body, a)));
    } else {
        context->push(context->getGarbageContainer()->add(new Call(body, a)));
    }

    is_visited_ = false;
}


void CallTemplate::calculateConstRecursive(RecursiveGenContext *context) {
    binary_value_ =
        corresponding_body_template_->genConstRecursiveByTemplate(args_);  // it might be worth changing the order
    temp_type_ = corresponding_body_template_->getRet().front()->getTempType();
    is_visited_ = false;
}
