#ifndef LINE_H
#define LINE_H

#include <iostream>
#include <vector>

#include "ifs/AsyncParameter.h"
#include "ifs/SyncParameter.h"
#include "operations/ExValue_ifs.h"

class Line : public ExValue_ifs {
   public:
    Line(const std::string& name, ExValue_ifs* var)
        : ExValue_ifs(var->type_,                  //
                      TypeEn::unknown_jty,         //
                      var->ds_ty_,                 //
                      length_t(var->getLength()))  //
    {
        // TODO remove comment
        // type_ = var->getType();
        // ds_ty_ = var->getDSType();
        // length_ = var->getLength();



        name_ = name;
        if (isConst(var)) {
            binary_value_ = var->getBinaryValue();
        }
        assigned_val_ = var;
        level_ = var->getLevel();
    }

    Line(const std::string& name, TypeEn ty, DataStructureTypeEn dsty, uint64_t len)
        : ExValue_ifs(ty,                   //
                      TypeEn::unknown_jty,  //
                      dsty,                 //
                      length_t(len))        //
    {
        name_ = name;
        is_arg_ = true;
    }

    Line(const std::string& name, const std::string& link_name, DataStructureTypeEn dsty)
        : ExValue_ifs(TypeEn::unknown_jty,  //
                      TypeEn::unknown_jty,  //
                      dsty,                 //
                      1)                    //
    {
        name_ = name;
        link_name_ = link_name;
        is_arg_ = true;
    }

    Line(const std::string& name, ParameterIfs* parameter)
        : ExValue_ifs(PRMType2JITType(parameter->getPrmType()),  //
                      TypeEn::unknown_jty,                       //
                      DataStructureTypeEn::kLargeArr,            //
                      parameter->getVirtualSize()) {
        is_arg_ = true;

        name_ = name;
        link_name_ = parameter->getName();
        if (parameter->isAsync()) {
            parameter_ = new AsyncParameter(*(AsyncParameter*)parameter);
        } else parameter_ = new SyncParameter(*(SyncParameter*)parameter);
    }

    explicit Line(const std::string& name)
        : ExValue_ifs(TypeEn::unknown_jty, TypeEn::unknown_jty, DataStructureTypeEn::kConstant, 1) {
        name_ = name;
        is_arg_ = true;
    }

    ~Line() override = default;

    ExValue_ifs* getAssignedVal(bool deep = false) override;

    virtual bool isArg() const;
    bool checkName(const std::string& name) const;

    std::string getName(bool only_name = false) const { return only_name ? name_ : checkBuffer(name_); }
    std::string getLinkName() const { return link_name_; }
    NodeTypeEn getNodeType() const override { return NodeTypeEn::kLine; }
    untyped_t* getBinaryValuePtr() { return &binary_value_; }

    // safe functions .external stack is used
    void visitEnter(stack<ExValue_ifs*>* visitor_stack) override;
    void reverseTraversalVisitEnter(stack<ExValue_ifs*>* visitor_stack) override;

    void genBodyVisitExit(BodyGenContext* context) override;
    void printVisitExit(PrintBodyContext* context) override;
    void genBlocksVisitExit(TableGenContext* context) override;
    void setupIR(IRGenerator& builder) override;

    virtual void setTempTypeAndBinaryValue(ExValue_ifs* var) {
        temp_type_ = var->type_;
        binary_value_ = var->getBinaryValue();
    }


   public:
    void genRecursiveVisitExit(RecursiveGenContext* context) override {
        // context->setUint(this);
        is_visited_ = false;
    }

    void calculateConstRecursive(RecursiveGenContext* context) override {
        binary_value_ = is_arg_ ? binary_value_ : assigned_val_->getBinaryValue();
        temp_type_ = assigned_val_->getTempType();
    }

    std::string printUint() override {
        return unique_name_ + (is_arg_ ? " = arg()" : " = assign(" + assigned_val_->getUniqueName() + ")");
    }

   protected:
    Line(TypeEn type, TypeEn time_type, DataStructureTypeEn data_structure_type, length_t length)
        : ExValue_ifs(type, time_type, data_structure_type, length) {}

    ExValue_ifs* assigned_val_ = nullptr;


    std::string name_;
    std::string link_name_;

   private:
    bool is_arg_ = false;
};



class ExArgument : public Line {
   public:
    ExArgument(const std::string& name, TypeEn ty, DataStructureTypeEn dsty, uint64_t len)
        : Line(ty,                   //
               TypeEn::unknown_jty,  //
               dsty,                 //
               length_t(len))        //
    {
        name_ = name;
    }

    ExArgument(const std::string& name, const std::string& link_name, DataStructureTypeEn dsty)
        : Line(TypeEn::unknown_jty,  //
               TypeEn::unknown_jty,  //
               dsty,                 //
               1)                    //
    {
        name_ = name;
        link_name_ = link_name;
    }

    ExArgument(const std::string& name, ParameterIfs* parameter)
        : Line(PRMType2JITType(parameter->getPrmType()),  //
               TypeEn::unknown_jty,                       //
               DataStructureTypeEn::kLargeArr,            //
               parameter->getVirtualSize()) {
        name_ = name;
        link_name_ = parameter->getName();
        if (parameter->isAsync()) {
            parameter_ = new AsyncParameter(*(AsyncParameter*)parameter);
        } else parameter_ = new SyncParameter(*(SyncParameter*)parameter);
    }

    explicit ExArgument(const std::string& name)
        : Line(TypeEn::unknown_jty, TypeEn::unknown_jty, DataStructureTypeEn::kConstant, 1) {
        name_ = name;
    }

    ~ExArgument() override = default;

    ExValue_ifs* getAssignedVal(bool deep = false) override { return this; }

    bool isArg() const override { return true; }

    void reverseTraversalVisitEnter(stack<ExValue_ifs*>* visitor_stack) override {
        commonMarkUnusedVisitEnter(visitor_stack);
        is_unused_ = false;
    }

   public:
    void genRecursiveVisitExit(RecursiveGenContext* context) override {
        // context->setUint(this);
        is_visited_ = false;
    }

    void calculateConstRecursive(RecursiveGenContext* context) override {
        binary_value_ = binary_value_;
        temp_type_ = assigned_val_->getTempType();
    }

    std::string printUint() override { return unique_name_ + " = arg()"; }
};



#endif