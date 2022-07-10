#include "operations/SelectOperation.h"

#include "jit/IR_generator.h"
#include "operations/TypeCastOperation.h"

ExValue* newSelectOp(GarbageContainer* garbage_container, TypeEn target_type, ExValue* arg_a, ExValue* arg_b,
                     ExValue* arg_c, bool rec_call) {
    if (!isCompatible(arg_b, arg_c) || !isCompatible(arg_a, arg_b)) print_error("incompatible values");

    auto i1 = newTypeConvOp(garbage_container, TypeEn::int1_jty, arg_a);

    if (isConst(i1) && !isUnknownTy(i1) && !isUnknownTy(target_type)) {
        if (i1->getConvTypeVal<bool>()) return arg_b;
        return arg_c;
    }

    return garbage_container->add(new SelectOperation(OpCodeEn::select, i1, arg_b, arg_c, target_type, rec_call));
}

SelectOperation::SelectOperation(OpCodeEn op, ExValue* var_a, ExValue* var_b, ExValue* var_c, TypeEn target_type,
                                 bool rec_call)
    : Operation_ifs() {
    commonSetup(op, maxDSVar(var_a, var_b));
    type_ = target_type;
    level_ = maxLevelVar(maxLevelVar(var_a, var_b), var_c)->getLevel();
    contain_rec_call_ = rec_call;

    operand_.push_back(var_a);

    if (rec_call && !isUnknownTy(type_))  // it is a dirty hack
        if (var_b->getAssignedVal(true)->getNodeType() == NodeTypeEn::kTailCall) type_ = var_c->getType();
        else type_ = var_b->getType();

    operand_.push_back(var_b);
    operand_.push_back(var_c);

    for (auto i : operand_)
        if (i->getLevel() < level_) i->getAssignedVal(true)->setBuffered();
}

void SelectOperation::visitEnterStackUpdate(stack<ExValue*>* visitor_stack) {
    visitor_stack->push(operand_[2]);
    visitor_stack->push(operand_[1]);
    visitor_stack->push(operand_[0]);
}

void SelectOperation::genBodyVisitExit(BodyGenContext* context) {
    is_visited_ = false;
    GarbageContainer* garbage_container = context->getGarbageContainer();
    g_pos = pos;

    auto op3 = context->pop();
    auto op2 = context->pop();
    auto op1 = context->pop();

    TypeEn target_type;
    if (contain_rec_call_) {
        target_type = (operand_[1]->getAssignedVal(true)->getNodeType() == NodeTypeEn::kTailCall) ? op3->getType()
                                                                                                  : op2->getType();
    } else {
        target_type = maxTypeVar(op2, op3)->getType();
        op2 = newTypeConvOp(garbage_container, target_type, op2);
        op3 = newTypeConvOp(garbage_container, target_type, op3);
    }

    auto ret = newSelectOp(garbage_container, target_type, op1, op2, op3, contain_rec_call_);
    context->push(ret);
}

void SelectOperation::calculateConstRecursive(RecursiveGenContext* context) {
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

void SelectOperation::printVisitExit(PrintBodyContext* context) {
    is_visited_ = false;

    auto op3 = context->pop();
    auto op2 = context->pop();
    auto op1 = context->pop();
    context->push(checkBuffer("(" + op1 + "? " + op2 + ": " + op3 + ")"));
}

std::string SelectOperation::printUint() {
    is_visited_ = false;

    auto txt_op_a = operand_[0]->getAssignedVal(true)->getUniqueName();
    auto txt_op_b = operand_[1]->getAssignedVal(true)->getUniqueName();
    auto txt_op_c = operand_[2]->getAssignedVal(true)->getUniqueName();

    return getUniqueName() + " = " + txt_op_a + "? " + txt_op_b + ": " + txt_op_c;
}

void SelectOperation::setupIR(IRGenerator& builder) {
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

void SelectOperation::calculate() {
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
