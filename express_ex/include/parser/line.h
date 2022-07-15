#ifndef LINE_H
#define LINE_H

#include <iostream>
#include <vector>

#include "ifs/AsyncParameter.h"
#include "ifs/SyncParameter.h"
#include "operations/ExValue_ifs.h"

class Line : public ExValue_ifs {
   public:
    Line(const std::string& name, ExValue_ifs* var) : ExValue_ifs() {
        names_.push_back(name);
        name_ = name;
        if (isConst(var)) {
            binary_value_ = var->getBinaryValue();
        }
        assigned_val_ = var;
        level_ = var->getLevel();
        type_ = var->getType();
        data_structure_type_ = var->getDSType();
        length_ = var->getLength();
    }

    Line(const std::string& name, TypeEn ty, DataStructureTypeEn dsty, uint64_t len) : ExValue_ifs() {
        names_.push_back(name);
        name_ = name;
        data_structure_type_ = dsty;
        type_ = ty;
        length_ = int64_t(len);
        is_arg_ = true;
    }

    Line(const std::string& name, const std::string& link_name, DataStructureTypeEn dsty) : ExValue_ifs() {
        names_.push_back(name);
        name_ = name;
        link_name_ = link_name;
        data_structure_type_ = dsty;
        is_arg_ = true;
    }

    Line(const std::string& name, ParameterIfs* parameter) : ExValue_ifs() {
        names_.push_back(name);
        name_ = name;
        link_name_ = parameter->getName();
        type_ = PRMType2JITType(parameter->getPrmType());
        length_ = int64_t(parameter->getVirtualSize());
        data_structure_type_ = DataStructureTypeEn::kLargeArr;
        is_arg_ = true;

        if (parameter->isAsync()) {
            parameter_ = new AsyncParameter(*(AsyncParameter*)parameter);
        } else parameter_ = new SyncParameter(*(SyncParameter*)parameter);
    }

    explicit Line(const std::string& name) : ExValue_ifs() {
        names_.push_back(name);
        name_ = name;
        type_ = TypeEn::unknown_jty;
        is_arg_ = true;
    }

    ~Line() override = default;

    void assignValue(ExValue_ifs* var);
    ExValue_ifs* getAssignedVal(bool deep = false) override;

    bool isArg() const;
    bool checkName(const std::string&) const;

    std::string getName(bool only_name = false) const { return only_name ? name_ : checkBuffer(name_); }
    std::string getLinkName() const { return link_name_; }
    NodeTypeEn getNodeType() const override { return NodeTypeEn::kLine; }
    untyped_t* getBinaryValuePtr() { return &binary_value_; }

    // safe functions .external stack is used
    void visitEnter(stack<ExValue_ifs*>* visitor_stack) override;
    void markUnusedVisitEnter(stack<ExValue_ifs*>* visitor_stack) override;

    void genBodyVisitExit(BodyGenContext* context) override;
    void printVisitExit(PrintBodyContext* context) override;
    void genBlocksVisitExit(TableGenContext* context) override;
    void setupIR(IRGenerator& builder) override;

    virtual void setTempTypeAndBinaryValue(ExValue_ifs* var) {
        temp_type_ = var->getType();
        binary_value_ = var->getBinaryValue();
    }

   private:
    uint32_t reference_ = 0;  // atavism
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

   private:
    bool is_arg_ = false;

    ExValue_ifs* assigned_val_ = nullptr;

    std::vector<std::string> names_;

    std::string name_ = std::string();
    std::string link_name_ = std::string();
};

#endif