
#include "operations/ComparisonOperation.h"

#include "jit/IR_generator.h"
#include "parser/operations.h"

static const std::string kArComp[17] = {
    "==", "!=", " ugt ", " uge ", " ult ", " ule ",  ">",  ">=", " less ",
    "<=", "==", "!=",    ">",     ">=",    " less ", "<=", "_",
};
static std::string txtCompOp(OpCodeEn op_code) { return kArComp[((int)op_code - (int)TypeOpCodeEn::comparsion)]; }

void ComparisonOperation::visitEnterStackUpdate(stack<Value*>* visitor_stack) {
    visitor_stack->push(operand_[1]);
    visitor_stack->push(operand_[0]);
}

void ComparisonOperation::genBodyVisitExit(BodyGenContext* context) {
    GarbageContainer* garbage_container = context->getGarbageContainer();
    g_pos                               = pos;

    is_visited_ = false;

    auto op2 = context->pop();
    auto op1 = context->pop();

    if ((op2 == nullptr) || (op1 == nullptr)) {
        auto txtOperation = txtCompOp(op_code_);
        print_error(txtOperation +
                    " "
                    "left operand:" +
                    ((op1 != nullptr) ? "a" : "unknown") + ".  " +
                    "right operand:" + ((op2 != nullptr) ? "a" : "unknown"));
    }
    TypeEn target_type = maxTypeVar(op1, op2)->getType();

    auto ret =
        newComparisonOperation(garbage_container, target_type, newTypeConvOp(garbage_container, target_type, op1),
                               newTypeConvOp(garbage_container, target_type, op2), (OpCodeEn)(int)op_code_);

    context->push(ret);
}

void ComparisonOperation::calculateConstRecursive(RecursiveGenContext* context) {
    auto op_a = operand_[0];
    auto op_b = operand_[1];

    auto local_type        = maxTempTypeVar(op_a, op_b)->getTempType();
    auto arg_a             = calcTypeConvConst(local_type, op_a->getTempType(), op_a->getBinaryValue());
    auto arg_b             = calcTypeConvConst(local_type, op_b->getTempType(), op_b->getBinaryValue());
    untyped_t binary_value = calcComparisonConst(op_code_, local_type, arg_a, arg_b);
    binary_value_          = binary_value;
    temp_type_             = TypeEn::int1_jty;
}

void ComparisonOperation::printVisitExit(PrintBodyContext* context) {
    is_visited_ = false;

    auto op2 = context->pop();
    auto op1 = context->pop();
    context->push(checkBuffer("(" + op1 + txtCompOp(op_code_) + op2 + ")"));

    Operation_ifs::printVisitExit(context);
}

void ComparisonOperation::genBlocksVisitExit(TableGenContext* context) { /*TODO:*/
}

std::string ComparisonOperation::printUint() {
    is_visited_ = false;

    auto name_op_a = operand_[0]->getAssignedVal(true)->getUniqueName();
    auto name_op_b = operand_[0]->getAssignedVal(true)->getUniqueName();

    return getUniqueName() + " = " + name_op_a + txtCompOp(op_code_) + name_op_b;
}

void ComparisonOperation::setupIR(IRGenerator& builder) {
    auto ir_op_a = operand_[0]->getAssignedVal(true)->getIRValue(builder, level_);
    auto ir_op_b = operand_[1]->getAssignedVal(true)->getIRValue(builder, level_);

    IR_value_ = builder.CreateComparsion(ir_op_a, ir_op_b, op_code_, getUniqueName());

    finishSetupIR(builder);
}

void ComparisonOperation::calculate() {
    int length = (int)length_;

    auto op_a = operand_[0]->getAssignedVal(true);
    auto op_b = operand_[1]->getAssignedVal(true);

    auto local_type = op_a->getType();

    if (op_a->isArray() && op_b->isArray())
        buffer_ptr_ = calcComparisonSmallArray(op_code_, local_type, buffer_ptr_, op_a->getBufferPtr(),
                                               op_b->getBufferPtr(), length);
    else if (op_a->isArray())
        buffer_ptr_ = calcComparisonSmallArray(op_code_, local_type, buffer_ptr_, op_a->getBufferPtr(),
                                               op_b->getBinaryValue(), length);
    else
        buffer_ptr_ = calcComparisonSmallArray(op_code_, local_type, buffer_ptr_, op_a->getBinaryValue(),
                                               op_b->getBufferPtr(), length);
}
