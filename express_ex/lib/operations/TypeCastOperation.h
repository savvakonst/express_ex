#ifndef TYPE_CAST_OPERATION_H
#define TYPE_CAST_OPERATION_H

#include <vector>


#include "operations/Operation_ifs.h"

ExValue_ifs* newTypeConvOp(GarbageContainer* garbage_container, TypeEn target_type, ExValue_ifs* arg);

class TypeCastOperation : public Operation_ifs {
   public:
    TypeCastOperation(OpCodeEn op, ExValue_ifs* var, TypeEn target_type);

    ~TypeCastOperation() override = default;

    // void visitEnterSetupBuffer(stack<Value*>* visitor_stack) override;
    void visitEnterStackUpdate(stack<ExValue_ifs*>* visitor_stack) override;

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
