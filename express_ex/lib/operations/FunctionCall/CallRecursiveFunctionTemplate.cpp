//
// Created by SVK on 21.07.2022.
//

#include "CallRecursiveFunction.h"
#include "operations/ExConstValue.h"
#include "operations/callTemplate.h"
#include "operations/operations.h"


CallRecursiveFunctionTemplate::CallRecursiveFunctionTemplate(BodyTemplate *body, const stack<ExValue_ifs *> &args)
    : CallTemplate_ifs(body->getRet().front()->type_, TypeEn::unknown, body->getRet().front(), args) {
    corresponding_body_template_ = body;


    if (corresponding_body_template_->getRet().empty()) return;
    auto ret = corresponding_body_template_->getRet().front();

    level_ = ret->getLevel();


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
        a.push_front(var);
    }

    if (large_array && small_array) {
        print_error("invalid signature for recursive call method");
    } else if (large_array || small_array) {
        Signature signature;
        for (auto &i : a) signature.push_back(i->type_);

        Body *body = context->getPureFunctionBody(corresponding_body_template_->getName(), signature);

        if (body == nullptr) {
            body = corresponding_body_template_->genBodyByTemplate(context->current_body_, a, true);
            context->setFunctionBody(body);
        }

        context->push(context->getGarbageContainer()->add(new CallRecursiveFunction(body, a)));

    } else {
        binary_value_ = corresponding_body_template_->genConstRecursiveByTemplate(a);

        context->push(context->getGarbageContainer()->add(
            new ExConstValue(binary_value_, corresponding_body_template_->getRet().front()->getTempType())));
    }

    is_visited_ = false;
}
void CallRecursiveFunctionTemplate::calculateConstRecursive(RecursiveGenContext *context) {
    binary_value_ =
        corresponding_body_template_->genConstRecursiveByTemplate(args_);  // it might be worth changing the order
    temp_type_ = corresponding_body_template_->getRet().front()->getTempType();
    is_visited_ = false;
}



void TailCallDirectiveTemplate::genBodyVisitExit(BodyGenContext *context) {
    stack<ExValue_ifs *> a;

    std::list<TypeEn> signature;
    for (auto &i : context->getNamespace())
        if (i->isArg()) signature.push_front(i->type_);


    for (auto &type : signature) {
        auto val = newTypeConvOp(context->getGarbageContainer(), type, context->pop());
        context->getGarbageContainer()->add(val);
        a.push_front(val);
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

    auto &ds_list = context->args_reference_;
    for (auto dst = ds_list.begin(), src = args_.begin(); dst != ds_list.end(); dst++, src++) {
        (*dst)->binary_value_ = (*src)->binary_value_;
    }
    // for (size_t i = 0; i < size; i++)
    //     context->args_reference_[i]->binary_value_ = args_[i]->binary_value_;
    is_visited_ = false;
}