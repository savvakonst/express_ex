#ifndef CALL_H_
#define CALL_H_
#include "parser/ExValue.h"
#include "parser/basic.h"
#include "parser/body.h"
#include "parser/line.h"

namespace llvm {
class Function;
}

class IRGenerator;

class CallI_ifs : public ExValue {
   public:
    CallI_ifs() : ExValue() {}
    ~CallI_ifs() override = default;

    void visitEnter(stack<ExValue*>* visitor_stack) override {
        visitor_stack->push(this);
        for (int64_t i = ((int64_t)args_.size() - 1); i >= 0; i--) {
            visitor_stack->push(args_[(size_t)i]);
        }
        is_visited_ = true;
    }

    void printVisitExit(PrintBodyContext* context) override {
        if (body_) {
            context->addVoid(body_->print(context->tab_ + "  ", context->DST_ena_, context->hide_unused_lines_));

            context->push(body_->getName() + "( " + printArgs(context) + ").ret." + toString(type_));
            // is_visited_ = false;
        }
        is_visited_ = false;
    }

    ExValue* getAssignedVal(bool deep = false) override {
        if (body_ == nullptr) return nullptr;

        if (is_buffered_ & deep) {
            body_->getRet().front()->getAssignedVal(true)->setBuffered();
        }

        return body_->getRet().front()->getAssignedVal(deep);
    }

    void genBodyVisitExit(BodyGenContext* context) override {
        print_error("genBodyVisitExit is not supported for CallI_ifs");
    }

    void genRecursiveVisitExit(RecursiveGenContext* context) override {
        context->setUint(this);
        is_visited_ = false;
    }

   protected:
    std::string printArgs(PrintBodyContext* context) {
        std::string ret;
        for (size_t i = args_.size(); i > 0; i--) ret += context->pop() + ((i != 1) ? ", " : "");
        return ret;
    }

    Body* body_ = nullptr;
    stack<ExValue*> args_;
};



class Call : public CallI_ifs {
   public:
    explicit Call(Body* body, const stack<ExValue*>& args = {});
    ~Call() override = default;

    void markUnusedVisitEnter(stack<ExValue*>* visitor_stack) override;
    void genBlocksVisitExit(TableGenContext* context) override;
    void setupIR(IRGenerator& builder) override;

    NodeTypeEn getNodeType() const override { return NodeTypeEn::kCall; }
};



#endif