#include "parser/line.h"

#include "operations/operations.h"

// void print_error(std::string content);

template <class T>
const T& max(const T& a, const T& b) {
    return (a < b) ? b : a;  // or: return comp(a,b)?b:a; for version (2)
}

bool Line::isArg() const { return is_arg_; }

void Line::assignValue(ExValue_ifs* var) {
    assigned_val_ = var;
    type_ = var->getType();
}

bool Line::checkName(const std::string& name) const {
    if (name_ == name) return true;
    return false;
}

ExValue_ifs* Line::getAssignedVal(bool deep) {
    if (is_arg_) return this;

    if (deep) return assigned_val_->getAssignedVal(true);

    return assigned_val_;
}

// safe functions .external stack is used

void Line::markUnusedVisitEnter(stack<ExValue_ifs*>* visitor_stack) {
    commonMarkUnusedVisitEnter(visitor_stack);
    if (!is_arg_) {
        visitor_stack->push(assigned_val_);
        assigned_val_->setBufferLength(this);
    }
    is_unused_ = false;
}

void Line::genBlocksVisitExit(TableGenContext* context) {
    unique_name_ = (isLargeArr(this) ? "vb" : "vs") + std::to_string(context->getUniqueIndex()) + "." + name_;
    // context_->setUint(this);
    is_visited_ = false;
}

void Line::visitEnter(stack<ExValue_ifs*>* visitor_stack) {
    visitor_stack->push(this);
    is_visited_ = true;
}

void Line::genBodyVisitExit(BodyGenContext* context) {
    is_visited_ = false;
    std::vector<Line*> namespace_l = context->getNamespace();

    std::string name = getName(true);
    if (namespace_l.empty()) return;
    for (int i = (int)namespace_l.size() - 1; i >= 0; i--) {
        if (namespace_l[i]->checkName(name)) {
            context->push(namespace_l[i]);
            return;
        }
    }
    print_error("visitExit can't find var name: " + name);
}

void Line::printVisitExit(PrintBodyContext* context) {
    is_visited_ = false;
    context->push(name_ + "." + toString(type_));
}
