#ifndef ARITHMETIC_OPERATION_H
#define ARITHMETIC_OPERATION_H

#include <vector>

#include "operations/Operation_ifs.h"

ExValue_ifs* newArithmeticOperation(GarbageContainer* garbage_container, TypeEn target_type, ExValue_ifs* arg_a,
                                    ExValue_ifs* arg_b, OpCodeEn op_type);

class ArithmeticOperation : public Operation_ifs {
   public:
    // constructor of arithmetic, logic or comparision operation
    ArithmeticOperation(OpCodeEn op, ExValue_ifs* var_a, ExValue_ifs* var_b);

    // void visitEnterSetupBuffer(stack<Value*>* visitor_stack) override;
    void visitEnterStackUpdate(stack<ExValue_ifs*>* visitor_stack) override;

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
