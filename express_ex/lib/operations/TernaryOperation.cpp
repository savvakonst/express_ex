#include "TernaryOperation.h"

#include "TypeCastOperation.h"
#include "jit/IR_generator.h"
#include "operations/ExConstValue.h"
#include "parser/bodyTemplate.h"

ExValue_ifs* newTernaryOperation(GarbageContainer* garbage_container, TypeEn target_type, ExValue_ifs* arg_a,
                                 ExValue_ifs* arg_b, ExValue_ifs* arg_c, bool rec_call) {
    if (!isCompatible(arg_b, arg_c) || !isCompatible(arg_a, arg_b)) print_error("incompatible values");

    auto i1 = newTypeConvOp(garbage_container, TypeEn::int1_jty, arg_a);

    if (isConst(i1) && !isUnknownTy(i1) && !isUnknownTy(target_type)) {
        auto const_val = (ExConstValue*)i1;
        if (const_val->getConvTypeVal<bool>()) return arg_b;
        return arg_c;
    }

    if (rec_call && !isUnknownTy(target_type)) {  // it is a dirty hack
        if (arg_b->getAssignedVal(true)->getNodeType() == NodeTypeEn::kTailCall) target_type = arg_c->type_;
        else {
            target_type = arg_b->type_;
        }
    }

    return garbage_container->add(new TernaryOperation(OpCodeEn::select, i1, arg_b, arg_c, target_type, rec_call));
}

ExValue_ifs* newTernaryOperation(BodyTemplate* body_template) {
    ExValue_ifs* arg_c = body_template->pop();
    ExValue_ifs* arg_b = body_template->pop();
    ExValue_ifs* arg_a = body_template->pop();

    bool valid_recursion = false;

    if (body_template->is_tail_callable_) {
        const NodeTypeEn p = arg_c->getAssignedVal(true)->getNodeType();
        valid_recursion = (p == NodeTypeEn::kTailCall);
        valid_recursion = valid_recursion || (arg_b->getAssignedVal(true)->getNodeType() == NodeTypeEn::kTailCall);
    }

    return newTernaryOperation(body_template->getGarbageContainer(), TypeEn::unknown_jty, arg_a, arg_b, arg_c,
                               valid_recursion);
}

TernaryOperation::TernaryOperation(OpCodeEn op, ExValue_ifs* var_a, ExValue_ifs* var_b, ExValue_ifs* var_c,
                                   TypeEn target_type, bool rec_call)
    : Operation_ifs(target_type, TypeEn::unknown_jty, maxDataStructType(var_a, var_b),
                    std::max(var_c->getLength(), maxLength(var_a, var_b)), op) {
    level_ = maxLevelVar(maxLevelVar(var_a, var_b), var_c)->getLevel();
    contain_rec_call_ = rec_call;

    operand_.push_back(var_a);
    operand_.push_back(var_b);
    operand_.push_back(var_c);

    for (auto i : operand_)
        if (i->getLevel() < level_) i->getAssignedVal(true)->setBuffered();
}

void TernaryOperation::visitEnterStackUpdate(stack<ExValue_ifs*>* visitor_stack) {
    visitor_stack->push(operand_[2]);
    visitor_stack->push(operand_[1]);
    visitor_stack->push(operand_[0]);
}

void TernaryOperation::genBodyVisitExit(BodyGenContext* context) {
    is_visited_ = false;
    GarbageContainer* garbage_container = context->getGarbageContainer();
    g_pos = pos_;

    auto op3 = context->pop();
    auto op2 = context->pop();
    auto op1 = context->pop();

    TypeEn target_type;
    if (contain_rec_call_) {
        target_type =
            (operand_[1]->getAssignedVal(true)->getNodeType() == NodeTypeEn::kTailCall) ? op3->type_ : op2->type_;
    } else {
        target_type = maxTypeVar(op2, op3)->type_;
        op2 = newTypeConvOp(garbage_container, target_type, op2);
        op3 = newTypeConvOp(garbage_container, target_type, op3);
    }

    auto ret = newTernaryOperation(garbage_container, target_type, op1, op2, op3, contain_rec_call_);
    context->push(ret);
}

