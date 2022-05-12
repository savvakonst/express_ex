#include "operations/ArithmeticOperation.h"

#include "jit/IR_generator.h"
#include "parser/operations.h"

static const std::string kArSym[14] = {"+", "+.", "-", "-.", "*", "*.", "/", "/", "/.", "%", "%", "%.", "**", "**."};
static std::string txtArOp(OpCodeEn op_code) { return kArSym[((int)op_code - (int)TypeOpCodeEn::arithetic)]; }

void ArithmeticOperation::visitEnterStackUpdate(stack<Value*>* visitor_stack) {
    visitor_stack->push(operand_[1]);
    visitor_stack->push(operand_[0]);
    Operation_ifs::visitEnterStackUpdate(visitor_stack);
}

void ArithmeticOperation::genBodyVisitExit(BodyGenContext* context) {
    is_visited_ = false;
    GarbageContainer* garbage_container = context->getGarbageContainer();

    g_pos = pos;

    auto op2 = context->pop();
    auto op1 = context->pop();

    if ((op2 == nullptr) || (op1 == nullptr)) {
        auto txtOperation = txtArOp(op_code_);
        print_error(txtOperation +
                    " "
                    "left operand:" +
                    ((op1 != nullptr) ? "a" : "unknown") + ".  " +
                    "right operand:" + ((op2 != nullptr) ? "a" : "unknown"));
    }
    TypeEn target_type = maxTypeVar(op1, op2)->getType();

    auto ret =
        newArithmeticOperation(garbage_container, target_type, newTypeConvOp(garbage_container, target_type, op1),
                               newTypeConvOp(garbage_container, target_type, op2), (OpCodeEn)(int)op_code_);
    context->push(ret);
}

void ArithmeticOperation::calculateConstRecursive(RecursiveGenContext* context) {
    auto op_a = operand_[0], op_b = operand_[1];

    temp_type_ = maxTempTypeVar(op_a, op_b)->getTempType();
    auto arg_a = calcTypeConvConst(temp_type_, op_a->getTempType(), op_a->getBinaryValue());
    auto arg_b = calcTypeConvConst(temp_type_, op_b->getTempType(), op_b->getBinaryValue());
    binary_value_ = calcArithmeticConst(op_code_, temp_type_, arg_a, arg_b);
}

void ArithmeticOperation::printVisitExit(PrintBodyContext* context) {
    is_visited_ = false;
    auto op2 = context->pop();
    auto op1 = context->pop();
    context->push(checkBuffer("(" + op1 + txtArOp(op_code_) + op2 + ")"));
}

void ArithmeticOperation::genBlocksVisitExit(TableGenContext* context) { /*TODO*/
}

std::string ArithmeticOperation::printUint() {
    is_visited_ = false;

    auto name_op_a = operand_[0]->getAssignedVal(true)->getUniqueName();
    auto name_op_b = operand_[1]->getAssignedVal(true)->getUniqueName();

    return getUniqueName() + " = " + name_op_a + txtArOp(op_code_) + name_op_b;
}

void ArithmeticOperation::setupIR(IRGenerator& builder) {
    auto ir_op_a = operand_[0]->getAssignedVal(true)->getIRValue(builder, level_);
    auto ir_op_b = operand_[1]->getAssignedVal(true)->getIRValue(builder, level_);

    IR_value_ = builder.CreateArithmetic(ir_op_a, ir_op_b, op_code_, getUniqueName());

    finishSetupIR(builder);
}

void ArithmeticOperation::calculate() {
    int length = (int)length_;

    auto op_a = operand_[0]->getAssignedVal(true);
    auto op_b = operand_[1]->getAssignedVal(true);

    if (op_a->isArray() && op_b->isArray())
        buffer_ptr_ =
            calcArithmeticSmallArray(op_code_, type_, buffer_ptr_, op_a->getBufferPtr(), op_b->getBufferPtr(), length);
    else if (op_a->isArray())
        buffer_ptr_ = calcArithmeticSmallArray(op_code_, type_, buffer_ptr_, op_a->getBufferPtr(),
                                               op_b->getBinaryValue(), length);
    else
        buffer_ptr_ = calcArithmeticSmallArray(op_code_, type_, buffer_ptr_, op_a->getBinaryValue(),
                                               op_b->getBufferPtr(), length);
}