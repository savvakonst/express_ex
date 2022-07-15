#ifndef CONVOLVE_OPERATION_H
#define CONVOLVE_OPERATION_H

#include <vector>

#include "operations/Operation_ifs.h"

ExValue_ifs* newIntegrateOperation(GarbageContainer* garbage_container, ExValue_ifs* value);

class IntegrateOperation : public Operation_ifs {
   public:
    explicit IntegrateOperation(ExValue_ifs* var);

    ~IntegrateOperation() override = default;

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
    llvm::Value* local_buffer_ptr_ = nullptr;
};

#endif