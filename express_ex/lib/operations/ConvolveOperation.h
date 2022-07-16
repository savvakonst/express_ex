#ifndef CONVOLVE_OPERATION_H
#define CONVOLVE_OPERATION_H

#include <vector>

#include "operations/Operation_ifs.h"

ExValue_ifs* newConvolveOperation(GarbageContainer* garbage_container, TypeEn target_type, ExValue_ifs* arg_a,
                                  ExValue_ifs* arg_b, int64_t shift, OpCodeEn op_type);

class ConvolveOperation : public Operation_ifs {
   public:
    /**
     * you nust ensure that the data_structure_type_ of large_arr is more or equal small_arr data_structure_type_.
     */
    ConvolveOperation(ExValue_ifs* large_arr, ExValue_ifs* small_arr, int64_t shift);

    ~ConvolveOperation() override = default;

    void visitEnterSetupBuffer(stack<ExValue_ifs*>* visitor_stack) override;
    void visitEnterStackUpdate(stack<ExValue_ifs*>* visitor_stack) override;

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