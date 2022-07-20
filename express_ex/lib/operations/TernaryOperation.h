#ifndef SELECT_OPERATION_H
#define SELECT_OPERATION_H

#include <vector>

#include "operations/Operation_ifs.h"

ExValue_ifs* newTernaryOperation(GarbageContainer* garbage_container, TypeEn target_type, ExValue_ifs* arg_a,
                                 ExValue_ifs* arg_b, ExValue_ifs* arg_c, bool rec_call = false);

class TernaryOperation : public Operation_ifs {
   public:
    TernaryOperation(OpCodeEn op, ExValue_ifs* var_a, ExValue_ifs* var_b, ExValue_ifs* var_c, TypeEn target_type,
                     bool rec_call = false);

    ~TernaryOperation() override = default;

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