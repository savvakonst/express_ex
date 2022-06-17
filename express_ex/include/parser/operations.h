#ifndef OPERATION_H
#define OPERATION_H

#include <vector>

#include "common/types_jty.h"
#include "variable.h"

// extern bool g_gpu_acceleration_enable;

class Operation : public Value {
   public:
    Operation() : Value() { op_code_ = OpCodeEn::none_op; }

    // constructor of type conversion operation or shift and decimation
    Operation(OpCodeEn op, Value* var, TypeEn target_type, int64_t shift_or_decimation = 0) : Value() {
        commonSetup(op, var);
        type_ = target_type;

        if (op == OpCodeEn::shift) {
            shift_parameter_ = shift_or_decimation;
            level_ = var->getLevel() + 1;
        } else if (op == OpCodeEn::decimation) {
            decimation_parameter_ = shift_or_decimation;
            level_ = var->getLevel() + 1;
        } else {
            level_ = var->getLevel();
        }

        operand_.push_back(var);
    }

    // constructor of convolve operation
    Operation(OpCodeEn op, Value* large_arr, Value* small_arr, int64_t shift) : Value() {
        commonSetup(op, maxDSVar(large_arr, small_arr));

        if (op == OpCodeEn::convolve) {
            shift_parameter_ = shift;
            level_ = large_arr->getLevel() + 1;
        } else {
            print_error("unknown convolve op");
        }

        type_ = large_arr->getType();
        if (data_structure_type_ == DataStructureTypeEn::kLargeArr) length_ = large_arr->getLength();
        else
            length_ = maxInt(large_arr->getLength(), small_arr->getLength());

        shift_parameter_ = shift;

        operand_.push_back(large_arr);
        operand_.push_back(small_arr);
    }

    // constructor of arithmetic, logic or comparision operation
    Operation(OpCodeEn op, Value* var_a, Value* var_b) : Value() {
        commonSetup(op, maxDSVar(var_a, var_b));

        type_ = maxTypeVar(var_a, var_b)->getType();
        type_ = isComparison(op) && !isUnknownTy(type_) ? TypeEn::int1_jty : type_;

        level_ = maxLevelVar(var_a, var_b)->getLevel();

        operand_.push_back(var_a);
        operand_.push_back(var_b);

        for (auto i : operand_)
            if (i->getLevel() < level_) i->getAssignedVal(true)->setBuffered();
    }

    // constructor of ternary operation
    Operation(OpCodeEn op, Value* var_a, Value* var_b, Value* var_c, TypeEn target_type, bool rec_call = false)
        : Value() {
        commonSetup(op, maxDSVar(var_a, var_b));
        type_ = target_type;
        level_ = maxLevelVar(maxLevelVar(var_a, var_b), var_c)->getLevel();
        contain_rec_call_ = rec_call;

        operand_.push_back(var_a);

        if (rec_call && !isUnknownTy(type_))  // it is a dirty hack
            if (var_b->getAssignedVal(true)->getNodeType() == NodeTypeEn::kTailCall) type_ = var_c->getType();
            else
                type_ = var_b->getType();

        operand_.push_back(var_b);
        operand_.push_back(var_c);

        for (auto i : operand_)
            if (i->getLevel() < level_) i->getAssignedVal(true)->setBuffered();
    }

    // constructor of small array definition
    Operation(OpCodeEn op, stack<Value*>& args, TypeEn target_type) : Value() {
        size_t args_size = args.size();
        op_code_ = op;

        if (args.empty()) print_error("range() - invalid signature");

        if (op == OpCodeEn::smallArrayDef) {
            for (auto& i : args) operand_.push_back(i);

            type_ = target_type;
            data_structure_type_ = DataStructureTypeEn::kSmallArr;
            length_ = int64_t(args_size);

            level_ = 0;
        } else if (op == OpCodeEn::smallArrayRange) {
            for (auto& i : args) operand_.push_back(i);

            if (isUnknownTy(target_type)) return;

            data_structure_type_ = DataStructureTypeEn::kSmallArr;
            smallArray();

            if (args_size > 3) print_error("range( .. ) -invalid signature");
        }
    }

    void commonSetup(OpCodeEn op, Value* var) {
        op_code_ = op;
        data_structure_type_ = var->getDSType();
        length_ = var->getLength();
    }

    int64_t getSliceParameter() const {
        if (op_code_ == OpCodeEn::shift) return shift_parameter_;
        if (op_code_ == OpCodeEn::decimation) return decimation_parameter_;
        print_error("getSliceParameter");
        return -1;
    }

    NodeTypeEn getNodeType() const override {
        return contain_rec_call_ ? NodeTypeEn::kTailCallSelect : NodeTypeEn::kOperation;
    }

