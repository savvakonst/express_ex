#ifndef LINE_H
#define LINE_H

#include <iostream>
#include <vector>

#include "ifs/AsyncParameter.h"
#include "ifs/SyncParameter.h"
#include "operations/ExValue_ifs.h"

class ExLine : public ExValue_ifs {
   public:
    ExLine(const std::string& name, ExValue_ifs* var)
        : ExValue_ifs(var->type_,                  //
                      TypeEn::unknown_jty,         //
                      var->ds_ty_,                 //
                      length_t(var->getLength()))  //
    {
        name_ = name;
        if (isConst(var)) {
            binary_value_ = var->getBinaryValue();
        }
        assigned_val_ = var;
        level_ = var->getLevel();
    }

    ~ExLine() override = default;

    ExValue_ifs* getAssignedVal(bool deep = false) override;

    virtual bool isArg() const { return false; }
    bool checkName(const std::string& name) const;

    std::string getName(bool only_name = false) const { return only_name ? name_ : checkBuffer(name_); }
    std::string getLinkName() const { return link_name_; }
    NodeTypeEn getNodeType() const override { return NodeTypeEn::kLine; }
    untyped_t* getBinaryValuePtr() { return &binary_value_; }

    // safe functions .external stack is used
    void visitEnter(stack<ExValue_ifs*>* visitor_stack, bool set_visited = true) override;
    void reverseTraversalVisitEnter(stack<ExValue_ifs*>* visitor_stack) override;

    void genBodyVisitExit(BodyGenContext* context) override;
    void printVisitExit(PrintBodyContext* context) override;
    void genBlocksVisitExit(TableGenContext* context) override;
    void setupIR(IRGenerator& builder) override {}

    virtual void setTempTypeAndBinaryValue(ExValue_ifs* var) {
        temp_type_ = var->type_;
        binary_value_ = var->getBinaryValue();
    }


    void genRecursiveVisitExit(RecursiveGenContext* context) override { is_visited_ = false; }

    void calculateConstRecursive(RecursiveGenContext* context) override {
        binary_value_ = assigned_val_->getBinaryValue();
        temp_type_ = assigned_val_->getTempType();
    }

    std::string printUint() override { return unique_name_ + " = assign(" + assigned_val_->getUniqueName() + ")"; }



   protected:
    ExLine(TypeEn type, TypeEn time_type, DataStructureTypeEn data_structure_type, length_t length)
        : ExValue_ifs(type, time_type, data_structure_type, length) {}

    ExValue_ifs* assigned_val_ = nullptr;


    std::string name_;
    std::string link_name_;
};

class ExArg : public ExLine {
   public:
    ExArg(const std::string& name, ExValue_ifs* var) : ExLine(name, var) {}

    explicit ExArg(const std::string& name)
        : ExLine(TypeEn::unknown_jty, TypeEn::unknown_jty, DataStructureTypeEn::kConstant, 1) {
        name_ = name;
    }
    ~ExArg() override = default;

    ExValue_ifs* getAssignedVal(bool deep = false) override { return assigned_val_ ? ExLine::getAssignedVal() : this; }

    bool isArg() const override { return true; }

    std::string printUint() override { return ""; }

   protected:
    ExArg(TypeEn type, TypeEn time_type, DataStructureTypeEn data_structure_type, length_t length)
        : ExLine(type, time_type, data_structure_type, length) {}
};



class ExRecursiveArg : public ExArg {
   public:
    ExRecursiveArg(const std::string& name, TypeEn ty)
        : ExArg(ty,                              //
                TypeEn::unknown_jty,             //
                DataStructureTypeEn::kVariable,  //
                length_t(1))                     //
    {
        name_ = name;
    }

    ~ExRecursiveArg() override = default;

    ExValue_ifs* getAssignedVal(bool deep = false) override { return this; }

    void reverseTraversalVisitEnter(stack<ExValue_ifs*>* visitor_stack) override {
        commonMarkUnusedVisitEnter(visitor_stack);
        is_unused_ = false;
    }
    void setupIR(IRGenerator& builder) override;

    void calculateConstRecursive(RecursiveGenContext* context) override { temp_type_ = assigned_val_->getTempType(); }

    std::string printUint() override { return unique_name_ + " = arg()"; }
};



class ExParam : public ExLine {
   public:
    ExParam(const std::string& name, const std::string& link_name, DataStructureTypeEn dsty)
        : ExLine(TypeEn::unknown_jty,  //
                 TypeEn::unknown_jty,  //
                 dsty,                 //
                 1)                    //
    {
        name_ = name;
        link_name_ = link_name;
    }

    ExParam(const std::string& name, ParameterIfs* parameter)
        : ExLine(PRMType2JITType(parameter->getPrmType()),  //
                 TypeEn::unknown_jty,                       //
                 DataStructureTypeEn::kLargeArr,            //
                 parameter->getVirtualSize()) {
        name_ = name;
        link_name_ = parameter->getName();
        if (parameter->isAsync()) {
            parameter_ = new AsyncParameter(*(AsyncParameter*)parameter);
        } else parameter_ = new SyncParameter(*(SyncParameter*)parameter);
    }


    ~ExParam() override = default;

    ExValue_ifs* getAssignedVal(bool deep = false) override { return this; }

    bool isArg() const override { return true; }

    void reverseTraversalVisitEnter(stack<ExValue_ifs*>* visitor_stack) override {
        commonMarkUnusedVisitEnter(visitor_stack);
        is_unused_ = false;
    }

    void setupIR(IRGenerator& builder) override;

    void calculateConstRecursive(RecursiveGenContext* context) override { temp_type_ = assigned_val_->getTempType(); }
    std::string printUint() override { return unique_name_ + " = parameter()"; }
};



#endif