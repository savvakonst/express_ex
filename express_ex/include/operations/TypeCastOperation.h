#ifndef TYPE_CAST_OPERATION_H
#define TYPE_CAST_OPERATION_H

#include <vector>

#include "Operation_ifs.h"

ExValue* newTypeConvOp(GarbageContainer* garbage_container, TypeEn target_type, ExValue* arg);

class TypeCastOperation : public Operation_ifs {
   public:
    TypeCastOperation(OpCodeEn op, ExValue* var, TypeEn target_type) : Operation_ifs() {
        commonSetup(op, var);
        type_ = target_type;

        level_ = var->getLevel();

        operand_.push_back(var);
    }

    ~TypeCastOperation() override = default;

    // void visitEnterSetupBuffer(stack<Value*>* visitor_stack) override;
    void visitEnterStackUpdate(stack<ExValue*>* visitor_stack) override;

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
