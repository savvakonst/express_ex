#include "parser/bodyTemplate.h"

#include <string>
#include <utility>

#include "operations/ExLine.h"
#include "operations/ExValue_ifs.h"
#include "operations/callTemplate.h"
#include "operations/operations.h"
#include "parser/body.h"



BodyTemplate::BodyTemplate(std::string name, BodyTemplate* parent_body_template)
    : name_(std::move(name)), parent_body_template_(parent_body_template) {
    garbage_container_ = new GarbageContainer();
    // lines_.reserve(30);
}

BodyTemplate::~BodyTemplate() { delete garbage_container_; }

void BodyTemplate::addLine(ExLine* line) {
    garbage_container_->add(line);
    if (line->isArg()) arg_count_++;
    references_.push_back(line);
}

void BodyTemplate::addReturn() {  //?remove Value param
    // auto line = new ExLine(name, var);
    auto var = pop();

    if (is_tail_callable_) {
        auto assigned_var = var->getAssignedVal(true);
        bool valid_recursion = assigned_var->getNodeType() == NodeTypeEn::kTailCallTernary;
        if (!valid_recursion) print_error("it isn't tail recursion");
    } else if (is_recurrence_relation_) {
        var_stack_.push_back(var);
        var = newRecurrenceRelationTemplate(this);
    }

    garbage_container_->add(var);
    return_stack_.push_back(var);
}


void BodyTemplate::push(ExValue_ifs* line) {
    garbage_container_->add(line);
    var_stack_.push_back(line);
}

ExValue_ifs* BodyTemplate::pop() {
    if (var_stack_.empty()) print_error("stack is empty");
    return var_stack_.pop();
}

stack<ExValue_ifs*> BodyTemplate::pop(size_t length) { return var_stack_.pop(length); }

std::map<std::string, std::string> BodyTemplate::getParameterLinkNames(bool hide_unused) const {
    std::map<std::string, std::string> ret;
    for (auto& value : references_)
        if (value->isArg())
            if (!(hide_unused && value->isUnused())) ret[value->getName(true)] = value->getLinkName();
    return ret;
}


void BodyTemplate::addCall(BodyTemplate* body) {
    stack<ExValue_ifs*> args;

    for (int i = body->getArgCount() - 1; i >= 0; i--) {
        args.push_front(pop());
    }

    is_operator_ = is_operator_ || body->is_operator_;

    if (body->is_tail_callable_) push(garbage_container_->add(new CallRecursiveFunctionTemplate(body, args)));
    else push(garbage_container_->add(new CallTemplate(body, args)));
}

void BodyTemplate::addTailCall() {
    stack<ExValue_ifs*> a;

    for (int i = this->getArgCount() - 1; i >= 0; i--) {
        a.push_front(pop());
    }

    if (is_tail_callable_) print_error("second recursive call");

    is_tail_callable_ = true;
    // new TailCallDirective(a);
    push(garbage_container_->add(new TailCallDirectiveTemplate(a)));
}

ExLine* BodyTemplate::getLastReferenceByName(const std::string& name) const {
    if (references_.empty()) return nullptr;
    // for (intptr_t i = intptr_t(lines_.size()) - 1; i >= 0; i--) {
    //     if (lines_[i]->checkName(name)) return (lines_[i]);
    // }

    for (auto l = references_.rbegin(); l != references_.rend(); l++) {
        if ((*l)->checkName(name)) return *l;
    }
    print_error("unknown symbol " + name);
    return nullptr;
}

BodyTemplate* BodyTemplate::getFunctionBody(const std::string& name) const {  // NOLINT
    for (auto i : child_body_template_list_)
        if (i->getName() == name) return i;

    if (parent_body_template_) return parent_body_template_->getFunctionBody(name);
    return nullptr;
}

