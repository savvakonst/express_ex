#ifndef CALL_H_
#define CALL_H_

#include "operations/ExLine.h"
#include "operations/ExValue_ifs.h"
#include "parser/basic.h"
#include "parser/body.h"

namespace llvm {
class Function;
}

class IRGenerator;

class CallI_ifs : public ExValue_ifs {
   protected:
    CallI_ifs(TypeEn ty, TypeEn time_ty, DataStructureTypeEn ds_ty, length_t length, const stack<ExValue_ifs*>& args)
        : ExValue_ifs(ty, time_ty, ds_ty, length) {
        args_ = args;
        args_.reverse();
    }

    CallI_ifs(TypeEn ty, TypeEn time_ty, const ExValue_ifs* var, const stack<ExValue_ifs*>& args)
        : CallI_ifs(ty, time_ty, var->ds_ty_, var->getLength(), args) {}

   public:
    CallI_ifs() : ExValue_ifs() {}
    ~CallI_ifs() override = default;

    void visitEnter(stack<ExValue_ifs*>* visitor_stack, bool set_visited = true) override {
        visitor_stack->push(this);
        for (auto arg = args_.rbegin(); arg != args_.rend(); arg++) visitor_stack->push(*arg);

        is_visited_ = set_visited;
    }

    void printVisitExit(PrintBodyContext* context) override {
        if (body_) {
            context->addFunction(body_->print(context->tab_ + "  ", context->DST_ena_, context->hide_unused_lines_));

            context->push(body_->getName() + "( " + printArgs(context) + ").ret." + toString(type_));
            // is_visited_ = false;
        }
        is_visited_ = false;
    }

    std::string printUint() override { return "call" + body_->getName(); }

    ExValue_ifs* getAssignedVal(bool deep = false) override {
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
    stack<ExValue_ifs*> args_;
};



class Call : public CallI_ifs {
   public:
    explicit Call(Body* body, const stack<ExValue_ifs*>& args = {});
    ~Call() override = default;

    void reverseTraversalVisitEnter(stack<ExValue_ifs*>* visitor_stack) override;
    void genBlocksVisitExit(TableGenContext* context) override;
    void setupIR(IRGenerator& builder) override;

    NodeTypeEn getNodeType() const override { return NodeTypeEn::kCall; }
};



#endif