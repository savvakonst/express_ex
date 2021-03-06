#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "TableGenContext.h"
#include "common/common.h"
#include "ifs/parameterIO.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"

using std::string;
class ExValue_ifs;

enum class CycleStageEn
{
    start,
    midle,
    end
};

namespace llvm {
class ConstantFolder;
class IRBuilderDefaultInserter;

class Module;
class LLVMContext;
class Function;
class Type;
class Value;
template <typename T = ConstantFolder, typename Inserter = IRBuilderDefaultInserter>
class IRBuilder;

namespace legacy {
class FunctionPassManager;
}
}  // namespace llvm

class IRGenerator;

typedef std::map<OpCodeEn, llvm::Function*> BuiltInFuncMap;


/**
 * Block object contains ExValue_ifs objects with the same level and length_.
 *
 */
class Block {
   public:
    explicit Block(ExValue_ifs* var);

    ~Block() = default;

    void setUint(ExValue_ifs* var);
    void setBufferLength(uint64_t buffer_length);
    uint64_t getLevel() const { return level_; };
    uint64_t getLength() const { return length_; };

    std::string print() const;

    bool generateIR(IRGenerator& builder, CycleStageEn type = CycleStageEn::midle,
                    const std::string& basic_block_prefix = "");

   private:
    stack<ExValue_ifs*> uint_list_;

    uint64_t left_length_;
    uint64_t right_length_;
    uint64_t level_;
    uint64_t length_;
    uint64_t buffer_length_ = 1 << 20;
};


/**
 * TableColumn object contains Block objects with the same length_.
 *
 */
class TableColumn {
   public:
    explicit TableColumn(uint64_t len) { length_ = len; }
    explicit TableColumn(ExValue_ifs* var);

    ~TableColumn() {
        for (auto i : block_list_) delete i;
    }

    void setUint(ExValue_ifs* var);
    void setBufferLength(uint64_t buffer_length) const {
        for (auto i : block_list_) i->setBufferLength(buffer_length);
    }

    uint64_t getLength() const { return length_; }
    uint64_t getMaxLevel() const {
        uint64_t level = 0;
        for (auto i : block_list_) {
            auto tmp = i->getLevel();
            level = level > tmp ? level : tmp;
        }
        return level;
    }
    Block* getBlock(int level) const {
        for (auto i : block_list_)
            if (i->getLength() == length_) {
                return i;
            }
        return nullptr;
    }


    std::string print();

    bool generateIR(IRGenerator& builder, CycleStageEn type = CycleStageEn::midle,
                    const std::string& basic_block_prefix = "");
    bool generateIR(IRGenerator& builder, CycleStageEn type, int64_t level, const std::string& basic_block_prefix = "");

   private:
    uint64_t length_;
    stack<Block*> block_list_;
};

class Table {
   public:
    Table(int max_buffer_length = (1 << 20), int min_buffer_length = 0) {
        max_buffer_length_ = max_buffer_length;
        min_buffer_length_ = min_buffer_length;
    }

    ~Table();

    bool containsColumn(uint64_t length) {
        for (auto i : column_list_)
            if (i->getLength() == length) return true;
        return false;
    }

    void setUint(ExValue_ifs* var);

    TableColumn* getColumn(uint64_t length) {
        for (auto i : column_list_)
            if (i->getLength() == length) {
                return i;
            }
        return nullptr;
    }
    uint64_t getMaxLevel() {
        uint64_t level = 0;
        for (auto i : column_list_) {
            auto tmp = i->getMaxLevel();
            level = level > tmp ? level : tmp;
        }
        return level;
    }

    llvm::Function* getFloatBIFunc(OpCodeEn op);
    llvm::Function* getDoubleBIFunc(OpCodeEn op);
    llvm::Function* getBIFunc(BuiltInFuncMap& UBIFMap, OpCodeEn op, llvm::Type* Ty);
    llvm::Function* getConvolveFunc(TypeEn type);
    llvm::Function* getGPUConvolveFunc(TypeEn type);
    llvm::Module* getModule() const { return M_; }

    void declareBuiltInFunctions(BuiltInFuncMap& UBIFMap, llvm::Type* Ty);

    std::string print() const;
    void calculateBufferLength(const std::string& basic_block_prefix = "");

    bool llvmInit();

    bool generateIR(const std::string& basic_block_prefix = "");

    bool runOptimization();
    bool run();

    std::string printLlvmIr();

    friend class TableGenContext;

   private:
    typedef struct {
        uint64_t max_length;
        uint64_t min_length;
        uint64_t iterations;
        stack<TableColumn*> column_list;
    } Group;

    std::vector<Group> group_list_;

    bool generateIRInGroup(Group& group, uint32_t index);
    void declareFunctions();

    // what means external ?
    llvm::Module* M_ = nullptr;             // external
    llvm::LLVMContext* context_ = nullptr;  // external

    std::unique_ptr<llvm::Module> module_u_ptr_;
    std::unique_ptr<llvm::legacy::FunctionPassManager> the_FPM_;

    llvm::Function* buffer_update_function_ = nullptr;  // external
    llvm::Function* main_function_ = nullptr;           // external

    std::map<TypeEn, llvm::Function*> convolve_map_;  // external

    IRGenerator* builder_ = nullptr;  // internal

    BuiltInFuncMap float_BI_func_map_;   // external
    BuiltInFuncMap double_BI_func_map_;  // external

    llvm::Type* fTy_ = nullptr;  // external
    llvm::Type* dTy_ = nullptr;  // external

    stack<ExValue_ifs*> const_list_;        // external
    stack<ExValue_ifs*> small_array_list_;  // external

    // uint64_t* max_column_depth_ = nullptr;
    stack<TableColumn*> column_list_;  // internal

    // int64_t top_level_ = 0ll;

    std::map<OpCodeEn, int> BIF2LLVMmap_;
    std::set<ParameterIfs*> parameter_set_;  // external

    uint64_t min_buffer_length_;
    uint64_t max_buffer_length_;
    uint64_t iterations_ = 0;

    std::string error_info_;
};

#endif
