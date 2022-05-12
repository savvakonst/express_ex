#ifndef BUILT_IN_CALL_OPERATION_H
#define BUILT_IN_CALL_OPERATION_H

#include <vector>

#include "Operation_ifs.h"

class BuiltInCallOperation : public Operation_ifs {
   public:
    // constructor of type conversion operation or shift and decimation
    BuiltInCallOperation(OpCodeEn op, Value* var, TypeEn target_type) : Operation_ifs() {
        commonSetup(op, var);
        type_  = target_type;
        level_ = var->getLevel();
        operand_.push_back(var);
    }

    ~BuiltInCallOperation() override = default;

    // void visitEnterSetupBuffer(stack<Value*>* visitor_stack) override;
    void visitEnterStackUpdate(stack<Value*>* visitor_stack) override;

    void genBodyVisitExit(BodyGenContext* context) override;
    void calculateConstRecursive(RecursiveGenContext* context) override;
    void printVisitExit(PrintBodyContext* context) override;
    void genBlocksVisitExit(TableGenContext* context) override;

    std::string printUint() override;
    void setupIR(IRGenerator& builder) override;

    void calculate() override;

   private:
};

#endif