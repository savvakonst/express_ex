//
// Created by SVK on 15.07.2022.
//

#ifndef EXPRESS_EX_ENV_EXCONSTVALUE_H
#define EXPRESS_EX_ENV_EXCONSTVALUE_H


#include "operations/ExValue_ifs.h"


class ExConstValue : public ExValue_ifs {
   public:
    ExConstValue(const std::string& text, TypeEn type);
    ExConstValue(untyped_t binary_value, TypeEn type);

    template <typename T>
    T getConvTypeVal() const {
        return *((T*)(&binary_value_));
    }

    double getDoubleValue() const;

    void visitEnter(stack<ExValue_ifs*>* visitor_stack) override;

    void reverseTraversalVisitEnter(stack<ExValue_ifs*>* visitor_stack) override;

    void genBodyVisitExit(BodyGenContext* context) override;

    void genBlocksVisitExit(TableGenContext* context) override;

    void printVisitExit(PrintBodyContext* context) override;

    void genRecursiveVisitExit(RecursiveGenContext* context) override;

    void calculateConstRecursive(RecursiveGenContext* context) override {}

    std::string printUint() override { return unique_name_ + "=" + text_value_; }

    void setupIR(IRGenerator& builder) override;

   private:
    // TODO replace and make const
    const std::string text_value_;
};



#endif  // EXPRESS_EX_ENV_EXCONSTVALUE_H