std::string BodyTemplate::print(const std::string& tab, bool dst_ena, bool hide_unused_lines) const {
    PrintBodyContext context(tab, dst_ena, hide_unused_lines);
    stack<ExValue_ifs*> visitor_stack;

    context.setName(getName());

    for (auto& value : references_) {
        if (value->isArg()) {
            context.createArg(value);
        } else {
            visitor_stack.push(value->getAssignedVal());
            do {
                auto var = visitor_stack.pop();
                if (var->isVisited()) var->printVisitExit(&context);
                else var->visitEnter(&visitor_stack);
            } while (!visitor_stack.empty());

            context.createLine(value);
        }
    }

    for (auto& value : return_stack_) {
        visitor_stack.push(value->getAssignedVal());
        do {
            auto var = visitor_stack.pop();
            if (var->isVisited()) var->printVisitExit(&context);
            else var->visitEnter(&visitor_stack);
        } while (!visitor_stack.empty());
        context.createReturn(value);
    }

    return context.getResult();
}

std::list<std::string> BodyTemplate::getNamesOfDefinedFunctions() const {
    std::list<std::string> ret;
    for (auto i : child_body_template_list_) {
        ret.push_back(i->getName());
    }
    return ret;
}

Body* BodyTemplate::genBodyByTemplate(Body* parent_body, stack<ExValue_ifs*> args, bool is_recursive_function) const {
    auto body = new Body(name_, getNamesOfDefinedFunctions(), parent_body, is_operator_);
    BodyGenContext context(body, this, is_recursive_function);

    stack<ExValue_ifs*> visitor_stack;

    auto arg = args.begin();
    for (const auto& value : references_) {
        if (value->isArg()) {
            auto line = isTopBody()             ? (ExLine*)(*arg)
                        : is_recursive_function ? new ExRecursiveArgument(value->getName(), (*arg)->type_)
                                                : new ExArgument(value->getName(), *arg);

            body->addLine(line);
            ++arg;
        } else {
            visitor_stack.push(value->getAssignedVal());

            do {
                auto var = visitor_stack.pop();

                if (var->isVisited()) {
                    var->genBodyVisitExit(&context);
                } else {
                    var->visitEnter(&visitor_stack);
                }
            } while (!visitor_stack.empty());
            body->addLine(new ExLine(value->getName(), context.pop()));
        }
    }


    for (const auto& value : return_stack_) {
        visitor_stack.push(value);
        do {
            auto var = visitor_stack.pop();
            if (var->isVisited()) {
                var->genBodyVisitExit(&context);
            } else {
                var->visitEnter(&visitor_stack);
            }
        } while (!visitor_stack.empty());

        body->addReturn("return", context.pop());
    }

    return body;
}

untyped_t BodyTemplate::genConstRecursiveByTemplate(stack<ExValue_ifs*>& args) const {
    stack<ExValue_ifs*> visitor_stack;
    RecursiveGenContext context(is_tail_callable_);

    auto arg = args.begin();

    for (auto& line : references_) {
        if (line->isArg()) {
            line->setTempTypeAndBinaryValue(*arg++);
            context.addArg(line);
        } else {
            visitor_stack.push(line->getAssignedVal());
            do {
                auto var = visitor_stack.pop();
                if (var->isVisited()) var->genRecursiveVisitExit(&context);
                else var->visitEnter(&visitor_stack);
            } while (!visitor_stack.empty());
            context.setUint(line);
        }
    }

    for (auto& value : return_stack_) {
        visitor_stack.push(value->getAssignedVal());
        do {
            auto var = visitor_stack.pop();
            if (var->isVisited()) var->genRecursiveVisitExit(&context);
            else var->visitEnter(&visitor_stack);
        } while (!visitor_stack.empty());
        context.setUint(value);
    }

    size_t size = context.instructions_list_.size();
    for (int32_t iteration_cnt = 0; !context.exitFromLoop(); iteration_cnt++) {
        for (size_t index = 0; index < size; index++) {
            auto ptr = context.instructions_list_[index];
            ptr->calculateConstRecursive(&context);
        }
        if (iteration_cnt == -1) print_error("recursion too deep");
    }

    // maybe it is not necessary

    untyped_t return_val = context.instructions_list_.back()->getBinaryValue();
    return return_val;
}

GarbageContainer::~GarbageContainer() {
    for (auto i : value_set_) delete i;
}