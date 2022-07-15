//
// Created by SVK on 13.07.2022.
//

#include "parser/callTemplate.h"

#include "CallRecursiveFunction.h"
#include "operations/ExConstValue.h"
#include "operations/operations.h"

CallRecursiveFunctionTemplate::CallRecursiveFunctionTemplate(BodyTemplate *body, const stack<ExValue_ifs *> &args)
    : CallTemplate_ifs() {
    body_template_ = body;
    args_ = args;

    if (body_template_->getRet().empty()) return;
    auto ret = body_template_->getRet().front();

    level_ = ret->getLevel();
    type_ = ret->getType();
    data_structure_type_ = ret->getDSType();
    length_ = ret->getLength();

    if (isConst(ret)) {
        binary_value_ = ret->getBinaryValue();
    }
}

void CallRecursiveFunctionTemplate::genBodyVisitExit(BodyGenContext *context) {
    stack<ExValue_ifs *> a;
    bool large_array = false, small_array = false;

    for (auto i : args_) {
        ExValue_ifs *var = context->pop();
        large_array |= isLargeArr(var);
        small_array |= isSmallArr(var);
        a.push(var);
    }

    if (large_array && small_array) {
        print_error("invalid signature for recursive call method");
    } else if (large_array || small_array) {
        Signature signature;
        for (auto &i : a) signature.push_back(i->getType());

        Body *body = context->getPureFunctionBody(body_template_->getName(), signature);

        if (body == nullptr) {
            body = body_template_->genBodyByTemplate(context->current_body_, a, true);
            context->setPureFunctionBody(body);
        }

        context->push(context->getGarbageContainer()->add(new CallRecursiveFunction(body, a)));

    } else {
        binary_value_ = body_template_->genConstRecursiveByTemplate(a);

        context->push(context->getGarbageContainer()->add(
            new ExConstValue(binary_value_, body_template_->getRet().front()->getTempType())));
    }

    is_visited_ = false;
}
void CallRecursiveFunctionTemplate::calculateConstRecursive(RecursiveGenContext *context) {
    binary_value_ = body_template_->genConstRecursiveByTemplate(args_);  // it might be worth changing the order
    type_ = body_template_->getRet().front()->getTempType();
    is_visited_ = false;
}



void TailCallDirectiveTemplate::genBodyVisitExit(BodyGenContext *context) {
    stack<ExValue_ifs *> a;

    std::list<TypeEn> signature;
    for (auto &i : context->getNamespace())
        if (i->isArg())
            signature.push_front(i->getType());


    for (auto &type : signature) {
        auto val = newTypeConvOp(context->getGarbageContainer(), type, context->pop());
        context->getGarbageContainer()->add(val);
        a.push(val);
    };

    context->push(context->getGarbageContainer()->add(new TailCallDirective(a)));
    is_visited_ = false;
}

void TailCallDirectiveTemplate::printVisitExit(PrintBodyContext *context) {
    std::string ret = "tailCallTemplate( " + printArgs(context) + ")";
    context->push(ret);
    is_visited_ = false;
}

void TailCallDirectiveTemplate::calculateConstRecursive(RecursiveGenContext *context) {
    size_t size = context->args_reference_.size();
    for (size_t i = 0; i < size; i++) context->args_reference_[i]->binary_value_ = args_[i]->binary_value_;
    is_visited_ = false;
}



CallTemplate::CallTemplate(BodyTemplate *body, const stack<ExValue_ifs *> &args) : CallTemplate_ifs() {
    body_template_ = body;
    args_ = args;

    if (body_template_->getRet().empty()) return;
    auto ret = body_template_->getRet().front();

    level_ = ret->getLevel();
    type_ = ret->getType();
    data_structure_type_ = ret->getDSType();
    length_ = ret->getLength();

    if (isConst(ret)) {
        binary_value_ = ret->getBinaryValue();
    }
}


void CallTemplate::genBodyVisitExit(BodyGenContext *context) {
    stack<ExValue_ifs *> a;
    for (auto &i : args_) a.push(context->pop());

    Body *body = body_template_->genBodyByTemplate(context->current_body_, a, context->is_function_);

    if (context->is_function_) {
        context->setPureFunctionBody(body);
        context->push(context->getGarbageContainer()->add(new CallRecursiveFunction(body, a)));
    } else {
        context->push(context->getGarbageContainer()->add(new Call(body, a)));
    }

    is_visited_ = false;
}


void CallTemplate::calculateConstRecursive(RecursiveGenContext *context) {
    binary_value_ = body_template_->genConstRecursiveByTemplate(args_);  // it might be worth changing the order
    type_ = body_template_->getRet().front()->getTempType();
    is_visited_ = false;
}
