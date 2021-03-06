#ifndef CALL_TEMPLATE_H_
#define CALL_TEMPLATE_H_


#include "ExLine.h"
#include "parser/body.h"
#include "parser/bodyTemplate.h"


class CallTemplate_ifs : public ExValue_ifs {
   public:
    CallTemplate_ifs(TypeEn ty, TypeEn time_ty, DataStructureTypeEn ds_ty, length_t length,
                     const stack<ExValue_ifs*>& args)
        : ExValue_ifs(ty, time_ty, ds_ty, length), args_(args) {}

    CallTemplate_ifs(TypeEn ty, TypeEn time_ty, const ExValue_ifs* var, const stack<ExValue_ifs*>& args)
        : ExValue_ifs(ty, time_ty, DataStructureTypeEn::kVariable, var->getLength()), args_(args) {}

    ~CallTemplate_ifs() override = default;

    void visitEnter(stack<ExValue_ifs*>* visitor_stack, bool set_visited = true) override {
        visitor_stack->push(this);

        for (auto arg = args_.rbegin(); arg != args_.rend(); arg++) visitor_stack->push(*arg);
        // for (int64_t i = ((int64_t)args_.size() - 1); i >= 0; i--) {
        //     visitor_stack->push(args_[(size_t)i]);
        // }
        is_visited_ = set_visited;
    }

    void genBlocksVisitExit(TableGenContext* context) override {
        print_error("genBlocksVisitExit can't be used with templates");
    }

    void printVisitExit(PrintBodyContext* context) override {
        if (corresponding_body_template_) {
            context->addFunction(corresponding_body_template_->print(context->tab_ + "  ", context->DST_ena_,
                                                                     context->hide_unused_lines_));

            context->push(corresponding_body_template_->getName() + "( " + printArgs(context) + ").ret." +
                          toString(type_));
        }
        is_visited_ = false;
    }

    std::string printUint() override {
        return unique_name_ + " = assignCallTemplate(" +
               corresponding_body_template_->getRet().front()->getAssignedVal(true)->getUniqueName() + ")";
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

    BodyTemplate* corresponding_body_template_ = nullptr;
    stack<ExValue_ifs*> args_;
};

/**
 *
 *
 */
class CallTemplate : public CallTemplate_ifs {
   public:
    explicit CallTemplate(BodyTemplate* body, const stack<ExValue_ifs*>& args = {});

    ~CallTemplate() override = default;

    void genBodyVisitExit(BodyGenContext* context) override;

    void calculateConstRecursive(RecursiveGenContext* context) override;

    NodeTypeEn getNodeType() const override { return NodeTypeEn::kCall; }
};

/**
 *
 *
 */
class CallRecursiveFunctionTemplate : public CallTemplate_ifs {
   public:
    explicit CallRecursiveFunctionTemplate(BodyTemplate* body, const stack<ExValue_ifs*>& args = {});

    ~CallRecursiveFunctionTemplate() override = default;

    void genBodyVisitExit(BodyGenContext* context) override;

    void calculateConstRecursive(RecursiveGenContext* context) override;

    NodeTypeEn getNodeType() const override { return NodeTypeEn::kCall; }
};

/**
 *
 *
 */
class TailCallDirectiveTemplate : public CallTemplate_ifs {
   public:
    explicit TailCallDirectiveTemplate(const stack<ExValue_ifs*>& args)
        // TODO try with DataStructureTypeEn::kVariable
        : CallTemplate_ifs(TypeEn::unknown, TypeEn::unknown, DataStructureTypeEn::kConstant, 1, args) {}

    ~TailCallDirectiveTemplate() override = default;

    void genBodyVisitExit(BodyGenContext* context) override;

    void printVisitExit(PrintBodyContext* context) override;

    void calculateConstRecursive(RecursiveGenContext* context) override;

    NodeTypeEn getNodeType() const override { return NodeTypeEn::kTailCall; }
};



#endif