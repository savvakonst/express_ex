#ifndef SMALL_ARRAY_DEF_OPERATION_H
#define SMALL_ARRAY_DEF_OPERATION_H

#include <vector>

#include "operations/Operation_ifs.h"

ExValue_ifs* newSmallArrayDefOp(GarbageContainer* garbage_container, stack<ExValue_ifs*>& args, OpCodeEn op_type,
                                bool is_template = false);

class SmallArrayDefOperation : public Operation_ifs {
   public:
    SmallArrayDefOperation(OpCodeEn op, stack<ExValue_ifs*>& args, TypeEn target_type) : Operation_ifs() {
        size_t args_size = args.size();
        op_code_ = op;

        if (args.empty()) print_error("range() - invalid signature");

        if (op == OpCodeEn::smallArrayDef) {
            for (auto& i : args) operand_.push_back(i);

            type_ = target_type;
            data_structure_type_ = DataStructureTypeEn::kSmallArr;
            length_ = int64_t(args_size);

            level_ = 0;
        } else if (op == OpCodeEn::smallArrayRange) {
            for (auto& i : args) operand_.push_back(i);

            if (isUnknownTy(target_type)) return;

            data_structure_type_ = DataStructureTypeEn::kSmallArr;
            smallArray();

            if (args_size > 3) print_error("range( .. ) -invalid signature");
        }
    }

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
    void smallArray();
    void smallArray(ExValue_ifs* arg1, ExValue_ifs* arg2, ExValue_ifs* arg3);
    void smallArray(ExValue_ifs* arg1, ExValue_ifs* arg2);
    void smallArray(ExValue_ifs* arg1);

    // int64_t shift_parameter_ = 0;
};

#endif