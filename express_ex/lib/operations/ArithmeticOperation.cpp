#include "operations/ArithmeticOperation.h"

#include "jit/IR_generator.h"
#include "operations/TypeCastOperation.h"
#include "parser/bodyTemplate.h"


static const std::string kArSym[14] = {"+", "+.", "-", "-.", "*", "*.", "/", "/", "/.", "%", "%", "%.", "**", "**."};
static std::string txtArOp(OpCodeEn op_code) { return kArSym[((int)op_code - (int)TypeOpCodeEn::arithetic)]; }

ExValue* newArithmeticOperation(GarbageContainer* garbage_container, TypeEn target_type, ExValue* arg_a, ExValue* arg_b,
                                OpCodeEn op_type) {
    if (!isCompatible(arg_a, arg_b)) print_error("incompatible values");

    if (isConst(arg_a) && isConst(arg_b) && !isUnknownTy(target_type)) {
        if (isBool(target_type)) print_error("invalid type: Int1_jty ");

        return garbage_container->add(new ExValue(
            calcArithmeticConst(op_type, target_type, arg_a->getBinaryValue(), arg_b->getBinaryValue()), target_type));
    }

    auto local_op_type = OpCodeEn::none_op;

    if (isInteger(target_type) || isUnknownTy(target_type)) {
        local_op_type = op_type;
    } else if (isFloating(target_type)) {
        switch (op_type) {
        case (OpCodeEn::add):
            local_op_type = OpCodeEn::fadd;
            break;
        case (OpCodeEn::sub):
            local_op_type = OpCodeEn::fsub;
            break;
        case (OpCodeEn::mul):
            local_op_type = OpCodeEn::fmul;
            break;
        case (OpCodeEn::sdiv):
            local_op_type = OpCodeEn::fdiv;
            break;
        case (OpCodeEn::srem):
            local_op_type = OpCodeEn::frem;
            break;
        case (OpCodeEn::pow):
            local_op_type = OpCodeEn::fpow;
            break;
        default:;
        }
    } else {
        print_error("newArithmeticOperation - unsigned is not supported yet");
    }

    return garbage_container->add(new ArithmeticOperation(local_op_type, arg_a, arg_b));
}

ExValue* newArithmeticOperation(BodyTemplate* body_template, OpCodeEn u_type_op) {
    auto* arg_b = body_template->pop();
    auto* arg_a = body_template->pop();
    return newArithmeticOperation(body_template->getGarbageContainer(), TypeEn::DEFAULT_JTY, arg_a, arg_b, u_type_op);
}

ExValue* newInversionOperation(BodyTemplate* body_template) {
    ExValue* arg = body_template->pop();
    ExValue* zero = body_template->getGarbageContainer()->add(new ExValue("0", TypeEn::int32_jty));
    return newArithmeticOperation(body_template->getGarbageContainer(), TypeEn::DEFAULT_JTY, zero, arg, OpCodeEn::sub);
}

ArithmeticOperation::ArithmeticOperation(OpCodeEn op, ExValue* var_a, ExValue* var_b) : Operation_ifs() {
    commonSetup(op, maxDSVar(var_a, var_b));

    type_ = maxTypeVar(var_a, var_b)->getType();
    type_ = isComparison(op) && !isUnknownTy(type_) ? TypeEn::int1_jty : type_;

    level_ = maxLevelVar(var_a, var_b)->getLevel();

    operand_.push_back(var_a);
    operand_.push_back(var_b);

    for (auto i : operand_)
        if (i->getLevel() < level_) i->getAssignedVal(true)->setBuffered();
}

void ArithmeticOperation::visitEnterStackUpdate(stack<ExValue*>* visitor_stack) {
    visitor_stack->push(operand_[1]);
    visitor_stack->push(operand_[0]);
}

void ArithmeticOperation::genBodyVisitExit(BodyGenContext* context) {
    is_visited_ = false;
    GarbageContainer* garbage_container = context->getGarbageContainer();

    g_pos = pos_;

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

std::string ArithmeticOperation::printUint() {
    is_visited_ = false;

    auto name_op_a = operand_[0]->getAssignedVal(true)->getUniqueName();
    auto name_op_b = operand_[1]->getAssignedVal(true)->getUniqueName();

    return getUniqueName() + " = " + name_op_a + txtArOp(op_code_) + name_op_b;
}

void ArithmeticOperation::setupIR(IRGenerator& builder) {
    auto ir_op_a = operand_[0]->getAssignedVal(true)->getIRValue(builder, level_);
    auto ir_op_b = operand_[1]->getAssignedVal(true)->getIRValue(builder, level_);

    IR_value_ = builder.createArithmetic(ir_op_a, ir_op_b, op_code_, getUniqueName());

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
