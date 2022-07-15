#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>

#include "common/common.h"
#include "ifs/parameterIO.h"
#include "jit/table.h"
#include "parser/basic.h"

using std::string;
class IRGenerator;

class ExValue_ifs : public SmallArr {
   public:
    ExValue_ifs() : SmallArr(){};

    // ExValue()  = delete;
    ExValue_ifs(TypeEn type, TypeEn time_type, DataStructureTypeEn data_structure_type_)
        : SmallArr(), type_(type), time_type_(time_type), data_structure_type_(data_structure_type_){};

    // ExValue_ifs(std::string text, TypeEn type);
    // ExValue_ifs(untyped_t value, TypeEn type);
    // ExValue_ifs(ExValue_ifs* arg_1, ExValue_ifs* arg_2, ExValue_ifs* arg_3);
    // ExValue_ifs(ExValue_ifs* arg_1, ExValue_ifs* arg_2);
    // explicit ExValue_ifs(ExValue_ifs* arg_1);



    ~ExValue_ifs() override = default;

    virtual void setBuffered();

    void setReturned() { is_returned_ = true; }
    void setBufferLength(uint64_t central_length);
    void setBufferLength(uint64_t left, uint64_t right);
    void setBufferLength(ExValue_ifs* var);
    void setLevel(int64_t var);


    untyped_t getBinaryValue() const { return *((untyped_t*)(&binary_value_)); }

    double getDoubleValue() const;
    std::string getTxtDSType() const;
    std::string getUniqueName() const { return unique_name_; }
    int64_t getUsageCounter() const { return int64_t(usage_counter_); }
    int64_t getLength() const { return length_; }
    int64_t getLevel() const { return level_; }
    int64_t getDecimation() const { return decimation_; }
    int64_t getBufferLen() const { return buffer_length_; }
    uint64_t getLeftBufferLen() const { return left_buffer_length_; }
    uint64_t getRightBufferLen() const { return right_buffer_length_; }

    bool isSync() const { return time_type_ == TypeEn::unknown_jty; }
    TypeEn getTimeType() const { return time_type_; }
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
    virtual ExValue_ifs* getAssignedVal(bool deep = false) { return this; }

    bool isUnused() const { return is_unused_; }
    bool isArray() const { return data_structure_type_ != DataStructureTypeEn::kConstant; }
    bool isVisited() const;
    bool isBuffered() const { return is_buffered_; }
    bool isReturned() const { return is_returned_; }

    virtual bool isTerminalLargeArray() { return false; }
    // safe functions .external stack is used
    void commonMarkUnusedVisitEnter(stack<ExValue_ifs*>* visitor_stack) { usage_counter_++; }

    virtual void visitEnter(stack<ExValue_ifs*>* visitor_stack) {
        visitor_stack->push(this);
        is_visited_ = true;
    }

    virtual void markUnusedVisitEnter(stack<ExValue_ifs*>* visitor_stack) {
        commonMarkUnusedVisitEnter(visitor_stack);
        is_unused_ = false;
    }


    virtual void genBodyVisitExit(BodyGenContext* context) = 0;
    /*
    virtual void genBodyVisitExit(BodyGenContext* context) {
        context->push(context->getGarbageContainer()->add(new ExValue_ifs(text_value_, type_)));
        is_visited_ = false;
    }
    */


    virtual void genBlocksVisitExit(TableGenContext* context) = 0;
    /*
virtual void genBlocksVisitExit(TableGenContext* context) {
    unique_name_ = "c" + std::to_string(context->getUniqueIndex());
    context->setUint(this);
    is_visited_ = false;
}
*/

    virtual void printVisitExit(PrintBodyContext* context) = 0;
    /*virtual void printVisitExit(PrintBodyContext* context) {
        context->push(text_value_);
        is_visited_ = false;
    }
    */

    virtual void genRecursiveVisitExit(RecursiveGenContext* context) = 0;
    /*
    virtual void genRecursiveVisitExit(RecursiveGenContext* context) {
        // context->setUint(this);  //not used. this should be removed.
        if (!context->hide_const_values_) context->setUint(this);
        is_visited_ = false;
    }
    */

