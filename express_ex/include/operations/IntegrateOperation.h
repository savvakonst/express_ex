#ifndef CONVOLVE_OPERATION_H
#define CONVOLVE_OPERATION_H

#include <vector>

#include "Operation_ifs.h"

Value* newIntegrateOperation(GarbageContainer* garbage_container, Value* value);

class IntegrateOperation : public Operation_ifs {
   public:
    explicit IntegrateOperation(Value* var) : Operation_ifs() {
        commonSetup(OpCodeEn::integrate, var);
        type_ = var->getType();
        operand_.push_back(var);
    }

    ~IntegrateOperation() override = default;

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
};

#endif