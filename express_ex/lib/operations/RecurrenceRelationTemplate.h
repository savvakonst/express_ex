//
// Created by svk on 21.07.2022.
//

#ifndef EXPRESS_EX_ENV_RECURRENCERELATIONTEMPLATE
#define EXPRESS_EX_ENV_RECURRENCERELATIONTEMPLATE

#include "operations/Operation_ifs.h"

ExValue_ifs* newRecurrenceRelationTemplate(GarbageContainer* garbage_container, ExValue_ifs* ret_value);



class RecurrenceRelationTemplate : public Operation_ifs {
   public:
    explicit RecurrenceRelationTemplate(ExValue_ifs* ret_value);



    ~RecurrenceRelationTemplate() override = default;

    NodeTypeEn getNodeType() const override { return NodeTypeEn::kRecurrenceRelation; }

    void visitEnterSetupBuffer(stack<ExValue_ifs*>* visitor_stack) override;
    void visitEnterStackUpdate(stack<ExValue_ifs*>* visitor_stack) override;

    void genBodyVisitExit(BodyGenContext* context) override;
    void calculateConstRecursive(RecursiveGenContext* context) override;
    void printVisitExit(PrintBodyContext* context) override;
    // void genBlocksVisitExit(TableGenContext* context) override;

    std::string printUint() override;
    void setupIR(IRGenerator& builder) override;

    void calculate() override;

    //void set()

   private:
};


#endif  // EXPRESS_EX_ENV_RECURRENCERELATIONTEMPLATE
