#include "operations/SmallArrayDefOperation.h"

#include "jit/IR_generator.h"
#include "operations/TypeCastOperation.h"
#include "parser/bodyTemplate.h"


ExValue* newSmallArrayDefOp(GarbageContainer* garbage_container, stack<ExValue*>& args, OpCodeEn op_type,
                            bool is_template) {
    if (args.empty()) print_error("SmallArray is empty");

    ExValue* var = args[0];

    bool all_is_const = true;
    for (auto i : args) {
        var = maxTypeVar(i, var);
        all_is_const &= isConst(var);
    }

    if (!all_is_const) print_error("array concatenation is not supported yet");

    TypeEn target_type = var->getType();

    if (isUnknownTy(target_type) || op_type == OpCodeEn::smallArrayRange)
        return garbage_container->add(new SmallArrayDefOperation(op_type, args, target_type));

    if (is_template)
        return garbage_container->add(new SmallArrayDefOperation(OpCodeEn::smallArrayDef, args, target_type));

    stack<ExValue*> typed_args;
    for (auto i : args) typed_args.push(newTypeConvOp(garbage_container, target_type, i));

    return garbage_container->add(new SmallArrayDefOperation(OpCodeEn::smallArrayDef, typed_args, target_type));
}

ExValue* newRangeOp(BodyTemplate* body_template, size_t arg_count) {
    if ((arg_count < 1) || (arg_count > 3)) print_error("invalid signature of range(..) function");
    body_template->is_operator_ = true;
    stack<ExValue*> v = body_template->pop(arg_count);
    return newSmallArrayDefOp(body_template->getGarbageContainer(), v, OpCodeEn::smallArrayRange);
}

ExValue* newSmallArrayDefOp(BodyTemplate* body_template, size_t arg_count) {
    stack<ExValue*> op;
    body_template->is_operator_ = true;
    for (size_t i = 0; i < arg_count; i++) op.push(body_template->pop());
    std::reverse(op.begin(), op.end());
    return newSmallArrayDefOp(body_template->getGarbageContainer(), op, OpCodeEn::smallArrayDef, true);
}


void SmallArrayDefOperation::visitEnterStackUpdate(stack<ExValue*>* visitor_stack) {
    for (auto i = operand_.rbegin(); i != operand_.rend(); i++) visitor_stack->push(*i);
}

void SmallArrayDefOperation::genBodyVisitExit(BodyGenContext* context) {
    is_visited_ = false;

    GarbageContainer* garbage_container = context->getGarbageContainer();
    g_pos = pos_;

    size_t size = operand_.size();
    stack<ExValue*> op;
    for (size_t i = 0; i < size; i++) op.push(context->pop());
    std::reverse(op.begin(), op.end());

    ExValue* ret = newSmallArrayDefOp(garbage_container, op, op_code_);

    context->push(ret);
}

void SmallArrayDefOperation::calculateConstRecursive(RecursiveGenContext* context) {
    print_IR_error("calculateConstRecursive SmallArrayDefOperation invalid command .");
    Operation_ifs::calculateConstRecursive(context);
}

void SmallArrayDefOperation::printVisitExit(PrintBodyContext* context) {
    is_visited_ = false;
    size_t size = operand_.size();
    stack<std::string> op;

    for (size_t i = 0; i < size; i++) op.push(context->pop());
    std::reverse(op.begin(), op.end());

    std::string out;
    for (auto& i : op) out += i + ", ";

    if (op_code_ == OpCodeEn::smallArrayDef) context->push("[" + out + "]");
    else context->push("range[" + out + "]");
}

void SmallArrayDefOperation::genBlocksVisitExit(TableGenContext* context) {
    unique_name_ = (isLargeArr(this) ? "vb" : "vs") + std::to_string(context->getUniqueIndex());
    context->setUint(this);
    is_visited_ = false;

    PrmTypesEn prm_type = JITType2PRMType(type_);

    context->setParameter(parameter_);

    if (isReturned()) {
        parameter_ = parameter_->newParameter();  // new SyncParameter("", parameter_->getMainTimeInterval(),
                                                  // parameter_->getDataIntervalList(),false);
        context->setParameter(parameter_);
    }

    if (parameter_ != nullptr) context->setParameter(parameter_);
}

std::string SmallArrayDefOperation::printUint() {
    is_visited_ = false;

    std::string out;
    for (auto& i : operand_) out += i->getAssignedVal(true)->getUniqueName() + ", ";

    if (op_code_ == OpCodeEn::smallArrayDef) return "[" + out + "]";

    return "range[" + out + "]";
}

void SmallArrayDefOperation::setupIR(IRGenerator& builder) {
    print_IR_error("setupIR StoreToBuffer unknown command .");
}

void SmallArrayDefOperation::calculate() { /*do nothing*/
    if (op_code_ == OpCodeEn::smallArrayDef) buffer_ptr_ = calcSmallArrayDef(type_, operand_);
    else smallArrayGen();
}

void SmallArrayDefOperation::smallArray() {
    size_t args_size = operand_.size();
    if (args_size == 1) smallArray(operand_[0]);
    else if (args_size == 2) smallArray(operand_[0], operand_[1]);
    else if (args_size == 3) smallArray(operand_[0], operand_[1], operand_[2]);
}

void SmallArrayDefOperation::smallArrayGen() {
    double delta = 0.0;
    if (isInteger(type_)) {
        delta = (stop_ - start_) / length_;
    }

    buffer_ptr_ = calcSmallArrayAlloc(type_, (int)length_);

#define OP(T)                                                                      \
    {                                                                              \
        T* point = (T*)buffer_ptr_;                                                \
        for (uint64_t i = 0; i < length_; i++) *point++ = (T)(start_ + delta * i); \
    }

    SWITCH_TYPE_OP(type_, print_SA_error("samallarray range ");)
#undef OP
};

void SmallArrayDefOperation::smallArray(ExValue* arg1, ExValue* arg2, ExValue* arg3) {
    if ((isConst(arg1) && isConst(arg2) && isConst(arg3) && isInteger(arg3))) {
        start_ = arg1->getDoubleValue();
        stop_ = arg2->getDoubleValue();

        type_ = TypeEn::double_jty;
        length_ = arg3->getBinaryValue();
    } else {
        // print_error("range(start_num,stop_num,length) -signature is not supported yet");
        print_error("range(start_num,stop_num,length) -signature is not supported yet");
        return;
    }
};

void SmallArrayDefOperation::smallArray(ExValue* arg1, ExValue* arg2) {
    if (isConst(arg1) && isConst(arg2) && isInteger(arg1) && isInteger(arg2)) {
        start_ = arg1->getDoubleValue();
        stop_ = arg2->getDoubleValue();

        type_ = TypeEn::int64_jty;
        length_ = (uint64_t)(stop_ - start_);
    } else {
        print_error("range(start_num,stop_num) - arg must be integer constant");
    }
};

void SmallArrayDefOperation::smallArray(ExValue* arg1) {
    if (isConst(arg1) && isInteger(arg1)) {
        start_ = 0;
        stop_ = (double)arg1->getBinaryValue();
        type_ = TypeEn::int64_jty;
        length_ = (uint64_t)stop_;
    } else {
        print_error("range(len) - arg must be integer constant");
    }
};