    virtual void calculateConstRecursive(RecursiveGenContext* context) {}


    virtual std::string printUint() = 0;
    // virtual std::string printUint() { return unique_name_ + "=" + text_value_; }


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



    TypeEn time_type_ = TypeEn::DEFAULT_JTY;
    TypeEn type_ = TypeEn::DEFAULT_JTY;

    /**
     * it is used to const value calculation
     * inside functions with recursive tail call
     */
    TypeEn temp_type_ = TypeEn::DEFAULT_JTY;


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

inline bool operator==(const ExValue_ifs* var_a, DataStructureTypeEn var_b) { return var_a->getDSType() == var_b; }
inline bool operator==(DataStructureTypeEn var_a, const ExValue_ifs* var_b) { return var_a == var_b->getDSType(); }
inline bool operator<(TypeEn var_a, const ExValue_ifs* var_b) { return var_a < var_b->getType(); }
inline bool operator<(const ExValue_ifs* var_a, TypeEn var_b) { return var_a->getType() < var_b; }

inline bool isConst(const ExValue_ifs* var_a) { return var_a == DataStructureTypeEn::kConstant; }
inline bool isVariable(const ExValue_ifs* var_a) { return var_a == DataStructureTypeEn::kVariable; }
inline bool isSmallArr(const ExValue_ifs* var_a) { return var_a == DataStructureTypeEn::kSmallArr; }
inline bool isLargeArr(const ExValue_ifs* var_a) { return var_a == DataStructureTypeEn::kLargeArr; }
inline bool isSimilar(const ExValue_ifs* var_a, const ExValue_ifs* var_b) {
    return (var_a->getDSType() == var_b->getDSType() && (var_a->getLength() == var_b->getLength()));
}
inline bool isCompatible(const ExValue_ifs* var_a, const ExValue_ifs* var_b) {
    return isConst(var_a) || isConst(var_b) || isVariable(var_a) || isVariable(var_b) || isSimilar(var_a, var_b);
}

inline bool isUnknownTy(const ExValue_ifs* var) { return isUnknownTy(var->getType()); }
inline bool isFloating(const ExValue_ifs* var) { return isFloating(var->getType()); }
inline bool isInteger(const ExValue_ifs* var) { return isInteger(var->getType()); }
inline bool isUInteger(const ExValue_ifs* var) { return isUInteger(var->getType()); }
inline bool isBool(const ExValue_ifs* var) { return isBool(var->getType()); }

inline ExValue_ifs* maxTypeVar(ExValue_ifs* var_a, ExValue_ifs* var_b) {
    return var_a->getType() < var_b->getType() ? var_b : var_a;
}
inline ExValue_ifs* maxTempTypeVar(ExValue_ifs* var_a, ExValue_ifs* var_b) {
    return var_a->getTempType() < var_b->getTempType() ? var_b : var_a;
}
inline ExValue_ifs* minTypeVar(ExValue_ifs* var_a, ExValue_ifs* var_b) {
    return var_a->getType() < var_b->getType() ? var_a : var_b;
}
inline ExValue_ifs* maxTypeVar(std::vector<ExValue_ifs*> args) {
    ExValue_ifs* var = args[0];
    for (auto i : args) var = maxTypeVar(i, var);
    return var;
}  // unsafe function .zero size array check missing

inline ExValue_ifs* maxDSVar(ExValue_ifs* var_a, ExValue_ifs* var_b) {
    return var_a->getDSType() < var_b->getDSType() ? var_b : var_a;
}
inline ExValue_ifs* maxLevelVar(ExValue_ifs* var_a, ExValue_ifs* var_b) {
    return var_a->getLevel() < var_b->getLevel() ? var_b : var_a;
}
inline ExValue_ifs* minLevelVar(ExValue_ifs* var_a, ExValue_ifs* var_b) {
    return var_a->getLevel() < var_b->getLevel() ? var_a : var_b;
}

#endif