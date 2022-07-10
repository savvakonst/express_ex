#ifndef ARITHMETIC_OPERATION_H
#define ARITHMETIC_OPERATION_H

#include <vector>

#include "Operation_ifs.h"

ExValue* newArithmeticOperation(GarbageContainer* garbage_container, TypeEn target_type, ExValue* arg_a, ExValue* arg_b,
                                OpCodeEn op_type);

class ArithmeticOperation : public Operation_ifs {
   public:
    // constructor of arithmetic, logic or comparision operation
    ArithmeticOperation(OpCodeEn op, ExValue* var_a, ExValue* var_b);

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
