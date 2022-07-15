#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>

#include "basic.h"
#include "common/common.h"
#include "ifs/parameterIO.h"
#include "jit/table.h"

using std::string;
class IRGenerator;

class ExValue : public SmallArr {
   public:
    ExValue() : SmallArr(){};
    ExValue(std::string text, TypeEn type);
    ExValue(untyped_t value, TypeEn type);
    ExValue(ExValue* arg1, ExValue* arg2, ExValue* arg3);
    ExValue(ExValue* arg1, ExValue* arg2);
    explicit ExValue(ExValue* arg1);



    ~ExValue() override = default;

    virtual void setBuffered();

    void setReturned() { is_returned_ = true; }
    void setBufferLength(uint64_t central_length);
    void setBufferLength(uint64_t left, uint64_t right);
    void setBufferLength(ExValue* var);
    void setLevel(int64_t var);

    template <typename T>
    T getConvTypeVal() const {
        return *((T*)(&binary_value_));
    }
    untyped_t getBinaryValue() const { return *((untyped_t*)(&binary_value_)); }
    double getDoubleValue() const;
    std::string getTextValue() const { return text_value_; }
    std::string getTxtDSType() const;
    std::string getUniqueName() const { return unique_name_; }
    int64_t getUsageCounter() const { return int64_t(usage_counter_); }
    int64_t getLength() const { return length_; }
    int64_t getLevel() const { return level_; }
    int64_t getDecimation() const { return decimation_; }
    int64_t getBufferLen() const { return buffer_length_; }
    uint64_t getLeftBufferLen() const { return left_buffer_length_; }
    uint64_t getRightBufferLen() const { return right_buffer_length_; }
    TypeEn getType() const { return type_; }
    TypeEn getTempType() const { return isUnknownTy(type_) ? temp_type_ : type_; }
    DataStructureTypeEn getDSType() const { return data_structure_type_; }
    ParameterIfs* getParameter() const { return parameter_; }

    llvm::Value* getIRValue(IRGenerator& builder, int64_t parent_level);
    llvm::Value* getIrValueBasePtr(IRGenerator& builder, int64_t parent_level);
    llvm::Value* getIrValuePtr(IRGenerator& builder, int64_t parent_level);

    virtual NodeTypeEn getNodeType() const { return NodeTypeEn::kValue; }

    /**
     * if object is Line object it will return pointer to assigned value
     * otherwise it returns pointer to itself
     * @param deep if is true it will return first non-Line object in case when Line object points to Another Line
     * object
     * @return pointer to assigned value
     */
    virtual ExValue* getAssignedVal(bool deep = false) { return this; }

    bool isUnused() const { return is_unused_; }
    bool isArray() const { return data_structure_type_ != DataStructureTypeEn::kConstant; }
    bool isVisited() const;
    bool isBuffered() const { return is_buffered_; }
    bool isReturned() const { return is_returned_; }

    virtual bool isTerminalLargeArray() { return false; }
    // safe functions .external stack is used
    void commonMarkUnusedVisitEnter(stack<ExValue*>* visitor_stack) { usage_counter_++; }

    virtual void visitEnter(stack<ExValue*>* visitor_stack) {
        visitor_stack->push(this);
        is_visited_ = true;
    }

    virtual void markUnusedVisitEnter(stack<ExValue*>* visitor_stack) {
        commonMarkUnusedVisitEnter(visitor_stack);
        is_unused_ = false;
    }

    virtual void genBodyVisitExit(BodyGenContext* context) {
        context->push(context->getGarbageContainer()->add(new ExValue(text_value_, type_)));
        is_visited_ = false;
    }

    virtual void genBlocksVisitExit(TableGenContext* context) {
        unique_name_ = "c" + std::to_string(context->getUniqueIndex());
        context->setUint(this);
        is_visited_ = false;
    }

    virtual void printVisitExit(PrintBodyContext* context) {
        context->push(text_value_);
        is_visited_ = false;
    }

    virtual void genRecursiveVisitExit(RecursiveGenContext* context) {
        // context->setUint(this);  //not used. this should be removed.
        if (!context->hide_const_values_) context->setUint(this);
        is_visited_ = false;
    }

    virtual void calculateConstRecursive(RecursiveGenContext* context) {}

    virtual std::string printUint() { return unique_name_ + "=" + text_value_; }
    virtual void setupIR(IRGenerator& builder);

    void calculate() override;

