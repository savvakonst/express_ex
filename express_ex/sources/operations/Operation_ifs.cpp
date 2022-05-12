
#include "operations/Operation_ifs.h"

#include "jit/IR_generator.h"
#include "parser/operations.h"

void Operation_ifs::visitEnterStackUpdate(stack<Value*>* visitor_stack) { print_error("visitEnter unknown command"); }

void Operation_ifs::markUnusedVisitEnter(stack<Value*>* visitor_stack) {
    commonMarkUnusedVisitEnter(visitor_stack);
    visitEnterSetupBuffer(visitor_stack);
    visitEnterStackUpdate(visitor_stack);
    is_unused_ = false;
}

void Operation_ifs::finishSetupIR(IRGenerator& builder) {
    if (isBuffered() | isReturned()) {
        if (!is_initialized_) {
            if (isReturned()) builder.AddBufferAlloca(new OutputBuffer(this));
            else
                builder.AddBufferAlloca(new Buffer(this));

            IR_buffer_base_ptr_ = builder.CreateBufferInit(type_, "internal_");
            is_initialized_ = true;
        }
        builder.SetStoreInsertPoint();
        IR_buffer_ptr_ = builder.CreateInBoundsGEP(IR_buffer_base_ptr_, builder.getCurrentOffsetValue(), "offset_incr");
        builder.CreatePositionalStore(IR_value_, IR_buffer_ptr_);
    }
}
