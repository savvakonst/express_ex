#ifndef CALL_TEMPLATE_H_
#define CALL_TEMPLATE_H_

#include "body.h"
#include "bodyTemplate.h"
#include "line.h"


class CallTemplate_ifs : public ExValue {
   public:
    CallTemplate_ifs() : ExValue() {}
    ~CallTemplate_ifs() override = default;

    void visitEnter(stack<ExValue*>* visitor_stack) override {
        visitor_stack->push(this);
        for (int64_t i = ((int64_t)args_.size() - 1); i >= 0; i--) {
            visitor_stack->push(args_[(size_t)i]);
        }
        is_visited_ = true;
    }

    void printVisitExit(PrintBodyContext* context) override {
        if (body_template_) {
            context->addVoid(
                body_template_->print(context->tab_ + "  ", context->DST_ena_, context->hide_unused_lines_));

            context->push(body_template_->getName() + "( " + printArgs(context) + ").ret." + toString(type_));
        }
        is_visited_ = false;
    }

    std::string printUint() override {
        return unique_name_ + " = assignCallTemplate(" +
               body_template_->getRet().front()->getAssignedVal(true)->getUniqueName() + ")";
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

    BodyTemplate* body_template_ = nullptr;
    stack<ExValue*> args_;
};

class CallRecursiveFunctionTemplate : public CallTemplate_ifs {
   public:
    explicit CallRecursiveFunctionTemplate(BodyTemplate* body, const stack<ExValue*>& args = {});

    ~CallRecursiveFunctionTemplate() override = default;

    void genBodyVisitExit(BodyGenContext* context) override;

    void calculateConstRecursive(RecursiveGenContext* context) override;

    NodeTypeEn getNodeType() const override { return NodeTypeEn::kCall; }
};

class TailCallDirectiveTemplate : public CallTemplate_ifs {
   public:
    explicit TailCallDirectiveTemplate(const stack<ExValue*>& args) : CallTemplate_ifs() {
        args_ = args;
        type_ = TypeEn::unknown_jty;
    }

    ~TailCallDirectiveTemplate() override = default;

    void genBodyVisitExit(BodyGenContext* context) override;

    void printVisitExit(PrintBodyContext* context) override;

    void calculateConstRecursive(RecursiveGenContext* context) override;



    NodeTypeEn getNodeType() const override { return NodeTypeEn::kTailCall; }
};

class CallTemplate : public CallTemplate_ifs {
   public:
    explicit CallTemplate(BodyTemplate* body, const stack<ExValue*>& args = {});

    ~CallTemplate() override = default;

    void genBodyVisitExit(BodyGenContext* context) override;

    void calculateConstRecursive(RecursiveGenContext* context) override;

    NodeTypeEn getNodeType() const override { return NodeTypeEn::kCall; }
};

#endif