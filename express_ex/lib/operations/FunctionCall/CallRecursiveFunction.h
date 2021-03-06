//
// Created by SVK on 13.07.2022.
//

#ifndef EXPRESS_EX_ENV_CALLRECURSIVEFUNCTION_H
#define EXPRESS_EX_ENV_CALLRECURSIVEFUNCTION_H


#include "call.h"

class CallRecursiveFunction : public CallI_ifs {
   public:
    explicit CallRecursiveFunction(Body* body, const stack<ExValue_ifs*>& args = {});
    ~CallRecursiveFunction() override = default;

    void reverseTraversalVisitEnter(stack<ExValue_ifs*>* visitor_stack) override;
    void genBlocksVisitExit(TableGenContext* context) override;
    void setupIR(IRGenerator& builder) override;

    ExValue_ifs* getAssignedVal(bool deep = false) override { return this; }
    NodeTypeEn getNodeType() const override { return NodeTypeEn::kCall; }
};



class TailCallDirective : public CallI_ifs {
   public:
    explicit TailCallDirective(const stack<ExValue_ifs*>& args)
        : CallI_ifs(TypeEn::unknown, TypeEn::unknown, DataStructureTypeEn::kConstant, 1, args) {}

    ~TailCallDirective() override = default;

    void genBlocksVisitExit(TableGenContext* context) override;

    void setupIR(IRGenerator& builder) override;

    void printVisitExit(PrintBodyContext* context) override;

    ExValue_ifs* getAssignedVal(bool deep = false) override { return this; }
    NodeTypeEn getNodeType() const override { return NodeTypeEn::kTailCall; }
};



#endif  // EXPRESS_EX_ENV_CALLRECURSIVEFUNCTION_H