void TernaryOperation::calculateConstRecursive(RecursiveGenContext* context) {
    auto op_a = operand_[0], op_b = operand_[1], op_c = operand_[2];

    auto arg_a = calcTypeConvConst(TypeEn::int1_jty, op_a->getTempType(), op_a->getBinaryValue());
    if (contain_rec_call_) {
        bool cond = (*((bool*)&arg_a));
        context->exit_from_loop_ = cond != (op_b->getNodeType() == NodeTypeEn::kTailCall);
        auto active_operand = (op_b->getNodeType() != NodeTypeEn::kTailCall) ? op_b : op_c;
        binary_value_ = active_operand->getBinaryValue();
        temp_type_ = active_operand->getTempType();
    } else {
        temp_type_ = maxTempTypeVar(op_b, op_c)->getTempType();
        auto arg_b = calcTypeConvConst(temp_type_, op_b->getTempType(), op_b->getBinaryValue());
        auto arg_c = calcTypeConvConst(temp_type_, op_c->getTempType(), op_c->getBinaryValue());
        binary_value_ = calcSelectConst(op_code_, temp_type_, arg_a, arg_b, arg_c);
    }
}

void TernaryOperation::printVisitExit(PrintBodyContext* context) {
    is_visited_ = false;

    auto op3 = context->pop();
    auto op2 = context->pop();
    auto op1 = context->pop();
    context->push(checkBuffer("(" + op1 + "? " + op2 + ": " + op3 + ")"));
}

std::string TernaryOperation::printUint() {
    is_visited_ = false;

    auto txt_op_a = operand_[0]->getAssignedVal(true)->getUniqueName();
    auto txt_op_b = operand_[1]->getAssignedVal(true)->getUniqueName();
    auto txt_op_c = operand_[2]->getAssignedVal(true)->getUniqueName();

    return getUniqueName() + " = " + txt_op_a + "? " + txt_op_b + ": " + txt_op_c;
}

void TernaryOperation::setupIR(IRGenerator& builder) {
    auto ir_op_a = operand_[0]->getAssignedVal(true)->getIRValue(builder, level_);
    auto ir_op_b = operand_[1]->getAssignedVal(true)->getIRValue(builder, level_);
    auto ir_op_c = operand_[2]->getAssignedVal(true)->getIRValue(builder, level_);

    if (contain_rec_call_) {
        llvm::Value* ret_val = nullptr;
        if (operand_[1]->getAssignedVal(true)->getNodeType() == NodeTypeEn::kTailCall) {
            builder.CreateCondBr(ir_op_a, builder.getCurrentBlock(), builder.getExitBlock());
            ret_val = ir_op_c;
        } else {
            builder.CreateCondBr(ir_op_a, builder.getExitBlock(), builder.getCurrentBlock());
            ret_val = ir_op_b;
        }
        builder.setExitInsertPoint();
        builder.CreateRet(ret_val);
        builder.setCalcInsertPoint();
    } else IR_value_ = builder.CreateSelect(ir_op_a, ir_op_b, ir_op_c, getUniqueName());

    finishSetupIR(builder);
}

void TernaryOperation::calculate() {
    int length = (int)length_;

    auto op_a = operand_[0]->getAssignedVal(true);
    auto op_b = operand_[1]->getAssignedVal(true);
    auto op_c = operand_[2]->getAssignedVal(true);

    if (op_b->isArray() && op_c->isArray())
        buffer_ptr_ = calcSelectSmallArray(op_code_, type_, buffer_ptr_, op_a->getBufferPtr(), op_b->getBufferPtr(),
                                           op_c->getBufferPtr(), length);
    else if (op_b->isArray())
        buffer_ptr_ = calcSelectSmallArray(op_code_, type_, buffer_ptr_, op_a->getBufferPtr(), op_b->getBufferPtr(),
                                           op_c->getBinaryValue(), length);
    else if (op_c->isArray())
        buffer_ptr_ = calcSelectSmallArray(op_code_, type_, buffer_ptr_, op_a->getBufferPtr(), op_b->getBinaryValue(),
                                           op_c->getBufferPtr(), length);
    else
        buffer_ptr_ = calcSelectSmallArray(op_code_, type_, buffer_ptr_, op_a->getBufferPtr(), op_b->getBinaryValue(),
                                           op_c->getBinaryValue(), length);
}
