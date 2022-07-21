#ifndef BAISIC_H
#define BAISIC_H

#include <iostream>
#include <list>
#include <set>
#include <string>

#include "body.h"
#include "common/types_jty.h"

class ExValue_ifs;
class ExLine;
class Body;

class GarbageContainer {
   public:
    ~GarbageContainer();
    /*
    ~GarbageContainer() {
        for (auto i : value_set_)
            delete i;
    }
    */
    ExValue_ifs* add(ExValue_ifs* var) {
        if (var != nullptr) value_set_.insert(var);
        return var;
    }

   protected:
    std::set<ExValue_ifs*> value_set_;
};

class CallRecursiveFunctionTemplate;
class CallTemplate;
#include <iterator>

class PrintBodyContext {
   public:
    PrintBodyContext(std::string tab, bool DST_ena, bool hide_unused_lines)
        : tab_(std::move(tab)), DST_ena_(DST_ena), hide_unused_lines_(hide_unused_lines) {}

    void createArg(ExLine* value);

    void createLine(ExLine* value);

    void createReturn(ExValue_ifs* value);

    void setName(const std::string& name);

    inline void push(const std::string& var) { string_stack_.push(var); }

    inline std::string pop() { return string_stack_.pop(); }

    inline void addVoid(const std::string& var) { return void_definition_list_.push(var); }

    inline std::string getResult() {
        std::ostringstream imploded;
        std::copy(void_definition_list_.begin(), void_definition_list_.end(),
                  std::ostream_iterator<std::string>(imploded, " \n"));

        return tab_ + "\n" + name_ + imploded.str() + "\n" + result_;
    }


    const std::string tab_;

    const bool DST_ena_;

    const bool hide_unused_lines_;

   private:
    const size_t max_line_length_ = 90;

    stack<std::string> void_definition_list_;

    stack<std::string> string_stack_;

    std::string result_;

    std::string name_;
};

// body generation context from Template
class BodyGenContext {
   public:
    BodyGenContext(Body* current_body, bool is_function)
        : is_recursive_function_(is_function),
          garbage_container_(current_body->getGarbageContainer()),
          current_body_(current_body),
          namespace_ptr_(current_body->lines_) {
        //
    }

    ~BodyGenContext() = default;

    inline void push(ExValue_ifs* var) { var_stack_.push(var); }

    inline ExValue_ifs* pop() { return var_stack_.pop(); }

    inline const std::vector<ExLine*>& getNamespace() const { return namespace_ptr_; }

    inline GarbageContainer* getGarbageContainer() const { return garbage_container_; }

    inline Body* getPureFunctionBody(const std::string& name, const Signature& signature) const {
        return current_body_->getFunctionBody(name, signature);
    }

    inline void setFunctionBody(Body* body) const { current_body_->setFunctionBody(body); }


    /* inline Body* getOrCreatePureFunctionBody(const BodyTemplate* body, const Signature& signature) const{
        current_body_->getOrCreatePureFunctionBody( body, signature);
    }*/

    inline const Body* getCurrentBody() const { return current_body_; }

    const bool is_recursive_function_;

   private:
    GarbageContainer* garbage_container_ = nullptr;

    stack<ExValue_ifs*> var_stack_;

    Body* current_body_ = nullptr;

    const std::vector<ExLine*>& namespace_ptr_;

    friend CallRecursiveFunctionTemplate;
    friend CallTemplate;
};

// constant value recursive clculation context
class RecursiveGenContext {
   public:
    explicit RecursiveGenContext(bool is_recursive, bool hide_const_values = true)
        : hide_const_values_(hide_const_values), exit_from_loop_(!is_recursive) {}

    ~RecursiveGenContext() = default;

    void setUint(ExValue_ifs* var) { instructions_list_.push(var); }

    uint32_t getReference() { return reference_cnt_++; }

    void addArg(ExValue_ifs* arg) {
        if (!hide_const_values_) instructions_list_.push(arg);
        args_reference_.push(arg);
    }

    bool exitFromLoop() const { return exit_from_loop_; }


    stack<ExValue_ifs*> instructions_list_;
    const bool hide_const_values_;

    bool exit_from_loop_;

   private:
    uint32_t reference_cnt_ = 0;

    stack<ExValue_ifs*> args_reference_;

    friend class TailCallDirectiveTemplate;

    // std::map<std::string, Value> namespace_ptr_ ;
};

extern PosInText g_pos;

class SmallArr {
   public:
    SmallArr() { pos_ = g_pos; }

    virtual ~SmallArr() { delete buffer_ptr_; }

    virtual void calculate() {}

    char* getBufferPtr() const { return buffer_ptr_; }

   protected:
    PosInText pos_;

    char* buffer_ptr_ = nullptr;
};

#endif