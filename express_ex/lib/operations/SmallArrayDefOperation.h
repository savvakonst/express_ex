#ifndef SMALL_ARRAY_DEF_OPERATION_H
#define SMALL_ARRAY_DEF_OPERATION_H

#include <vector>


#include "operations/Operation_ifs.h"

ExValue_ifs* newSmallArrayDefOp(GarbageContainer* garbage_container, stack<ExValue_ifs*>& args, OpCodeEn op_type,
                                bool is_template = false);

class SmallArrayDefOperation : public Operation_ifs {
   public:
    static SmallArrayDefOperation* create(OpCodeEn op, stack<ExValue_ifs*>& args, TypeEn target_type);

    SmallArrayDefOperation(double start, double stop, length_t length, const std::vector<ExValue_ifs*>& args,
                           TypeEn target_type);
    SmallArrayDefOperation(const stack<ExValue_ifs*>& args, TypeEn target_type);

    ~SmallArrayDefOperation() override = default;

    // void visitEnterSetupBuffer(stack<Value*>* visitor_stack) override;
    void visitEnterStackUpdate(stack<ExValue_ifs*>* visitor_stack) override;

    void genBodyVisitExit(BodyGenContext* context) override;
    void calculateConstRecursive(RecursiveGenContext* context) override;
    void printVisitExit(PrintBodyContext* context) override;
    void genBlocksVisitExit(TableGenContext* context) override;

    std::string printUint() override;
    void setupIR(IRGenerator& builder) override;

    void calculate() override;

   private:
    void smallArrayGen();

    static SmallArrayDefOperation* create(ExValue_ifs* arg1, ExValue_ifs* arg2, ExValue_ifs* arg3);
    static SmallArrayDefOperation* create(ExValue_ifs* arg1, ExValue_ifs* arg2);
    static SmallArrayDefOperation* create(ExValue_ifs* arg1);


    const double start_;
    const double stop_;
    // int64_t shift_parameter_ = 0;
};

#endif