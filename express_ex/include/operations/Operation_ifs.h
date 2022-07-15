#ifndef OPERATION_IFS_H
#define OPERATION_IFS_H

#include <vector>

#include "ExValue.h"
#include "common/types_jty.h"

// extern bool g_gpu_acceleration_enable;

class Operation_ifs : public ExValue {
   public:
    // constructor of arithmetic, logic or comparision operation
    Operation_ifs() : ExValue() {}

    void commonSetup(OpCodeEn op, const ExValue* var) {
        op_code_ = op;
        data_structure_type_ = var->getDSType();
        length_ = var->getLength();
    }

    virtual int64_t getSliceParameter() const {
        print_error("getSliceParameter");
        return -1;
    }

    NodeTypeEn getNodeType() const override {
        return contain_rec_call_ ? NodeTypeEn::kTailCallSelect : NodeTypeEn::kOperation;
    }


    virtual void visitEnterSetupBuffer(stack<ExValue*>* visitor_stack) {
        for (auto i : operand_) {
            i->setBufferLength(this);
        }
    }

    /**
     * it pushes operation operands/arguments to visitor_stack,
     * it is called by visitEnter()
     */
    virtual void visitEnterStackUpdate(stack<ExValue*>* visitor_stack);

    /**
     * it sets is_visited_ to true push themself to visitor_stack and than
     * call visitEnterStackUpdate() which pushes operation operands to visitor_stack
     * is_visited_ variable must be set to false at the begin of function
     */
    void visitEnter(stack<ExValue*>* visitor_stack) override {
        is_visited_ = true;
        visitor_stack->push(this);
        visitEnterStackUpdate(visitor_stack);
    }

    void markUnusedVisitEnter(stack<ExValue*>* visitor_stack) override;


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

    ExValue* getAssignedVal(bool deep = false) override { return this; }

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

    std::vector<ExValue*> operand_;
    std::vector<ExValue*> simplified_operand_;

    OpCodeEn op_code_ = OpCodeEn::none_op;

    bool contain_rec_call_ = false;
};

#endif
