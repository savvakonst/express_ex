#ifndef ARITHMETIC_OPERATION_H
#define ARITHMETIC_OPERATION_H

#include <vector>

#include "Operation_ifs.h"

Value* newArithmeticOperation(GarbageContainer* garbage_container, TypeEn target_type, Value* arg_a, Value* arg_b,
                              OpCodeEn op_type);

class ArithmeticOperation : public Operation_ifs {
   public:
    // constructor of arithmetic, logic or comparision operation
    ArithmeticOperation(OpCodeEn op, Value* var_a, Value* var_b);

    // void visitEnterSetupBuffer(stack<Value*>* visitor_stack) override;
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