    // safe functions .external stack is used
    void visitEnterSetupBuffer(stack<Value*>* visitor_stack);
    void visitEnterStackUpdate(stack<Value*>* visitor_stack);

    void visitEnter(stack<Value*>* visitor_stack) override;
    void markUnusedVisitEnter(stack<Value*>* visitor_stack) override;

    void genBodyVisitExit(BodyGenContext* context) override;
    void calculateConstRecursive(RecursiveGenContext* context) override;
    void printVisitExit(PrintBodyContext* context) override;
    void genBlocksVisitExit(TableGenContext* context) override;

    void genRecursiveVisitExit(RecursiveGenContext* context) override;

    Value* getAssignedVal(bool deep = false) override { return this; }

    std::string printUint() override;
    void setupIR(IRGenerator& builder) override;

    void calculate() override;

   private:
    void smallArrayGen();
    void smallArray();
    void smallArray(Value* arg1, Value* arg2, Value* arg3);
    void smallArray(Value* arg1, Value* arg2);
    void smallArray(Value* arg1);

    Value* getFirstLargeArrayOperand(bool deep = false);

    std::vector<Value*> operand_;
    std::vector<Value*> simplified_operand_;

    OpCodeEn op_code_ = OpCodeEn::none_op;

    // convolve params
    int64_t shift_parameter_ = 0;
    int64_t decimation_parameter_ = 0;

    bool contain_rec_call_ = false;

    const std::string ar_sym_[14] = {"+", "+.", "-", "-.", "*", "*.", "/", "/", "/.", "%", "%", "%.", "**", "**."};
    const std::string ar_comp_[17] = {
        "==", "!=", " ugt ", " uge ", " ult ", " ule ",  ">",  ">=", " less ",
        "<=", "==", "!=",    ">",     ">=",    " less ", "<=", "_",
    };
    const std::string ar_t_conv_[10] = {"trunc", "zext",   "sext",   "fptrunc", "fpext",
                                        "fptoi", "fptosi", "uitofp", "sitofp",  "common_cast"};
    const std::string ar_built_in_[6] = {"log", "log2", "log10", "cos", "sin", "exp"};
    const std::string ar_slice_[2] = {"decimation", "shift"};

    std::string txtArOp(OpCodeEn op_code) const { return ar_sym_[((int)op_code - (int)TypeOpCodeEn::arithetic)]; }
    std::string txtCompOp(OpCodeEn op_code) const { return ar_comp_[((int)op_code - (int)TypeOpCodeEn::comparsion)]; }
    std::string txtTConOp(OpCodeEn op_code) const { return ar_t_conv_[((int)op_code - (int)TypeOpCodeEn::type_conv)]; }
    std::string txtBuiltInOp(OpCodeEn op_code) const {
        return ar_built_in_[((int)op_code - (int)TypeOpCodeEn::builtInFunc)];
    }
    std::string txtSliceOp(OpCodeEn op_code) const { return ar_slice_[((int)op_code - (int)TypeOpCodeEn::slice_op)]; }
};

// Value* newInvOperation(GarbageContainer* garbage_container, Value* arg);
Value* newBuiltInFuncOperation(GarbageContainer* garbage_container, TypeEn target_type, Value* arg, OpCodeEn op_type);
Value* newIntegrateOperation(GarbageContainer* garbage_container, Value* value);
Value* newArithmeticOperation(GarbageContainer* garbage_container, TypeEn target_type, Value* arg_a, Value* arg_b,
                              OpCodeEn op_type);
Value* newComparisonOperation(GarbageContainer* garbage_container, TypeEn target_type, Value* arg_a, Value* arg_b,
                              OpCodeEn op_type);
Value* newConvolveOperation(GarbageContainer* garbage_container, TypeEn target_type, Value* arg1, Value* arg_b,
                            int64_t shift = 0, OpCodeEn op_type = OpCodeEn::convolve);
Value* newTypeConvOp(GarbageContainer* garbage_container, TypeEn target_type, Value* arg);
Value* newSelectOp(GarbageContainer* garbage_container, TypeEn target_type, Value* arg_a, Value* arg_b, Value* arg_c,
                   bool rec_call = false);
// Value* newSliceOp(GarbageContainer* garbage_container, Value* arg_a, Value* arg_b, OpCodeEn op_type);
// Value* newSliceOp(GarbageContainer* garbage_container, Value* arg_a, int64_t int_val, OpCodeEn type_op);
Value* newSmallArrayDefOp(GarbageContainer* garbage_container, stack<Value*>& args,
                          OpCodeEn op_type = OpCodeEn::smallArrayDef, bool is_template = false);

#endif