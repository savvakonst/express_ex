#ifndef CALL_TEMPLATE_H_
#define CALL_TEMPLATE_H_

#include "parser/body.h"
#include "parser/bodyTemplate.h"
#include "parser/line.h"


class CallTemplate_ifs : public ExValue_ifs {
   public:
    CallTemplate_ifs(TypeEn ty, TypeEn time_ty, DataStructureTypeEn ds_ty, length_t length)
        : ExValue_ifs(ty, time_ty, ds_ty, length) {}

    CallTemplate_ifs(TypeEn ty, TypeEn time_ty, const ExValue_ifs* var)
        : ExValue_ifs(ty, time_ty, var->ds_ty_, var->getLength()) {}

    ~CallTemplate_ifs() override = default;

    void visitEnter(stack<ExValue_ifs*>* visitor_stack) override {
        visitor_stack->push(this);
        for (int64_t i = ((int64_t)args_.size() - 1); i >= 0; i--) {
            visitor_stack->push(args_[(size_t)i]);
        }
        is_visited_ = true;
    }

    void genBlocksVisitExit(TableGenContext* context) override {
        print_error("genBlocksVisitExit can't be used with templates");
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

    void setupIR(IRGenerator& builder) override { print_error("setupIR can't be used with templates"); }

   protected:
    std::string printArgs(PrintBodyContext* context) {
        std::string ret;
        for (size_t i = args_.size(); i > 0; i--) ret += context->pop() + ((i != 1) ? ", " : "");
        return ret;
    }

    BodyTemplate* body_template_ = nullptr;
    stack<ExValue_ifs*> args_;
};

class CallRecursiveFunctionTemplate : public CallTemplate_ifs {
   public:
    explicit CallRecursiveFunctionTemplate(BodyTemplate* body, const stack<ExValue_ifs*>& args = {});

    ~CallRecursiveFunctionTemplate() override = default;

    void genBodyVisitExit(BodyGenContext* context) override;

    void calculateConstRecursive(RecursiveGenContext* context) override;

    NodeTypeEn getNodeType() const override { return NodeTypeEn::kCall; }
};



class TailCallDirectiveTemplate : public CallTemplate_ifs {
   public:
    explicit TailCallDirectiveTemplate(const stack<ExValue_ifs*>& args)
        // TODO try with DataStructureTypeEn::kVariable
        : CallTemplate_ifs(TypeEn::unknown_jty, TypeEn::unknown_jty, DataStructureTypeEn::kConstant, 1) {
        args_ = args;
    }

    ~TailCallDirectiveTemplate() override = default;

    void genBodyVisitExit(BodyGenContext* context) override;

    void printVisitExit(PrintBodyContext* context) override;

    void calculateConstRecursive(RecursiveGenContext* context) override;

    NodeTypeEn getNodeType() const override { return NodeTypeEn::kTailCall; }
};



class CallTemplate : public CallTemplate_ifs {
   public:
    explicit CallTemplate(BodyTemplate* body, const stack<ExValue_ifs*>& args = {});

    ~CallTemplate() override = default;

    void genBodyVisitExit(BodyGenContext* context) override;

    void calculateConstRecursive(RecursiveGenContext* context) override;

    NodeTypeEn getNodeType() const override { return NodeTypeEn::kCall; }
};

#endif