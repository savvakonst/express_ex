#ifndef CONVOLVE_OPERATION_H
#define CONVOLVE_OPERATION_H

#include <vector>

#include "Operation_ifs.h"

Value* newConvolveOperation(GarbageContainer* garbage_container, TypeEn target_type, Value* arg_a, Value* arg_b,
                            int64_t shift, OpCodeEn op_type);

class ConvolveOperation : public Operation_ifs {
   public:
    ConvolveOperation(Value* large_arr, Value* small_arr, int64_t shift);

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