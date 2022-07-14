#include "parser/bodyTemplate.h"

#include <string>
#include <utility>

#include "../operations/FunctionCall/call.h"
#include "parser/ExValue.h"
#include "parser/body.h"
#include "parser/callTemplate.h"
#include "parser/line.h"
#include "parser/operations.h"



BodyTemplate::BodyTemplate(std::string name, BodyTemplate* parent_body_template)
    : name_(std::move(name)), parent_body_template_(parent_body_template) {
    garbage_container_ = new GarbageContainer();
    lines_.reserve(30);
}

BodyTemplate::~BodyTemplate() { delete garbage_container_; }

void BodyTemplate::addLine(const std::string& name, ExValue* var) {
    auto line = new Line(name, var);
    garbage_container_->add(line);
    lines_.push_back(line);
}

void BodyTemplate::addArg(const std::string& name) {
    auto line = new Line(name);
    garbage_container_->add(line);
    arg_count_++;
    lines_.push_back(line);
}

void BodyTemplate::addParam(Line* line) {  //?delete
    garbage_container_->add(line);
    arg_count_++;
    lines_.push_back(line);
}

void BodyTemplate::addParam(const std::string& name, TypeEn ty, DataStructureTypeEn dsty, uint64_t len) {  //?delete
    auto line = new Line(name, ty, dsty, len);
    garbage_container_->add(line);
    arg_count_++;
    lines_.push_back(line);
}

void BodyTemplate::addParam(const std::string& name, const std::string& linkName, DataStructureTypeEn dsty) {
    auto line = new Line(name, linkName, dsty);
    garbage_container_->add(line);
    arg_count_++;
    lines_.push_back(line);
}

void BodyTemplate::addReturn(const std::string& name, ExValue* var) {  //?remove Value param
    auto line = new Line(name, var);

    if (is_tail_callable_) {
        auto assigned_var = var->getAssignedVal(true);

        bool valid_recursion = assigned_var->getNodeType() == NodeTypeEn::kTailCallSelect;
        if (!valid_recursion) print_error("it isn't tail recursion");
    }

    garbage_container_->add(line);
    return_stack_.push_back(line);
}


void BodyTemplate::push(ExValue* line) {
    garbage_container_->add(line);
    var_stack_.push_back(line);
}

ExValue* BodyTemplate::pop() {
    if (var_stack_.empty()) print_error("stack is empty");
    return var_stack_.pop();
}

stack<ExValue*> BodyTemplate::pop(size_t length) { return var_stack_.pop(length); }

std::map<std::string, std::string> BodyTemplate::getParameterLinkNames(bool hide_unused) const {
    std::map<std::string, std::string> ret;
    for (auto& value : lines_)
        if (value->isArg())
            if (!(hide_unused && value->isUnused())) ret[value->getName(true)] = value->getLinkName();
    return ret;
}

// create operation and push to varStack

// TODO: replace with ExValue* newTypeConvOp(BodyTemplate* body_template, TypeEn target_type);
void BodyTemplate::addTypeConvOp(TypeEn target_type) {
    ExValue* arg1 = pop();
    push(newTypeConvOp(garbage_container_, target_type, arg1));
}

// TODO: replace with ExValue* newBuiltInFuncOperation(BodyTemplate* body_template, OpCodeEn op_type);
void BodyTemplate::addBuiltInFuncOp(OpCodeEn u_type_op) {
    ExValue* arg = pop();
    auto target_type = TypeEn::unknown_jty;
    push(newBuiltInFuncOperation(garbage_container_, target_type, arg, u_type_op));
}

// TODO: replace with ExValue* newIntegrateOperation(BodyTemplate* body_template);
void BodyTemplate::addIntegrateOp() {
    ExValue* arg = pop();
    is_operator_ = true;
    push(newIntegrateOperation(garbage_container_, arg));
}
// TODO: replace with ExValue* newInversionOperation(BodyTemplate* body_template);
void BodyTemplate::addInvOp() {
    ExValue* arg = pop();
    ExValue* zero = garbage_container_->add(new ExValue("0", TypeEn::int32_jty));
    push(newArithmeticOperation(garbage_container_, TypeEn::DEFAULT_JTY, zero, arg, OpCodeEn::sub));
}

// TODO: replace with ExValue* newArithmeticOperation(BodyTemplate* body_template, OpCodeEn u_type_op);
void BodyTemplate::addArithmeticOp(OpCodeEn u_type_op) {
    ExValue* arg_b = pop();
    ExValue* arg_a = pop();
    push(newArithmeticOperation(garbage_container_, TypeEn::DEFAULT_JTY, arg_a, arg_b, u_type_op));
}

// TODO: replace with ExValue* newComparisonOperation(BodyTemplate* body_template, OpCodeEn op_type);
void BodyTemplate::addComparisonOp(OpCodeEn u_type_op) {
    ExValue* arg_b = pop();
    ExValue* arg_a = pop();
    push(newComparisonOperation(garbage_container_, TypeEn::DEFAULT_JTY, arg_a, arg_b, u_type_op));
}

