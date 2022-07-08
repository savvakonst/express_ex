#ifndef SELECT_OPERATION_H
#define SELECT_OPERATION_H

#include <vector>

#include "Operation_ifs.h"

Value* newSelectOp(GarbageContainer* garbage_container, TypeEn target_type, Value* arg_a, Value* arg_b, Value* arg_c,
                   bool rec_call = false);

class SelectOperation : public Operation_ifs {
   public:
    SelectOperation(OpCodeEn op, Value* var_a, Value* var_b, Value* var_c, TypeEn target_type, bool rec_call = false);

    ~SelectOperation() override = default;

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