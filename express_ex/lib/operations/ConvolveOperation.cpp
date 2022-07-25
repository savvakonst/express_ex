#include "ConvolveOperation.h"

#include "ArithmeticOperation.h"
#include "TypeCastOperation.h"
#include "jit/IR_generator.h"
#include "parser/bodyTemplate.h"

ExValue_ifs* newConvolveOperation(GarbageContainer* garbage_container, TypeEn target_type, ExValue_ifs* arg_a,
                                  ExValue_ifs* arg_b, int64_t shift, OpCodeEn op_type) {
    op_type = OpCodeEn::convolve;


    if ((isConst(arg_a) || isConst(arg_b)) && (!isUnknownTy(arg_a) && !isUnknownTy(arg_b))) {
        return garbage_container->add(
            newArithmeticOperation(garbage_container, target_type, arg_a, arg_b, OpCodeEn::mul));
    }
    if (isSmallArr(arg_a) && isSmallArr(arg_b)) {
        return garbage_container->add(new ConvolveOperation(arg_a, arg_b, shift));
        // print_error("convolve(SmallArr_t,SmallArr_t) - is not supported yet");
    }
    if (isSmallArr(arg_a) != isSmallArr(arg_b)) {
        if (isSmallArr(arg_b)) return garbage_container->add(new ConvolveOperation(arg_a, arg_b, shift));
        return garbage_container->add(new ConvolveOperation(arg_b, arg_a, shift));
    }
    if (isLargeArr(arg_a) && isLargeArr(arg_b)) {
        print_error("convolve(LargeArr_t,LargeArr_t) - is not supported");
    }

    print_error("convolve(xxx,xxx) - unknown case");
    return nullptr;
}

ExValue_ifs* newConvolveOperation(BodyTemplate* body_template, OpCodeEn u_type_op,
                                  uint32_t shift) {  // TODO: add type matching
    ExValue_ifs* arg_b = body_template->pop();
    ExValue_ifs* arg_a = body_template->pop();
    body_template->is_operator_ = true;
    return newConvolveOperation(body_template->getGarbageContainer(), TypeEn::unknown_jty, arg_a, arg_b, shift,
                                u_type_op);
}


ConvolveOperation::ConvolveOperation(ExValue_ifs* large_arr, ExValue_ifs* small_arr, int64_t shift)
    : Operation_ifs(
          large_arr->type_, TypeEn::unknown_jty, large_arr->ds_ty_,
          isLargeArr(large_arr) ? large_arr->getLength() : maxInt(large_arr->getLength(), small_arr->getLength()),
          OpCodeEn::convolve) {
    // TODO remove comment
    //  commonSetup(OpCodeEn::convolve, maxDSVar(large_arr, small_arr));
    //  type_ = large_arr->getType();
    //   if (ds_ty_ == DataStructureTypeEn::kLargeArr) length_ = large_arr->getLength();
    //   else length_ = maxInt(large_arr->getLength(), small_arr->getLength());
    //  length_ = isLargeArr(large_arr) ? large_arr->getLength() : maxInt(large_arr->getLength(),
    //  small_arr->getLength());


    if (large_arr->ds_ty_ < small_arr->ds_ty_)
        print_error(
            "ConvolveOperation::ConvolveOperation:  you nust ensure that the ds_ty_ of large_arr is more "
            "or equal small_arr ds_ty_. \n"
            "Please let the developers know about this issue.");

    level_ = large_arr->getLevel() + 1;

    shift_parameter_ = shift;

    operand_.push_back(large_arr);
    operand_.push_back(small_arr);
}

void ConvolveOperation::visitEnterSetupBuffer(stack<ExValue_ifs*>* visitor_stack) {
    auto small_array = operand_[1];
    auto shift = shift_parameter_;

    int64_t len = small_array->getLength();
    int64_t le = len / 2;
    int64_t ri = (len - 1) / 2;

    auto left = this->getLeftBufferLen() + ((shift < 0) ? 0 : shift) + le;
    auto right = this->getRightBufferLen() + ((shift > 0) ? 0 : -shift) + ri;

    operand_[0]->getAssignedVal(true)->setBuffered();
    operand_[0]->setBufferBordersLength(left, right);
}

void ConvolveOperation::visitEnterStackUpdate(stack<ExValue_ifs*>* visitor_stack) {
    visitor_stack->push(operand_[1]);
    visitor_stack->push(operand_[0]);
}

void ConvolveOperation::genBodyVisitExit(BodyGenContext* context) {
    is_visited_ = false;

    GarbageContainer* garbage_container = context->getGarbageContainer();
    g_pos = pos_;

    auto op2 = context->pop();
    auto op1 = context->pop();
    const TypeEn target_type = maxTypeVar(op1, op2)->type_;
    auto ret = newConvolveOperation(garbage_container, target_type, newTypeConvOp(garbage_container, target_type, op1),
                                    newTypeConvOp(garbage_container, target_type, op2), shift_parameter_, op_code_);

    context->push(ret);
}

void ConvolveOperation::calculateConstRecursive(RecursiveGenContext* context) {
    print_IR_error("ConvolveOperation::calculateConstRecursive invalid operation.");
}

void ConvolveOperation::printVisitExit(PrintBodyContext* context) {
    is_visited_ = false;

    auto op2 = context->pop();
    auto op1 = context->pop();
    context->push(checkBuffer("convolve(" + op1 + ", " + op2 + "," + std::to_string(shift_parameter_) + ")"));
}

std::string ConvolveOperation::printUint() {
    is_visited_ = false;

    auto name_op_a = operand_[0]->getAssignedVal(true)->getUniqueName();
    auto name_op_b = operand_[1]->getAssignedVal(true)->getUniqueName();

    std::string u_name = getUniqueName();

    return u_name + " = convolve( " + name_op_a + ", " + name_op_b + ", " + std::to_string(shift_parameter_) + ")";
}

void ConvolveOperation::setupIR(IRGenerator& builder) {
    auto ir_op_a_ptr = operand_[0]->getAssignedVal(true)->getIrValuePtr(builder, level_);

    auto op_b = operand_[1]->getAssignedVal(true);
    auto length = op_b->getLength();

    auto buffer_ptr = op_b->getBufferPtr();

    IR_value_ = builder.createConvolve(ir_op_a_ptr, buffer_ptr, length, -(length / 2 + shift_parameter_), type_,
                                       getUniqueName());

    finishSetupIR(builder);
}

void ConvolveOperation::calculate() {
    auto op_a = operand_[0]->getAssignedVal(true);
    auto op_b = operand_[1]->getAssignedVal(true);
    buffer_ptr_ = calcConvolveSmallArray(type_, buffer_ptr_, op_a->getBufferPtr(), op_b->getBufferPtr(),
                                         (int)op_a->getLength(), (int)op_b->getLength());
}
