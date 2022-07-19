#include "parser/body.h"

#include <list>
#include <string>

#include "parser/bodyTemplate.h"
#include "parser/line.h"



Body::Body(const std::string& name, const std::list<std::string>& names_of_defined_functions, Body* parent,
           bool is_operator)
    : declarated_functions_map_(names_of_defined_functions),
      parent_body_(parent),
      is_operator_(is_operator),
      name_(name) {
    garbage_contaiiner_ = new GarbageContainer;
    lines_.reserve(30);
}

Body::~Body() { delete garbage_contaiiner_; }

// varStack push/pop
void Body::push(ExValue_ifs* line) {
    garbage_contaiiner_->add(line);
    var_stack_.push_back(line);
}

ExValue_ifs* Body::pop() {
    if (var_stack_.empty()) print_error("stack is empty");
    return var_stack_.pop();
}

stack<ExValue_ifs*> Body::pop(size_t length) { return var_stack_.pop(length); }


void Body::addLine(const std::string& name, ExValue_ifs* var) {
    auto line = new Line(name, var);
    garbage_contaiiner_->add(line);
    lines_.push_back(line);
}

void Body::addVariableLine(const std::string& name, ExValue_ifs* var) {
    Line* line = new Line(name, var->type_, DataStructureTypeEn::kVariable, 1);
    garbage_contaiiner_->add(line);
    lines_.push_back(line);
}

void Body::addParam(Line* line) {  //?delete
    garbage_contaiiner_->add(line);
    arg_count_++;
    lines_.push_back(line);
}

void Body::addReturn(const std::string& name, ExValue_ifs* var) {  //?remove Value param
    auto line = new Line(name, var);

    if (is_tail_callable_) {
        var = var->getAssignedVal(true);

        bool valid_recursion = var->getNodeType() == NodeTypeEn::kTailCallSelect;
        if (!valid_recursion) print_error("it isn't tail recursion");
    }

    garbage_contaiiner_->add(line);
    return_stack_.push_back(line);
}



void Body::setFunctionBody(Body* body) {  // NOLINT
    if (!declarated_functions_map_.setFunctionBody(body)) {
        if (parent_body_) parent_body_->setFunctionBody(body);
    }
}

std::map<std::string, std::string> Body::getParameterLinkNames(bool hide_unused) const {
    std::map<std::string, std::string> ret;
    for (auto& value : lines_)
        if (value->isArg())
            if (!(hide_unused && value->isUnused())) ret[value->getName(true)] = value->getLinkName();
    return ret;
}

Line* Body::getLastLineFromName(const std::string& name) const {
    if (lines_.empty()) return nullptr;

    for (int i = (int)lines_.size() - 1; i >= 0; i--) {
        if (lines_[i]->checkName(name)) return (lines_[i]);
    }
    print_error("unknown symbol " + name);
    return nullptr;
}

std::list<ParameterIfs*> Body::getOutputParameterList() const {
    std::list<ParameterIfs*> p_list;
    for (auto i : return_stack_) {
        p_list.push_back(i->getAssignedVal(true)->getParameter());
    }
    return p_list;
}

Signature Body::getSignature() const {
    Signature ret;
    for (auto& arg : lines_)
        if (arg->isArg()) ret.push_back(arg->type_);
    return ret;
}

Body* Body::getFunctionBody(const std::string& name, const Signature& signature) const {  // NOLINT
    Body* body = declarated_functions_map_.getFunctionBody(name, signature);
    if (body) return body;

    if (parent_body_) return parent_body_->getFunctionBody(name, signature);
    return nullptr;
}



// tree walker methods
std::string Body::print(const string& tab, bool DSTEna, bool hide_unused_lines) {
    PrintBodyContext context(tab, DSTEna, hide_unused_lines);
    stack<ExValue_ifs*> visitor_stack;

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



void Body::simplify() {
    stack<ExValue_ifs*> visitor_stack;
    for (auto& value : return_stack_) {
        visitor_stack.push(value->getAssignedVal());
        do {
            auto var = visitor_stack.pop();
            var->reverseTraversalVisitEnter(&visitor_stack);

        } while (!visitor_stack.empty());
    }
}


void Body::genTable(TableGenContext* context) {
    stack<ExValue_ifs*> visitor_stack;

    if (name_ == "main")
        for (auto& value : return_stack_) value->getAssignedVal(true)->setReturned();

    for (auto& value : lines_) {
        if (value->isArg() && (!value->isUnused())) {
            context->setUint(value);
        } else if (!value->isUnused()) {
            visitor_stack.push(value->getAssignedVal());
            do {
                auto var = visitor_stack.pop();
                if (var->isVisited()) var->genBlocksVisitExit(context);
                else var->visitEnter(&visitor_stack);
            } while (!visitor_stack.empty());
        }
    }
    int64_t maxBufferLength = 0;
    for (auto& value : return_stack_) {
        visitor_stack.push(value->getAssignedVal());
        do {
            auto var = visitor_stack.pop();
            if (var->isVisited()) var->genBlocksVisitExit(context);
            else var->visitEnter(&visitor_stack);
        } while (!visitor_stack.empty());


        auto temp = value->getLength();
        if (maxBufferLength < temp) maxBufferLength = temp;
    }

    if (name_ == "main") context->setMaxBufferLength(maxBufferLength);
}


Body* DeclaredBodiesMap::getFunctionBody(const std::string& name, const Signature& signature) const {
    auto a = find(name);
    if (a == end()) return nullptr;

    for (auto i : a->second) {
        if (signature == i->getSignature()) return i;
    }
    return nullptr;
}

bool DeclaredBodiesMap::setFunctionBody(Body* body) {
    auto a = find(body->getName());
    if (a == end()) return false;
    a->second.push_back(body);
    return true;
}
