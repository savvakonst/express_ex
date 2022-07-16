#include "SmallArrayDefOperation.h"

#include "TypeCastOperation.h"
#include "jit/IR_generator.h"
#include "operations/ExConstValue.h"
#include "parser/bodyTemplate.h"

ExValue_ifs* newSmallArrayDefOp(GarbageContainer* garbage_container, stack<ExValue_ifs*>& args, OpCodeEn op_type,
                                bool is_template) {
    if (args.empty()) print_error("SmallArray is empty");

    ExValue_ifs* var = args[0];

    bool all_is_const = true;
    for (auto i : args) {
        var = maxTypeVar(i, var);
        all_is_const &= isConst(var);
    }

    if (!all_is_const) print_error("array concatenation is not supported yet");

    TypeEn target_type = var->type_;

    if (isUnknownTy(target_type) || op_type == OpCodeEn::smallArrayRange)
        return garbage_container->add(SmallArrayDefOperation::create(op_type, args, target_type));

    if (is_template)
        return garbage_container->add(SmallArrayDefOperation::create(OpCodeEn::smallArrayDef, args, target_type));

    stack<ExValue_ifs*> typed_args;
    for (auto i : args) typed_args.push(newTypeConvOp(garbage_container, target_type, i));

    return garbage_container->add(SmallArrayDefOperation::create(OpCodeEn::smallArrayDef, typed_args, target_type));
}

ExValue_ifs* newRangeOp(BodyTemplate* body_template, size_t arg_count) {
    if ((arg_count < 1) || (arg_count > 3)) print_error("invalid signature of range(..) function");
    body_template->is_operator_ = true;
    stack<ExValue_ifs*> v = body_template->pop(arg_count);
    return newSmallArrayDefOp(body_template->getGarbageContainer(), v, OpCodeEn::smallArrayRange);
}

ExValue_ifs* newSmallArrayDefOp(BodyTemplate* body_template, size_t arg_count) {
    stack<ExValue_ifs*> op;
    body_template->is_operator_ = true;
    for (size_t i = 0; i < arg_count; i++) op.push(body_template->pop());
    std::reverse(op.begin(), op.end());
    return newSmallArrayDefOp(body_template->getGarbageContainer(), op, OpCodeEn::smallArrayDef, true);
}

SmallArrayDefOperation::SmallArrayDefOperation(double start, double stop, ExValue_ifs::length_t length,
                                               const std::vector<ExValue_ifs*>& args, TypeEn target_type)
    : Operation_ifs(target_type,                     //
                    TypeEn::unknown_jty,             //
                    DataStructureTypeEn::kSmallArr,  //
                    length_t(args.size()),           //
                    OpCodeEn::smallArrayDef),        //
      start_(start),
      stop_(stop)  //
{
    if (args.empty()) print_error("range() - invalid signature");
    for (auto& i : args) operand_.push_back(i);
}

SmallArrayDefOperation::SmallArrayDefOperation(const stack<ExValue_ifs*>& args, TypeEn target_type)
    : Operation_ifs(target_type,                     //
                    TypeEn::unknown_jty,             //
                    DataStructureTypeEn::kSmallArr,  //
                    length_t(args.size()),           //
                    OpCodeEn::smallArrayDef),        //
      start_(0),
      stop_(0)  //
{
    if (args.empty()) print_error("range() - invalid signature");
    for (auto& i : args) operand_.push_back(i);
    level_ = 0;
}


void SmallArrayDefOperation::visitEnterStackUpdate(stack<ExValue_ifs*>* visitor_stack) {
    for (auto i = operand_.rbegin(); i != operand_.rend(); i++) visitor_stack->push(*i);
}

void SmallArrayDefOperation::genBodyVisitExit(BodyGenContext* context) {
    is_visited_ = false;

    GarbageContainer* garbage_container = context->getGarbageContainer();
    g_pos = pos_;

    size_t size = operand_.size();
    stack<ExValue_ifs*> op;
    for (size_t i = 0; i < size; i++) op.push(context->pop());
    std::reverse(op.begin(), op.end());

    ExValue_ifs* ret = newSmallArrayDefOp(garbage_container, op, op_code_);

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
    is_visited_ = false;

    unique_name_ = (isLargeArr(this) ? "vb" : "vs") + std::to_string(context->getUniqueIndex());
    context->setUint(this);


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



SmallArrayDefOperation* SmallArrayDefOperation::create(ExValue_ifs* arg1, ExValue_ifs* arg2, ExValue_ifs* arg3) {
    if ((isConst(arg1) && isConst(arg2) && isConst(arg3) && isInteger(arg3))) {
        auto start = ((ExConstValue*)arg1)->getDoubleValue();
        auto stop = ((ExConstValue*)arg2)->getDoubleValue();

        auto type = TypeEn::double_jty;
        auto length = arg3->getBinaryValue();
        return new SmallArrayDefOperation(start, stop, length, {arg1}, type);
    } else {
        // print_error("range(start_num,stop_num,length) -signature is not supported yet");
        print_error("range(start_num,stop_num,length) -signature is not supported yet");
    }
    return nullptr;
};

SmallArrayDefOperation* SmallArrayDefOperation::create(ExValue_ifs* arg1, ExValue_ifs* arg2) {
    if (isConst(arg1) && isConst(arg2) && isInteger(arg1) && isInteger(arg2)) {
        auto start = ((ExConstValue*)arg1)->getDoubleValue();
        auto stop = ((ExConstValue*)arg2)->getDoubleValue();

        auto type = TypeEn::int64_jty;
        auto length = length_t(stop - start);
        return new SmallArrayDefOperation(start, stop, length, {arg1}, type);
    } else {
        print_error("range(start_num,stop_num) - arg must be integer constant");
    }
    return nullptr;
};

SmallArrayDefOperation* SmallArrayDefOperation::create(ExValue_ifs* arg1) {
    if (isConst(arg1) && isInteger(arg1)) {
        auto start = 0;
        auto stop = (double)arg1->getBinaryValue();
        auto type = TypeEn::int64_jty;
        auto length = length_t(stop);
        return new SmallArrayDefOperation(start, stop, length, {arg1}, type);
    } else {
        print_error("range(len) - arg must be integer constant");
    }
    return nullptr;
}
SmallArrayDefOperation* SmallArrayDefOperation::create(OpCodeEn op, stack<ExValue_ifs*>& args, TypeEn target_type) {
    auto args_size = args.size();

    if (op == OpCodeEn::smallArrayDef) {
        return new SmallArrayDefOperation(args, target_type);
    } else if (op == OpCodeEn::smallArrayRange) {
        if (args_size == 1) return create(args[0]);
        else if (args_size == 2) return create(args[0], args[1]);
        else if (args_size == 3) return create(args[0], args[1], args[2]);
    }
    return nullptr;
}
