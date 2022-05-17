#ifndef CONVOLVE_OPERATION_H
#define CONVOLVE_OPERATION_H

#include <vector>

#include "Operation_ifs.h"

Value* newConvolveOperation(GarbageContainer* garbage_container, TypeEn target_type, Value* arg_a, Value* arg_b,
                            int64_t shift, OpCodeEn op_type);

class ConvolveOperation : public Operation_ifs {
   public:
    ConvolveOperation(OpCodeEn op, Value* large_arr, Value* small_arr, int64_t shift) : Operation_ifs() {
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

    ~ConvolveOperation() override = default;

    void visitEnterSetupBuffer(stack<Value*>* visitor_stack) override;
    void visitEnterStackUpdate(stack<Value*>* visitor_stack) override;

    void genBodyVisitExit(BodyGenContext* context) override;
    void calculateConstRecursive(RecursiveGenContext* context) override;
    void printVisitExit(PrintBodyContext* context) override;
    // void genBlocksVisitExit(TableGenContext* context) override;

    std::string printUint() override;
    void setupIR(IRGenerator& builder) override;

    void calculate() override;

   private:
    // convolve params
    int64_t shift_parameter_ = 0;
};

#endif