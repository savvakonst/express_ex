#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>

#include "common/common.h"
#include "ifs/parameterIO.h"
#include "parser/basic.h"

namespace llvm {
class Value;
}

using std::string;
class IRGenerator;

class ExValue_ifs : public SmallArr {
   public:
    typedef int64_t length_t;

    ExValue_ifs() : SmallArr(){};


    ExValue_ifs(TypeEn type, TypeEn time_type, DataStructureTypeEn data_structure_type, length_t length)
        : SmallArr(), type_(type), time_type_(time_type), ds_ty_(data_structure_type), length_(length){};


    ~ExValue_ifs() override = default;

    virtual void setBuffered();

    void setReturned() { is_returned_ = true; }

    /**
     * it is called on Table calculateBufferLength stage, and sets buffer_length_
     * @param central_length
     */
    void setBufferLength(uint64_t central_length);


    void setBufferBordersLength(uint64_t left, uint64_t right);

    void setBufferBordersLength(ExValue_ifs* var);


    untyped_t getBinaryValue() const { return *((untyped_t*)(&binary_value_)); }

    std::string getUniqueName() const { return unique_name_; }

    length_t getLength() const { return length_; }
    int64_t getLevel() const { return level_; }

    int64_t getBufferLen() const { return buffer_length_; }
    uint64_t getLeftBufferLen() const { return left_buffer_length_; }
    uint64_t getRightBufferLen() const { return right_buffer_length_; }

    bool isSync() const { return time_type_ == TypeEn::unknown_jty; }
    TypeEn getTempType() const { return isUnknownTy(type_) ? temp_type_ : type_; }

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
    bool isArray() const { return ds_ty_ != DataStructureTypeEn::kConstant; }
    bool isVisited() const;
    bool isBuffered() const { return is_buffered_; }
    bool isReturned() const { return is_returned_; }

    void commonMarkUnusedVisitEnter(stack<ExValue_ifs*>* visitor_stack) { usage_counter_++; }

    /**
     * it sets is_visited_ to true if set_visited = true and pushes themself to visitor_stack
     */
    virtual void visitEnter(stack<ExValue_ifs*>* visitor_stack, bool set_visited = true) = 0;

    virtual void reverseTraversalVisitEnter(stack<ExValue_ifs*>* visitor_stack) {
        commonMarkUnusedVisitEnter(visitor_stack);
        is_unused_ = false;
    }

    virtual void genBodyVisitExit(BodyGenContext* context) = 0;

    virtual void genBlocksVisitExit(TableGenContext* context) = 0;

    virtual void printVisitExit(PrintBodyContext* context) = 0;

    virtual void genRecursiveVisitExit(RecursiveGenContext* context) = 0;

    virtual void calculateConstRecursive(RecursiveGenContext* context) {}

    virtual std::string printUint() = 0;

    virtual void setupIR(IRGenerator& builder) = 0;

    void calculate() override;


    /**
     * data structure type
     */
    const DataStructureTypeEn ds_ty_ = DataStructureTypeEn::kConstant;


    const TypeEn time_type_ = TypeEn::unknown_jty;
    const TypeEn type_ = TypeEn::unknown_jty;

    /**
     * it is used to const value calculation
     * inside functions with recursive tail call
     */
    TypeEn temp_type_ = TypeEn::unknown_jty;


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



    std::string unique_name_;

    /**
     * for synchronous kLargeArr it represents frequency of data,
     * otherwise it represents length of data
     */
    const length_t length_ = 1;

    /**
     * it is used to determine when buffer usage is necessary, any operand that doesn't have the same level_ is marked
     * as buffered by setting the operand's is_buffered_ field to true;
     */
    int64_t level_ = 0;

    /**
     * it is set only when Table::reverseTraversal() is called,
     * by calling setBufferBordersLength().
     */
    uint64_t left_buffer_length_ = 0;

    /**
     * it is set only when Table::reverseTraversal() is called,
     * by calling setBufferBordersLength().
     */
    uint64_t right_buffer_length_ = 0;



