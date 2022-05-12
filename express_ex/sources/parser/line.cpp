#include "parser/line.h"

#include "parser/operations.h"

// void print_error(std::string content);

template <class T>
const T& max(const T& a, const T& b) {
    return (a < b) ? b : a;  // or: return comp(a,b)?b:a; for version (2)
}

bool Line::isArg() const { return is_arg_; }

void Line::assignValue(Value* var) {
    assigned_val_ = var;
    type_         = var->getType();
}

bool Line::checkName(const std::string& name) const {
    if (name_ == name) return true;
    return false;
}

Value* Line::getAssignedVal(bool deep) {
    if (is_arg_) return this;

    if (deep) return assigned_val_->getAssignedVal(true);

    return assigned_val_;
}

// safe functions .external stack is used

void Line::markUnusedVisitEnter(stack<Value*>* visitor_stack) {
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
    is_visited_  = false;
}

void Line::visitEnter(stack<Value*>* visitor_stack) {
    visitor_stack->push(this);
    is_visited_ = true;
}

void Line::genBodyVisitExit(BodyGenContext* context) {
    is_visited_                   = false;
    std::vector<Line*> namespace_ = context->getNamespace();

    std::string name = getName(true);
    if (namespace_.empty()) return;
    for (int i = (int)namespace_.size() - 1; i >= 0; i--) {
        if (namespace_[i]->checkName(name)) {
            context->push(namespace_[i]);
            return;
        }
    }
    print_error("visitExit can't find var name");
}

void Line::printVisitExit(PrintBodyContext* context) {
    is_visited_ = false;
    context->push(name_ + "." + toString(type_));
}
