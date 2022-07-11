
#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include <iostream>
#include <vector>

#include "buffer.h"
#include "common/types_jty.h"
#include "common/undefWarningIgnore.h"
#include "llvm/IR/IRBuilder.h"
#include "parser/defWarningIgnore.h"

class Table;
class ExValue;

inline llvm::raw_ostream& operator<<(llvm::raw_ostream& OS, Buffer& arg) {
    arg.stream(OS);
    return OS;
}

class IRGenerator : public llvm::IRBuilder<> {
   public:
    IRGenerator(llvm::LLVMContext& context, Table* table, bool is_pure_function = false);
    ~IRGenerator();

    bool checkExistence(llvm::Value* var) const {
        for (auto& i : initialized_value_list_)
            if (i == var) return true;
        return false;
    }


    void addInitializedValue(llvm::Value* var) { initialized_value_list_.push_back(var); }
    void clearInitializedValueList() { initialized_value_list_.clear(); }

    llvm::Value* createFPow(llvm::Value* a_operand, llvm::Value* b_operand, const std::string& name = "");
    llvm::Value* createConst(int64_t& binary_value, TypeEn target_ty, const std::string& name = "");
    llvm::Value* createArithmetic(llvm::Value* a_operand, llvm::Value* b_operand, OpCodeEn op_code,
                                  const std::string& name = "");
    llvm::Value* createComparison(llvm::Value* a_operand, llvm::Value* b_operand, OpCodeEn op_code,
                                  const std::string& name = "");
    llvm::Value* createInv(llvm::Value* a_operand, OpCodeEn op_code, const std::string& name = "");
    llvm::Value* createTypeConv(llvm::Value* a_operand, OpCodeEn op_code, TypeEn target_ty,
                                const std::string& name = "");
    llvm::Value* createBuiltInFunc(llvm::Value* a_operand, OpCodeEn op_code, const std::string& name = "");
    llvm::Value* createConvolve(llvm::Value* a_operand, char* ptr, int64_t length, int64_t shift, TypeEn type,
                                const std::string& name = "");
    llvm::Value* createCall(llvm::Value* callee, llvm::ArrayRef<llvm::Value*> args = {}, const std::string& name = "");
    llvm::Value* createConvolve(llvm::Value* a_operand, llvm::Value* b_operand, const std::string& name = "");
    llvm::Value* createGpuConvolve(llvm::Value* a_operand, char* ptr, int64_t length, int64_t shift, TypeEn type,
                                   const std::string& name = "");
    llvm::Value* createPositionalAlloca(llvm::Type* a_operand, int64_t i, const std::string& name = "");
    llvm::Value* createPositionalOffsetAlloca(const std::string& name = "", int64_t start_value = 0);
    llvm::Value* createPositionalInBoundsGep(llvm::Value* ptr, llvm::ArrayRef<llvm::Value*> idx_list,
                                             const std::string& name = "");
    llvm::Value* createPositionalLoad(llvm::Value* a_operand, const std::string& name = "");
    llvm::Value* createPositionalLoad(llvm::Value* a_operand, bool is_volatile, const std::string& name = "");
    llvm::Value* createLoadOffset(const std::string& name = "common_offset_");
    void createPositionalStore(llvm::Value* a_operand, llvm::Value* ptr, bool is_volatile = false);
    llvm::Value* createBufferInit(TypeEn target_ty, const std::string& name = "");
    void createStartBRs();
    void createMiddleBRs();

    void* addBuffer(Buffer* s);

    /**
     * allocates memory for buffer
     * @return  llvm pointer to allocated memory
     */
    llvm::Value* createLocalBuffer(TypeEn target_ty, size_t len, const std::string& name = "");



    llvm::BasicBlock* createNewIntermediateBb(const std::string& postfix) {
        return createNewBb(init_bock_, "intermediate_" + postfix);
    }

    llvm::BasicBlock* createNewLoadBb(const std::string& postfix) {
        return createNewBb(load_block_, "load_" + postfix);
    }

    llvm::BasicBlock* createNewCalcBb(const std::string& postfix) {
        return createNewBb(calc_block_, "calc_" + postfix);
    }

    llvm::BasicBlock* CreateNewStoreBB(const std::string& postfix) {
        return createNewBb(store_block_, "store_" + postfix);
    }


    llvm::BasicBlock* setInitInsertPoint(llvm::BasicBlock* bb = nullptr) { return setCInsertPoint(init_bock_, bb); }

    /**
     * Setup commands insertion point in the first part of loop body - load_block.
     * Loop body consists of the three parts: load_block, calc_block, store_block
     * @param bb when bb is not equal nullptr it stores bb value and setup setup command insertion point to bb, if
     * bb is omitted or equal nullptr it setup command insertion point to last stored bb
     * @return last insert block
     */
    llvm::BasicBlock* setLoadInsertPoint(llvm::BasicBlock* bb = nullptr) { return setCInsertPoint(load_block_, bb); }

    /**
     * Setup commands insertion point in the middle of loop body - calc_block.
     * Loop body consists of the three parts: load_block, calc_block, store_block.
     * @param bb when bb is not equal nullptr it stores bb value and setup setup command insertion point to bb, if
     * bb is omitted or equal nullptr it setup command insertion point to last stored bb
     * @return last insert block
     */
    llvm::BasicBlock* setCalcInsertPoint(llvm::BasicBlock* bb = nullptr) { return setCInsertPoint(calc_block_, bb); }

    /**
     * Setup commands insertion point in the end of loop body - store_block.
     * Loop body consists of the three parts: load_block, calc_block, store_block
     * @param bb when bb is not equal nullptr it stores bb value and setup setup command insertion point to bb, if
     * bb is omitted or equal nullptr it setup command insertion point to last stored bb
     * @return last insert block
     */
    llvm::BasicBlock* setStoreInsertPoint(llvm::BasicBlock* bb = nullptr) { return setCInsertPoint(store_block_, bb); }


