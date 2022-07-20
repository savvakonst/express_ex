//
// Created by SVK on 16.07.2022.
//

#ifndef EXPRESS_EX_ENV_RECURSIVE_NEIGHBOR_POINT_OPERATION_H
#define EXPRESS_EX_ENV_RECURSIVE_NEIGHBOR_POINT_OPERATION_H


#include <vector>

#include "operations/Operation_ifs.h"

ExValue_ifs* newRecursiveNeighborPointOperation(GarbageContainer* garbage_container, TypeEn target_type,
                                                ExValue_ifs* shift);


class RecursiveNeighborPointOperation : public Operation_ifs {
   public:
    RecursiveNeighborPointOperation(TypeEn target_type, ExValue_ifs* shift);

    ~RecursiveNeighborPointOperation() override = default;

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


#endif  // EXPRESS_EX_ENV_RECURSIVE_NEIGHBOR_POINT_OPERATION_H
