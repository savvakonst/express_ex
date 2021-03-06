#include "ComparisonOperation.h"

#include "TypeCastOperation.h"
#include "jit/IR_generator.h"
#include "operations/ExConstValue.h"
#include "parser/bodyTemplate.h"


static std::string txtCompOp(OpCodeEn op_code) { return kOpCodesStr[(int)op_code]; }

ExValue_ifs* newComparisonOperation(GarbageContainer* garbage_container, TypeEn target_type, ExValue_ifs* arg_a,
                                    ExValue_ifs* arg_b, OpCodeEn op_type) {
    if (!isCompatible(arg_a, arg_b)) print_error("incompatible values");

    if (isConst(arg_a) && isConst(arg_b) && !isUnknownTy(target_type)) {
        if (isBool(target_type)) print_error("invalid type: Int1_jty ");

        return garbage_container->add(new ExConstValue(
            calcComparisonConst(op_type, target_type, arg_a->getBinaryValue(), arg_b->getBinaryValue()), TypeEn::i1));
    }

    OpCodeEn local_op_type = OpCodeEn::none_op;

    if (isInteger(target_type) || isUnknownTy(target_type)) {
        local_op_type = op_type;
    } else if (isFloating(target_type)) {
        switch (op_type) {
        case (OpCodeEn::eq):
            local_op_type = (OpCodeEn::oeq);
            break;
        case (OpCodeEn::ne):
            local_op_type = (OpCodeEn::one);
            break;
        case (OpCodeEn::sgt):
            local_op_type = (OpCodeEn::ogt);
            break;
        case (OpCodeEn::sge):
            local_op_type = (OpCodeEn::oge);
            break;
        case (OpCodeEn::slt):
            local_op_type = (OpCodeEn::olt);
            break;
        case (OpCodeEn::sle):
            local_op_type = (OpCodeEn::ole);
            break;
        default:;
        }
    } else {
        print_error("newComparisonOperation - unsigned is not supported yet");
    }

    return garbage_container->add(new ComparisonOperation(local_op_type, arg_a, arg_b));
}

ExValue_ifs* newComparisonOperation(BodyTemplate* body_template, OpCodeEn op_type) {
    ExValue_ifs* arg_b = body_template->pop();
    ExValue_ifs* arg_a = body_template->pop();
    return newComparisonOperation(body_template->getGarbageContainer(), TypeEn::unknown, arg_a, arg_b, op_type);
}

ComparisonOperation::ComparisonOperation(OpCodeEn op, ExValue_ifs* var_a, ExValue_ifs* var_b)
    : Operation_ifs((isUnknownTy(var_a) || isUnknownTy(var_b) ? TypeEn::unknown : TypeEn::i1),  //
                    TypeEn::unknown,                                                            //
                    maxDataStructType(var_a, var_b),                                            //
                    maxLength(var_a, var_b),                                                    //
                    op)                                                                         //
{
    // TODO remove comment
    //  commonSetup(op, maxDSVar(var_a, var_b));
    //  type_ = isUnknownTy(var_a) || isUnknownTy(var_b) ? TypeEn::unknown : TypeEn::i1;

    level_ = maxLevelVar(var_a, var_b)->getLevel();

    operand_.push_back(var_a);
    operand_.push_back(var_b);

    for (auto i : operand_)
        if (i->getLevel() < level_) i->getAssignedVal(true)->setBuffered();
}

void ComparisonOperation::visitEnterStackUpdate(stack<ExValue_ifs*>* visitor_stack) {
    visitor_stack->push(operand_[1]);
    visitor_stack->push(operand_[0]);
}

void ComparisonOperation::genBodyVisitExit(BodyGenContext* context) {
    is_visited_ = false;
    GarbageContainer* garbage_container = context->getGarbageContainer();
    g_pos = pos_;

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
    TypeEn target_type = maxTypeVar(op1, op2)->type_;

    auto ret =
        newComparisonOperation(garbage_container, target_type, newTypeConvOp(garbage_container, target_type, op1),
                               newTypeConvOp(garbage_container, target_type, op2), op_code_);

    context->push(ret);
}

void ComparisonOperation::calculateConstRecursive(RecursiveGenContext* context) {
    auto op_a = operand_[0];
    auto op_b = operand_[1];

    auto local_type = maxTempTypeVar(op_a, op_b)->getTempType();
    auto arg_a = calcTypeConvConst(local_type, op_a->getTempType(), op_a->getBinaryValue());
    auto arg_b = calcTypeConvConst(local_type, op_b->getTempType(), op_b->getBinaryValue());
    untyped_t binary_value = calcComparisonConst(op_code_, local_type, arg_a, arg_b);
    binary_value_ = binary_value;
    temp_type_ = TypeEn::i1;
}

void ComparisonOperation::printVisitExit(PrintBodyContext* context) {
    is_visited_ = false;

    auto op2 = context->pop();
    auto op1 = context->pop();
    context->push(checkBuffer("(" + op1 + txtCompOp(op_code_) + op2 + ")"));
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

    IR_value_ = builder.createComparison(ir_op_a, ir_op_b, op_code_, getUniqueName());

    finishSetupIR(builder);
}

void ComparisonOperation::calculate() {
    int length = (int)length_;

    auto op_a = operand_[0]->getAssignedVal(true);
    auto op_b = operand_[1]->getAssignedVal(true);

    auto local_type = op_a->type_;

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
