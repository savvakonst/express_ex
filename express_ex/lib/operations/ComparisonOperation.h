#ifndef COMPARISON_OPERATION_H
#define COMPARISON_OPERATION_H

#include <vector>

#include "operations/Operation_ifs.h"

ExValue* newComparisonOperation(GarbageContainer* garbage_container, TypeEn target_type, ExValue* arg_a, ExValue* arg_b,
                                OpCodeEn op_type);

class ComparisonOperation : public Operation_ifs {
   public:
    // constructor of arithmetic, logic or comparision operation
    ComparisonOperation(OpCodeEn op, ExValue* var_a, ExValue* var_b);

    ~ComparisonOperation() override = default;

    // void visitEnterSetupBuffer(stack<Value*>* visitor_stack) override;
    void visitEnterStackUpdate(stack<ExValue*>* visitor_stack) override;

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
