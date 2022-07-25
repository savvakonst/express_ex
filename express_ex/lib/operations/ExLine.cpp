#include "operations/ExLine.h"

#include "jit/IR_generator.h"
#include "jit/TableGenContext.h"
#include "operations/operations.h"

// void print_error(std::string content);

template <class T>
const T& max(const T& a, const T& b) {
    return (a < b) ? b : a;  // or: return comp(a,b)?b:a; for version (2)
}



bool ExLine::checkName(const std::string& name) const {
    if (name_ == name) return true;
    return false;
}

ExValue_ifs* ExLine::getAssignedVal(bool deep) {
    if (deep) return assigned_val_->getAssignedVal(true);
    return assigned_val_;
}

// safe functions .external stack is used

void ExLine::reverseTraversalVisitEnter(stack<ExValue_ifs*>* visitor_stack) {
    commonMarkUnusedVisitEnter(visitor_stack);
    visitor_stack->push(assigned_val_);
    assigned_val_->setBufferBordersLength(this);
    is_unused_ = false;
}

void ExLine::genBlocksVisitExit(TableGenContext* context) {
    unique_name_ = (isLargeArr(this) ? "vb" : "vs") + std::to_string(context->getUniqueIndex()) + "." + name_;
    // context_->setUint(this);
    is_visited_ = false;
}

void ExLine::visitEnter(stack<ExValue_ifs*>* visitor_stack, bool set_visited) {
    visitor_stack->push(this);
    is_visited_ = set_visited;
}

void ExLine::genBodyVisitExit(BodyGenContext* context) {
    is_visited_ = false;
    stack<ExLine*> namespace_l = context->getNamespace();

    std::string name = getName(true);
    if (namespace_l.empty()) return;
    for (auto it = namespace_l.rbegin(); it != namespace_l.rend(); it++) {
        if ((*it)->checkName(name)) {
            context->push(*it);
            return;
        }
    }
    // for (int i = (int)namespace_l.size() - 1; i >= 0; i--) {
    //     if (namespace_l[i]->checkName(name)) {
    //         context->push(namespace_l[i]);
    //         return;
    //     }
    // }
    print_error("visitExit can't find var name: " + name);
}

void ExLine::printVisitExit(PrintBodyContext* context) {
    is_visited_ = false;
    context->push(name_ + "." + toString(type_));
}


void ExRecursiveArgument::setupIR(IRGenerator& builder) {
    builder.setInitInsertPoint();
    llvm::Function* function = builder.getCurrentFunction();
    IR_buffer_ptr_ = builder.CreateAlloca(builder.getLLVMType(type_));
    size_t arg_number = builder.arg_ptr_list_.size();
    builder.CreateStore(function->getArg((uint32_t)arg_number), IR_buffer_ptr_);
    builder.arg_ptr_list_.push_back(IR_buffer_ptr_);

    // insert to  loop_block
    builder.setCalcInsertPoint();
    IR_value_ = builder.CreateLoad(IR_buffer_ptr_);
}


void ExParam::setupIR(IRGenerator& builder) {


    if (!is_initialized_) {
        builder.addBuffer(new InputBuffer(this));
        IR_buffer_base_ptr_ = builder.createBufferInit(type_, "external_");
        is_initialized_ = true;
    }

    IR_buffer_ptr_ = builder.createPositionalInBoundsGep(IR_buffer_base_ptr_, builder.getCurrentOffsetValue(),
                                                             "offset_arg_incr_");
    IR_value_ = builder.createPositionalLoad(IR_buffer_ptr_, true, "arg_buffer_val_");

}