    llvm::BasicBlock* setTerminalOpInsertPoint(llvm::BasicBlock* bb = nullptr) {
        return setCInsertPoint(terminal_op_block_, bb);
    }


    llvm::BasicBlock* setLoopEnterInsertPoint(llvm::BasicBlock* bb = nullptr) {
        return setCInsertPoint(loop_enter_block_, bb);
    }

    llvm::BasicBlock* setIntermediateInsertPoint(llvm::BasicBlock* bb = nullptr) {
        return setCInsertPoint(intermediate_block_, bb);
    }

    llvm::BasicBlock* setCycleExitInsertPoint(llvm::BasicBlock* bb = nullptr) {
        return setCInsertPoint(cycle_exit_block_, bb);
    }

    llvm::BasicBlock* setExitInsertPoint(llvm::BasicBlock* bb = nullptr) { return setCInsertPoint(exit_block_, bb); }

    void setLastInsertPoint() { SetInsertPoint(bb_List_.back()); }

    void setCurrentFunction(llvm::Function* current_function) { current_function_ = current_function; }
    void setBufferUpdateFunction(llvm::Value* buffer_update_function) {
        buffer_update_function_ = buffer_update_function;
    }
    void setDeclareConvolve(llvm::Type* type, uintptr_t addr);

    void setCurrentOffsetValue(llvm::Value* current_offset_value) { current_offset_value_ = current_offset_value_; }
    void setCurrentCmpRes(llvm::Value* current_cmp_res) { current_cmp_res_ = current_cmp_res; }

    void setOffsetToZero();

    void setOffsetTo(int64_t val);

    void dropBaseInsertPoint() {
        calc_block_ = nullptr;
        load_block_ = nullptr;
        store_block_ = nullptr;
        intermediate_block_ = nullptr;
    };

    llvm::BasicBlock* getInitBlock() const { return init_bock_; }
    llvm::BasicBlock* getLoadBlock() const { return load_block_; }
    llvm::BasicBlock* getCalcBlock() const { return calc_block_; }
    llvm::BasicBlock* getStoreBlock() const { return store_block_; }
    llvm::BasicBlock* getTerminalOpBlock() const { return terminal_op_block_; }
    llvm::BasicBlock* getCurrentBlock() const { return BB; }
    llvm::BasicBlock* getExitBlock() const { return exit_block_; }

    llvm::BasicBlock* getBlock(int n) const { return bb_List_[n]; }
    llvm::BasicBlock* getLastBlock(int n) const { return bb_List_.back(); }

    llvm::Value* getCurrentOffsetValueAlloca() const { return current_offset_value_alloca_; }
    llvm::Value* getCurrentOffsetValue() const { return current_offset_value_; }
    llvm::Value* getCurrentCMPRes() const { return current_cmp_res_; }

    llvm::Type* getLLVMType(TypeEn target_ty);
    llvm::Function* getCurrentFunction() const { return current_function_; }
    llvm::Module* getCurrentModule();

    std::vector<std::list<Buffer*>*>* getBufferList(BufferTypeEn buffer_type = BufferTypeEn::input) {
        return &buffer_list_;
    }

    void nextBufferGroup() { buffer_list_.push_back(new std::list<Buffer*>()); }

    std::vector<llvm::Value*> arg_ptr_list_;
    const bool is_pure_function_;

   private:
    typedef llvm::BasicBlock* basicBlockPtr_t;

    llvm::BasicBlock* createNewBb(basicBlockPtr_t& prot, const std::string& name = "");

    llvm::BasicBlock* setCInsertPoint(basicBlockPtr_t& prot, llvm::BasicBlock* bb = nullptr) {
        auto last_block = GetInsertBlock();
        if (bb == nullptr) bb = prot;
        else {
            prot = bb;
            bb_List_.push_back(bb);
        }
        SetInsertPoint(bb);
        return last_block;
    }

    std::vector<std::list<Buffer*>*> buffer_list_;

    std::vector<char*> local_buffer_list_;
    // std::vector<std::list<Buffer*>*> buffer_list_;

    llvm::BasicBlock* init_bock_ = nullptr;
    llvm::BasicBlock* calc_block_ = nullptr;
    llvm::BasicBlock* load_block_ = nullptr;
    llvm::BasicBlock* store_block_ = nullptr;
    llvm::BasicBlock* terminal_op_block_ = nullptr;
    llvm::BasicBlock* intermediate_block_ = nullptr;
    llvm::BasicBlock* loop_enter_block_ = nullptr;
    llvm::BasicBlock* cycle_exit_block_ = nullptr;
    llvm::BasicBlock* exit_block_ = nullptr;

    std::vector<llvm::BasicBlock*> bb_List_;

    //  list of values that are already loaded in current loop block,
    //  this list should be cleared every time in the transition to a next block.
    std::vector<llvm::Value*> initialized_value_list_;

    llvm::Value* convolve_double_function_ = nullptr;
    llvm::Value* convolve_float_function_ = nullptr;

    llvm::Value* convolve_i64_function_ = nullptr;
    llvm::Value* convolve_i32_function_ = nullptr;

    llvm::Value* current_offset_value_alloca_ = nullptr;
    llvm::Value* current_offset_value_ = nullptr;
    llvm::Value* current_cmp_res_ = nullptr;

    Table* table_ = nullptr;

    llvm::Function* current_function_ = nullptr;
    llvm::Value* buffer_update_function_ = nullptr;

    std::vector<llvm::Value*> buffers_List_;
    // std::vector <void *> bufferList;
};

#endif  // !IR_GENERATOR_H