    /**
     * it is set oly by setBufferLength on Table calculateBufferLength stage
     * and only for
     *
     * \verbatim embed:rst:leading-asterisk
     *   .. uml::
     *
     *       @startuml
     *
     *       Express_ex -> Table::calculateBufferLength
     *       activate Table
     *           Table -> TableColumn::setBufferLength
     *           activate TableColumn
     *               TableColumn -> Block::setBufferLength
     *               activate Block
     *                   Block ->  ExValue_ifs::setBufferLength
     *               deactivate
     *           deactivate
     *       deactivate Table
     *       Express_ex -> Table::llvmInit
     *       Express_ex -> Table::generateIR
     *
     *       @enduml
     *
     * \endverbatim
     */
    uint64_t buffer_length_ = 0;

    untyped_t binary_value_ = 0;
    uint64_t usage_counter_ = 0;


    llvm::Value* IR_value_ = nullptr;
    llvm::Value* IR_loaded_buffer_value_ = nullptr;
    llvm::Value* IR_buffer_ptr_ = nullptr;
    llvm::Value* IR_buffer_base_ptr_ = nullptr;

    ParameterIfs* parameter_ = nullptr;
    friend class TailCallDirectiveTemplate;
    friend class RecurrenceRelationTemplate;
};

//   inline bool operator==(const ExValue_ifs* var_a, DataStructureTypeEn var_b) { return var_a->ds_ty_ == var_b; }
//   inline bool operator==(DataStructureTypeEn var_a, const ExValue_ifs* var_b) { return var_a == var_b->ds_ty_; }
//   inline bool operator<(TypeEn var_a, const ExValue_ifs* var_b) { return var_a < var_b->type_; }
//   inline bool operator<(const ExValue_ifs* var_a, TypeEn var_b) { return var_a->type_ < var_b; }

inline bool isConst(const ExValue_ifs* var_a) { return var_a->ds_ty_ == DataStructureTypeEn::kConstant; }
inline bool isVariable(const ExValue_ifs* var_a) { return var_a->ds_ty_ == DataStructureTypeEn::kVariable; }
inline bool isSmallArr(const ExValue_ifs* var_a) { return var_a->ds_ty_ == DataStructureTypeEn::kSmallArr; }
inline bool isLargeArr(const ExValue_ifs* var_a) { return var_a->ds_ty_ == DataStructureTypeEn::kLargeArr; }
inline bool isSimilar(const ExValue_ifs* var_a, const ExValue_ifs* var_b) {
    return (var_a->ds_ty_ == var_b->ds_ty_ && (var_a->getLength() == var_b->getLength()));
}
inline bool isCompatible(const ExValue_ifs* var_a, const ExValue_ifs* var_b) {
    return isConst(var_a) || isConst(var_b) || isVariable(var_a) || isVariable(var_b) || isSimilar(var_a, var_b);
}

inline bool isUnknownTy(const ExValue_ifs* var) { return isUnknownTy(var->type_); }
inline bool isFloating(const ExValue_ifs* var) { return isFloating(var->type_); }
inline bool isInteger(const ExValue_ifs* var) { return isInteger(var->type_); }
inline bool isUInteger(const ExValue_ifs* var) { return isUInteger(var->type_); }
inline bool isBool(const ExValue_ifs* var) { return isBool(var->type_); }

inline ExValue_ifs* maxTypeVar(ExValue_ifs* var_a, ExValue_ifs* var_b) {
    return var_a->type_ < var_b->type_ ? var_b : var_a;
}
inline ExValue_ifs* maxTempTypeVar(ExValue_ifs* var_a, ExValue_ifs* var_b) {
    return var_a->getTempType() < var_b->getTempType() ? var_b : var_a;
}
inline ExValue_ifs* minTypeVar(ExValue_ifs* var_a, ExValue_ifs* var_b) {
    return var_a->type_ < var_b->type_ ? var_a : var_b;
}
inline ExValue_ifs* maxTypeVar(std::vector<ExValue_ifs*> args) {
    ExValue_ifs* var = args[0];
    for (auto i : args) var = maxTypeVar(i, var);
    return var;
}  // unsafe function .zero size array check missing

inline ExValue_ifs* maxDSVar(ExValue_ifs* var_a, ExValue_ifs* var_b) {
    return var_a->ds_ty_ < var_b->ds_ty_ ? var_b : var_a;
}
inline ExValue_ifs* maxLevelVar(ExValue_ifs* var_a, ExValue_ifs* var_b) {
    return var_a->getLevel() < var_b->getLevel() ? var_b : var_a;
}
inline ExValue_ifs* minLevelVar(ExValue_ifs* var_a, ExValue_ifs* var_b) {
    return var_a->getLevel() < var_b->getLevel() ? var_a : var_b;
}

#endif