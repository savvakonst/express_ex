//
// Created by SVK on 13.07.2022.
//

#include "CallRecursiveFunction.h"

#include "jit/IR_generator.h"
#include "jit/TableGenContext.h"

static ExValue_ifs* getTempVal(const stack<ExValue_ifs*>& args) {
    ExValue_ifs* temp = nullptr;
    for (auto i : args)
        if (i->isArray()) temp = i;

    if (temp == nullptr) print_error("cant construct TempVal");
    return temp;
}

CallRecursiveFunction::CallRecursiveFunction(Body* body, const stack<ExValue_ifs*>& args)
    : CallI_ifs(body->getRet().front()->type_, TypeEn::unknown_jty, getTempVal(args)) {
    body_ = body;
    args_ = args;

    if (body_->getRet().empty()) return;


    auto ret = body_->getRet().front();

    std::vector<ParameterIfs*> p_list;


    for (auto i : args_) {
        p_list.push_back(i->getAssignedVal(true)->getParameter());
    }

    ExValue_ifs* temp = nullptr;
    for (auto i : args_) {
        level_ = maxInt(level_, i->getLevel());
        if (i->isArray()) temp = i;
    }


    // if (temp == nullptr) print_error("CallRecursiveFunction::CallRecursiveFunction : temp == nullptr");


    // type_ = ret->getType();
    // length_ = temp->getLength();
    // ds_ty_ = temp->getDSType();



    for (auto i : args_)
        if (i->getLevel() < level_) i->getAssignedVal(true)->setBuffered();


    if (isConst(ret)) {
        binary_value_ = ret->getBinaryValue();
    }
}


void CallRecursiveFunction::reverseTraversalVisitEnter(stack<ExValue_ifs*>* visitor_stack) {
    commonMarkUnusedVisitEnter(visitor_stack);
    for (int64_t i = ((int64_t)args_.size() - 1); i >= 0; i--) {
        auto arg = args_[i];
        visitor_stack->push(arg);
        if (arg->isArray()) arg->setBufferBordersLength(this);
    }
    is_unused_ = false;
}

void CallRecursiveFunction::setupIR(IRGenerator& builder) {
    llvm::Function* function = body_->getOrGenIRPureFunction(builder);
    std::vector<llvm::Value*> arg_list;
    for (auto i : args_) {
        arg_list.push_back(i->getAssignedVal(true)->getIRValue(builder, level_));
    }
    IR_value_ = builder.CreateCall(function, arg_list, "call_" + body_->getName());

    if (isBuffered() | isReturned()) {  // replace to new function
        if (!is_initialized_) {
            BufferTypeEn bufferType = isReturned() ? BufferTypeEn::output : BufferTypeEn::internal;
            if (isReturned()) {
                builder.addBuffer(new OutputBuffer(this));
            } else builder.addBuffer(new Buffer(this));

            IR_buffer_base_ptr_ = builder.createBufferInit(type_, "internal_");
            is_initialized_ = true;
        }
        builder.setStoreInsertPoint();
        IR_buffer_ptr_ =
            builder.CreateInBoundsGEP(IR_buffer_base_ptr_, builder.getCurrentOffsetValue(), "offset_cr_incr_");
        builder.createPositionalStore(IR_value_, IR_buffer_ptr_);
        builder.setCalcInsertPoint();
    }
}

void CallRecursiveFunction::genBlocksVisitExit(TableGenContext* context) {
    unique_name_ = (isLargeArr(this) ? "Rec_fb" : "Rec_fs") + std::to_string(context->getUniqueIndex());
    context->setUint(this);
    is_visited_ = false;


    PrmTypesEn RPMType = JITType2PRMType(type_);

    for (auto i : args_)
        if (i->isArray()) {
            parameter_ = retyping(i->getAssignedVal(true)->getParameter(), RPMType, "");
            break;
        }

    context->setParameter(parameter_);
    if (isReturned()) {
        parameter_ = parameter_->newParameter();
        // parameter_ =new SyncParameter("", parameter_->getMainTimeInterval(), parameter_->getDataIntervalList(),
        // false);
        context->setParameter(parameter_);
    }
}

/*
 *
 *
 */

void TailCallDirective::genBlocksVisitExit(TableGenContext* context) {
    print_error("genBlocksVisitExit is not supported for TailCallDirective");
}

void TailCallDirective::setupIR(IRGenerator& builder) {
    size_t size = builder.arg_ptr_list_.size();
    for (size_t i = 0; i < size; i++) {
        auto ex_value = args_[i]->getAssignedVal(true);
        llvm::Value* arg = ex_value->getIRValue(builder, level_);
        builder.CreateStore(arg, builder.arg_ptr_list_[size - 1 - i]);
    }
}

void TailCallDirective::printVisitExit(PrintBodyContext* context) {
    std::string ret = "tailCall( " + printArgs(context) + ")";
    context->push(ret);
    is_visited_ = false;
}