    std::string printSmallArray();

   protected:
    std::string checkBuffer(std::string arg) const {
        if (is_buffered_) return "storeToBuffer(" + arg + ")";
        else return arg;
    }

    bool is_unused_ = true;
    bool is_visited_ = false;
    bool is_buffered_ = false;

    bool is_returned_ = false;
    bool is_initialized_ = false;

    DataStructureTypeEn data_structure_type_ = DataStructureTypeEn::kConstant;

    const TypeEn type_ = TypeEn::DEFAULT_JTY;
    TypeEn temp_type_ = TypeEn::DEFAULT_JTY;

    std::string text_value_;
    std::string unique_name_;

    int64_t length_ = 1;
    int64_t decimation_ = 0;
    int64_t level_ = 0;

    uint64_t left_buffer_length_ = 0;
    uint64_t right_buffer_length_ = 0;

    int64_t buffer_length_ = 0;

    untyped_t binary_value_ = 0;
    uint64_t usage_counter_ = 0;

    uint64_t buffer_num_ = 0;  // unused

    llvm::Value* IR_value_ = nullptr;
    llvm::Value* IR_loaded_buffer_value_ = nullptr;
    llvm::Value* IR_buffer_ptr_ = nullptr;
    llvm::Value* IR_buffer_base_ptr_ = nullptr;

    ParameterIfs* parameter_ = nullptr;
    friend class TailCallDirectiveTemplate;
};

inline bool operator==(const ExValue* var_a, DataStructureTypeEn var_b) { return var_a->getDSType() == var_b; }
inline bool operator==(DataStructureTypeEn var_a, const ExValue* var_b) { return var_a == var_b->getDSType(); }
inline bool operator<(TypeEn var_a, const ExValue* var_b) { return var_a < var_b->getType(); }
inline bool operator<(const ExValue* var_a, TypeEn var_b) { return var_a->getType() < var_b; }

inline bool isConst(const ExValue* var_a) { return var_a == DataStructureTypeEn::kConstant; }
inline bool isVariable(const ExValue* var_a) { return var_a == DataStructureTypeEn::kVariable; }
inline bool isSmallArr(const ExValue* var_a) { return var_a == DataStructureTypeEn::kSmallArr; }
inline bool isLargeArr(const ExValue* var_a) { return var_a == DataStructureTypeEn::kLargeArr; }
inline bool isSimilar(const ExValue* var_a, const ExValue* var_b) {
    return (var_a->getDSType() == var_b->getDSType() && (var_a->getLength() == var_b->getLength()));
}
inline bool isCompatible(const ExValue* var_a, const ExValue* var_b) {
    return isConst(var_a) || isConst(var_b) || isVariable(var_a) || isVariable(var_b) || isSimilar(var_a, var_b);
}

inline bool isUnknownTy(const ExValue* var) { return isUnknownTy(var->getType()); }
inline bool isFloating(const ExValue* var) { return isFloating(var->getType()); }
inline bool isInteger(const ExValue* var) { return isInteger(var->getType()); }
inline bool isUInteger(const ExValue* var) { return isUInteger(var->getType()); }
inline bool isBool(const ExValue* var) { return isBool(var->getType()); }

inline ExValue* maxTypeVar(ExValue* var_a, ExValue* var_b) {
    return var_a->getType() < var_b->getType() ? var_b : var_a;
}
inline ExValue* maxTempTypeVar(ExValue* var_a, ExValue* var_b) {
    return var_a->getTempType() < var_b->getTempType() ? var_b : var_a;
}
inline ExValue* minTypeVar(ExValue* var_a, ExValue* var_b) {
    return var_a->getType() < var_b->getType() ? var_a : var_b;
}
inline ExValue* maxTypeVar(std::vector<ExValue*> args) {
    ExValue* var = args[0];
    for (auto i : args) var = maxTypeVar(i, var);
    return var;
}  // unsafe function .zero size array check missing

inline ExValue* maxDSVar(ExValue* var_a, ExValue* var_b) {
    return var_a->getDSType() < var_b->getDSType() ? var_b : var_a;
}
inline ExValue* maxLevelVar(ExValue* var_a, ExValue* var_b) {
    return var_a->getLevel() < var_b->getLevel() ? var_b : var_a;
}
inline ExValue* minLevelVar(ExValue* var_a, ExValue* var_b) {
    return var_a->getLevel() < var_b->getLevel() ? var_a : var_b;
}

#endif