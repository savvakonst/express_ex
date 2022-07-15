//
// Created by SVK on 15.07.2022.
//

#ifndef EXPRESS_EX_ENV_NEIGHBOR_POINT_OPERATION_H
#define EXPRESS_EX_ENV_NEIGHBOR_POINT_OPERATION_H


#include <vector>

#include "operations/Operation_ifs.h"

ExValue_ifs* newNeighborPointOperation(GarbageContainer* garbage_container, ExValue_ifs* arg_a, int64_t shift);


class NeighborPointOperation : public Operation_ifs {
   public:
    NeighborPointOperation(ExValue_ifs* large_arr, ExValue_ifs* small_arr, int64_t shift);

    ~NeighborPointOperation() override = default;

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


#endif  // EXPRESS_EX_ENV_NEIGHBOR_POINT_OPERATION_H