// TODO: replace with ExValue* newConvolveOperation(BodyTemplate* body_template, OpCodeEn u_type_op, uint32_t shift);
void BodyTemplate::addConvolveOp(OpCodeEn u_type_op, uint32_t shift) {  // TODO: add type maching
    ExValue* arg_b = pop();
    ExValue* arg_a = pop();
    is_operator_ = true;
    push(newConvolveOperation(garbage_container_, TypeEn::DEFAULT_JTY, arg_a, arg_b, shift, u_type_op));
}


// TODO: replace with ExValue* newSelectOp(BodyTemplate* body_template);
void BodyTemplate::addSelectOp() {
    ExValue* arg_c = pop();
    ExValue* arg_b = pop();
    ExValue* arg_a = pop();

    bool valid_recursion = false;

    if (is_tail_callable_) {
        const NodeTypeEn p = arg_c->getAssignedVal(true)->getNodeType();
        valid_recursion = (p == NodeTypeEn::kTailCall);
        valid_recursion = valid_recursion || (arg_b->getAssignedVal(true)->getNodeType() == NodeTypeEn::kTailCall);
    }

    push(newSelectOp(garbage_container_, TypeEn::DEFAULT_JTY, arg_a, arg_b, arg_c, valid_recursion));
}

void BodyTemplate::addCall(BodyTemplate* body) {
    stack<ExValue*> a;
    a.resize(body->getArgCount());
    for (int i = body->getArgCount() - 1; i >= 0; i--) {
        a[i] = pop();
    }

    is_operator_ = is_operator_ || body->is_operator_;

    if (body->is_tail_callable_) push(garbage_container_->add(new CallRecursiveFunctionTemplate(body, a)));
    else push(garbage_container_->add(new CallTemplate(body, a)));
}

void BodyTemplate::addTailCall() {
    stack<ExValue*> a;
    a.resize(this->getArgCount());
    for (int i = this->getArgCount() - 1; i >= 0; i--) {
        a[i] = pop();
    }

    if (is_tail_callable_) print_error("second recursive call");

    is_tail_callable_ = true;
    // new TailCallDirective(a);
    push(garbage_container_->add(new TailCallDirectiveTemplate(a)));
}

Line* BodyTemplate::getLastLineFromName(const std::string& name) const {
    if (lines_.empty()) return nullptr;
    for (intptr_t i = intptr_t(lines_.size()) - 1; i >= 0; i--) {
        if (lines_[i]->checkName(name)) return (lines_[i]);
    }
    print_error("unknown symbol " + name);
    return nullptr;
}

BodyTemplate* BodyTemplate::getFunctionBody(const std::string& name) const {
    for (auto i : child_body_template_list_)
        if (i->getName() == name) return i;

    if (parent_body_template_) return parent_body_template_->getFunctionBody(name);
    return nullptr;
}

std::string BodyTemplate::print(const std::string& tab, bool DST_ena, bool hide_unused_lines) const {
    PrintBodyContext context(tab, DST_ena, hide_unused_lines);
    stack<ExValue*> visitor_stack;

    context.setName(getName());

    for (auto& value : lines_) {
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

Body* BodyTemplate::genBodyByTemplate(Body* parent_body, stack<ExValue*> args, bool is_function) const {
    auto body = new Body(name_, getNamesOfDefinedFunctions(), parent_body, is_operator_);
    BodyGenContext context(body, is_function);

    stack<ExValue*> visitor_stack;

    auto arg = args.begin();
    for (const auto& value : lines_) {
        if (value->isArg()) {
            if (name_ == "main") {
                body->addParam((Line*)*(arg));
                ++arg;

            } else {
                if (is_function) {
                    body->addVariableLine(value->getName(), *(arg));
                } else {
                    body->addLine(value->getName(), *(arg));
                }

                ++arg;
            }
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
            body->addLine(value->getName(), context.pop());
        }
    }


    for (const auto& value : return_stack_) {
        visitor_stack.push(value->getAssignedVal());

        do {
            auto var = visitor_stack.pop();
            if (var->isVisited()) {
                var->genBodyVisitExit(&context);
            } else {
                var->visitEnter(&visitor_stack);
            }
        } while (!visitor_stack.empty());

        body->addReturn(return_stack_[0]->getName(), context.pop());
    }

    return body;
}

untyped_t BodyTemplate::genConstRecursiveByTemplate(stack<ExValue*>& args) const {
    stack<ExValue*> visitor_stack;
    RecursiveGenContext context(is_tail_callable_);

    auto arg = args.begin();

    for (auto& line : lines_) {
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
        for (size_t index = 0; index < size; index++)
            context.instructions_list_[index]->calculateConstRecursive(&context);

        if (iteration_cnt == -1) print_error("recursion too deep");
    }

    // maybe it is not necessary

    untyped_t return_val = context.instructions_list_.back()->getBinaryValue();
    return return_val;
}

GarbageContainer::~GarbageContainer() {
    for (auto i : value_set_) delete i;
}