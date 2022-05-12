#include "operations/TypeCastOperation.h"

#include "jit/IR_generator.h"
#include "parser/operations.h"

static const std::string ar_t_conv_[10] = {"trunc", "zext",   "sext",   "fptrunc", "fpext",
                                           "fptoi", "fptosi", "uitofp", "sitofp",  "common_cast"};
static std::string txtTypeCastOp(OpCodeEn op_code) { return ar_t_conv_[((int)op_code - (int)TypeOpCodeEn::type_conv)]; }

void TypeCastOperation::visitEnterStackUpdate(stack<Value*>* visitor_stack) { visitor_stack->push(operand_[0]); }

void TypeCastOperation::genBodyVisitExit(BodyGenContext* context) {
    is_visited_ = false;
    GarbageContainer* garbage_container = context->getGarbageContainer();
    g_pos = pos;

    auto ret = newTypeConvOp(garbage_container, type_, context->pop());

    context->push(ret);
}

void TypeCastOperation::calculateConstRecursive(RecursiveGenContext* context) {
    auto op_a = operand_[0];
    temp_type_ = type_;
    binary_value_ = calcTypeConvConst(temp_type_, op_a->getTempType(), op_a->getBinaryValue());
}

void TypeCastOperation::printVisitExit(PrintBodyContext* context) {
    is_visited_ = false;
    context->push(checkBuffer(txtTypeCastOp(op_code_) + "(" + context->pop() + ")"));
}

void TypeCastOperation::genBlocksVisitExit(TableGenContext* context) { /*TODO*/
}

std::string TypeCastOperation::printUint() {
    is_visited_ = false;

    auto name_op_a = operand_[0]->getAssignedVal(true)->getUniqueName();

    return getUniqueName() + " = " + txtTypeCastOp(op_code_) + "( " + name_op_a + ")";
}

void TypeCastOperation::setupIR(IRGenerator& builder) {
    auto ir_op_a = operand_[0]->getAssignedVal(true)->getIRValue(builder, level_);

    IR_value_ = builder.CreateTypeConv(ir_op_a, op_code_, type_, getUniqueName());

    finishSetupIR(builder);
}

void TypeCastOperation::calculate() {
    int length = (int)length_;
    auto op_a = operand_[0]->getAssignedVal(true);
    buffer_ptr_ = calcTypeConvSmallArray(type_, op_a->getType(), buffer_ptr_, op_a->getBufferPtr(), length);
}
