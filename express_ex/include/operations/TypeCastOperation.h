#ifndef TYPE_CAST_OPERATION_H
#define TYPE_CAST_OPERATION_H

#include <vector>

#include "Operation_ifs.h"

class TypeCastOperation : public Operation_ifs {
   public:
    // constructor of arithmetic, logic or comparision operation
    TypeCastOperation(OpCodeEn op, Value* var_a, Value* var_b) : Operation_ifs() {
        commonSetup(op, maxDSVar(var_a, var_b));

        type_ = isUnknownTy(var_a) || isUnknownTy(var_b) ? TypeEn::unknown_jty : TypeEn::int1_jty;

        level_ = maxLevelVar(var_a, var_b)->getLevel();

        operand_.push_back(var_a);
        operand_.push_back(var_b);

        for (auto i : operand_)
            if (i->getLevel() < level_) i->getAssignedVal(true)->setBuffered();
    }

    ~TypeCastOperation() override = default;

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
