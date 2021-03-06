
#include "operations/Operation_ifs.h"

#include "jit/IR_generator.h"
#include "jit/TableGenContext.h"

void Operation_ifs::visitEnterStackUpdate(stack<ExValue_ifs*>* visitor_stack) {
    print_error("visitEnter unknown command");
}

void Operation_ifs::reverseTraversalVisitEnter(stack<ExValue_ifs*>* visitor_stack) {
    commonMarkUnusedVisitEnter(visitor_stack);
    visitEnterSetupBuffer(visitor_stack);
    visitEnterStackUpdate(visitor_stack);
    for (auto i : operand_)
        if (i->getLevel() < level_) i->getAssignedVal(true)->setBuffered();

    is_unused_ = false;
}

void Operation_ifs::genBlocksVisitExit(TableGenContext* context) {
    is_visited_ = false;

    unique_name_ = (isLargeArr(this) ? "vb" : "vs") + std::to_string(context->getUniqueIndex());
    context->setUint(this);

    PrmTypesEn prm_type = toPrmType(type_);

    std::vector<ParameterIfs*> p_list;
    for (auto i : operand_) p_list.push_back(i->getAssignedVal(true)->getParameter());

    parameter_ = intersection(p_list, prm_type, "");

    context->setParameter(parameter_);
    if (isReturned()) {
        parameter_ = parameter_->newParameter();
        context->setParameter(parameter_);
    }

    if (parameter_ != nullptr) context->setParameter(parameter_);
}

void Operation_ifs::finishSetupIR(IRGenerator& builder) {
    if (isBuffered() || isReturned()) {
        if (!is_initialized_) {
            if (isReturned()) builder.addBuffer(new OutputBuffer(this));
            else builder.addBuffer(new Buffer(this));

            IR_buffer_base_ptr_ = builder.createBufferInit(type_, "internal_");
            is_initialized_ = true;
        }


        auto last_block = builder.GetInsertBlock();

        if (last_insert_block_ == last_block) return;

        last_insert_block_ = last_block;

        builder.setLoadInsertPoint();
        IR_buffer_ptr_ =
            builder.CreateInBoundsGEP(IR_buffer_base_ptr_, builder.getCurrentOffsetValue(), "offset_op_incr_");



        if (IR_value_) builder.createPositionalStore(IR_value_, IR_buffer_ptr_);

        builder.SetInsertPoint(last_block);
    }
}
