#ifndef OPERATION_IFS_H
#define OPERATION_IFS_H

#include <vector>

#include "ExValue_ifs.h"
#include "common/types_jty.h"



class Operation_ifs : public ExValue_ifs {
   public:
    Operation_ifs() : ExValue_ifs() {}

    Operation_ifs(TypeEn ty, TypeEn time_ty, DataStructureTypeEn ds_ty, length_t length, OpCodeEn op)
        : ExValue_ifs(ty, time_ty, ds_ty, length), op_code_(op) {}

    /**
     * initialize op_code_, ds_ty_ and length_ fields
     */
    Operation_ifs(TypeEn ty, TypeEn time_ty, OpCodeEn op, const ExValue_ifs* var)
        : ExValue_ifs(ty, time_ty, var->ds_ty_, var->getLength()), op_code_(op) {}


    NodeTypeEn getNodeType() const override { return NodeTypeEn::kOperation; }

    /**
     * it is called by reverseTraversalVisitEnter() in simplification stage, when typed Body has been already built.
     * This is used to define buffer bounds. Typically this is overridden by operations which implement operators
     * like convolve, neighbour point (data shift), etc.
     */
    virtual void visitEnterSetupBuffer(stack<ExValue_ifs*>* visitor_stack) {
        for (auto i : operand_) {
            i->setBufferBordersLength(this);
        }
    }

    /**
     * it pushes operation operands/arguments to visitor_stack,
     * it is called by visitEnter()
     * @param visitor_stack
     */
    virtual void visitEnterStackUpdate(stack<ExValue_ifs*>* visitor_stack);

    /**
     * it sets is_visited_ to true if set_visited = true, pushes themself to visitor_stack and than
     * call visitEnterStackUpdate() which pushes operation operands to visitor_stack
     * is_visited_ variable must be set to false at the begin of function
     * @param set_visited
     * @param visitor_stack
     */
    void visitEnter(stack<ExValue_ifs*>* visitor_stack, bool set_visited = true) override {
        is_visited_ = set_visited;
        visitor_stack->push(this);
        visitEnterStackUpdate(visitor_stack);
    }

    /**
     * It is called in simplification stage when typed Body has been already built.
     * it pushes to visitor stack and calls commonMarkUnusedVisitEnter(stack<ExValue_ifs*>* visitor_stack) which
     * increase usage counter and visitEnterSetupBuffer(visitor_stack);
     * @param visitor_stack
     *
     * \verbatim embed:rst:leading-asterisk
     *      .. uml::
     *
     *          @startuml
     *          activate Body
     *
     *          Body -> Operation_ifs: reverseTraversalVisitEnter:
     *
     *              activate Operation_ifs
     *
     *                  Operation_ifs ->Operation_ifs:   commonMarkUnusedVisitEnter(visitor_stack);
     *                  Operation_ifs ->Operation_ifs:   visitEnterSetupBuffer(visitor_stack);
     *                  Operation_ifs ->Operation_ifs:   visitEnterStackUpdate(visitor_stack);
     *
     *              deactivate Operation_ifs
     *
     *          deactivate Body
     *          @enduml
     *
     * \endverbatim
     */
    void reverseTraversalVisitEnter(stack<ExValue_ifs*>* visitor_stack) override;


    /**
     * is used for body generation (type inference, const expression execution, etc.).
     * is_visited_ variable must be set to false at the begin of function
     * @param context
     */
    void genBodyVisitExit(BodyGenContext* context) override {
        print_error("visitExit unknown command. In line : " + std::to_string(context->getNamespace().size()));
    }

    void calculateConstRecursive(RecursiveGenContext* context) override {
        print_IR_error("calculateConstRecursive unknown command.");
    }

    void printVisitExit(PrintBodyContext* context) override { print_error("visitExitTxt unknown command."); }

    void genBlocksVisitExit(TableGenContext* context) override;

    void genRecursiveVisitExit(RecursiveGenContext* context) override {
        context->setUint(this);
        is_visited_ = false;
    }

    ExValue_ifs* getAssignedVal(bool deep = false) override { return this; }

    std::string printUint() override {
        print_error("printUint unknown command .");
        return "";
    }

    /**
     * used fo ir generation, in the end of function implementation finishSetupIR() must be called
     * @param builder
     */
    void setupIR(IRGenerator& builder) override { print_IR_error("setupIR unknown command ."); }

    void calculate() override = 0;

   protected:
    /**
     * must be called in the end of setupIR()
     * @param builder
     */
    void finishSetupIR(IRGenerator& builder);

    std::vector<ExValue_ifs*> operand_;
    std::vector<ExValue_ifs*> simplified_operand_;

    const OpCodeEn op_code_ = OpCodeEn::none_op;
};

#endif
