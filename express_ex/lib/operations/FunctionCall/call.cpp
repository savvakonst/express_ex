#include "call.h"

#include "jit/IR_generator.h"
#include "jit/TableGenContext.h"

/// //////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// //////////////////////////////////////////////////////////////////////////////////////////////////

Call::Call(Body* body, const stack<ExValue_ifs*>& args)
    : CallI_ifs(body->getRet().front()->type_, TypeEn::unknown_jty, body->getRet().front()) {
    body_ = body;
    args_ = args;

    if (body_->getRet().empty()) return;
    auto ret = body_->getRet().front();

    level_ = ret->getLevel();
    // type_ = ret->getType();
    // ds_ty_ = ret->getDSType();
    // length_ = ret->getLength();

    if (isConst(ret)) {
        binary_value_ = ret->getBinaryValue();
    }
}

void Call::reverseTraversalVisitEnter(stack<ExValue_ifs*>* visitor_stack) {
    commonMarkUnusedVisitEnter(visitor_stack);

    if (body_->getRet().empty()) print_error("reverseTraversalVisitEnter: body_->getRet().empty() == true ");

    auto ret = body_->getRet().front();

    visitor_stack->push(ret);
    ret->setBufferBordersLength(this);

    is_unused_ = false;
}

void Call::genBlocksVisitExit(TableGenContext* context) {
    body_->genTable(context);
    unique_name_ = (isLargeArr(this) ? "fb" : "fs") + std::to_string(context->getUniqueIndex());
    context->setUint(this);
    is_visited_ = false;
}

void Call::setupIR(IRGenerator& builder) {
    if (builder.is_pure_function_) {
        llvm::Function* function = body_->getOrGenIRPureFunction(builder);
        std::vector<llvm::Value*> arg_list;
        for (auto i : args_) {
            arg_list.push_back(i->getAssignedVal(true)->getIRValue(builder, level_));
        }
        IR_value_ = builder.CreateCall(function, arg_list, "call_" + body_->getName());
    }
}
