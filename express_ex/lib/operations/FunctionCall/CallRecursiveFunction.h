//
// Created by SVK on 13.07.2022.
//

#ifndef EXPRESS_EX_ENV_CALLRECURSIVEFUNCTION_H
#define EXPRESS_EX_ENV_CALLRECURSIVEFUNCTION_H

#include "call.h"

class CallRecursiveFunction : public CallI_ifs {
   public:
    explicit CallRecursiveFunction(Body* body, const stack<ExValue*>& args = {});
    ~CallRecursiveFunction() override = default;

    void markUnusedVisitEnter(stack<ExValue*>* visitor_stack) override;
    void genBlocksVisitExit(TableGenContext* context) override;
    void setupIR(IRGenerator& builder) override;

    ExValue* getAssignedVal(bool deep = false) override { return this; }
    NodeTypeEn getNodeType() const override { return NodeTypeEn::kCall; }
};

class TailCallDirective : public CallI_ifs {
   public:
    explicit TailCallDirective(const stack<ExValue*>& args) : CallI_ifs() {
        type_ = TypeEn::unknown_jty;
        args_ = (args);
    }
    ~TailCallDirective() override = default;

    void setupIR(IRGenerator& builder) override;

    void printVisitExit(PrintBodyContext* context) override {
        std::string ret = "tailCall( " + printArgs(context) + ")";
        context->push(ret);
        is_visited_ = false;
    }

    ExValue* getAssignedVal(bool deep = false) override { return this; }
    NodeTypeEn getNodeType() const override { return NodeTypeEn::kTailCall; }
};



#endif  // EXPRESS_EX_ENV_CALLRECURSIVEFUNCTION_H