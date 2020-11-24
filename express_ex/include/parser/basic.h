#ifndef BAISIC_H
#define BAISIC_H


#include <iostream>
#include <string>
#include <set>
#include "types_jty.h"


class Variable;
class Body;

class GarbageContainer
{
public:
    ~GarbageContainer();
    /*
    ~GarbageContainer() {
        for (auto i : variable_set_)
            delete i;
    }
    */
    Variable* add(Variable* var) {
        if (var != nullptr)
            variable_set_.insert(var);
        return var;
    }

protected:
    std::set<Variable*> variable_set_;
};

class BodyGenContext {
public:
    BodyGenContext (stack<Variable*>* varStack, std::vector<Line*>* namespace_ptr, bool isPrototype, GarbageContainer* garbage_container) {
        var_stack_ = varStack;
        namespace_ptr_ = namespace_ptr;
        is_prototype_ = isPrototype;
        garbage_container_ = garbage_container;
    }
    BodyGenContext (Body & body) {}
    ~BodyGenContext() {}


    inline void push(Variable* var) { var_stack_->push(var); }
    inline Variable*  pop() { return var_stack_->pop(); }
    inline std::vector< Line*> &getNamespace() { return *namespace_ptr_; }
    inline GarbageContainer* getGarbageContainer() { return garbage_container_; }
    inline bool isPrototype() { return is_prototype_; }
private:
    GarbageContainer*   garbage_container_ = nullptr;
    stack<Variable*>*   var_stack_ = nullptr;
    std::vector<Line*>* namespace_ptr_ = nullptr;
    bool is_prototype_ = false;
};

extern PosInText g_pos;

class SmallArr {
public:
    SmallArr() {
        pos=g_pos;
    }

    ~SmallArr() {
        if (bufferPtr_ != nullptr)
            delete bufferPtr_;
    }

    void genRange() {};

    void loadFromFile(const std::string &filename) {
    };

    virtual void calculate() {}
    char * getBufferPtr() {
        return bufferPtr_;
    }

protected:
    PosInText pos;
    double start_ = 0;
    double stop_ = 0;

    char * bufferPtr_ = nullptr;
};




#endif