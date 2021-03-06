#ifndef BODY_TEMPLATE_H
#define BODY_TEMPLATE_H

#include <iostream>
#include <list>
#include <map>
#include <vector>

#include "common/common.h"

class BodyGenContext;
class GarbageContainer;
class ExValue_ifs;
class ExLine;
class Body;


/**
 * BodyTemplate's object represents/contains template (type less) ASG
 * (Abstract semantic graph) unit. This class has different
 * methods for building, printing, template ASG and method for
 * producing main(type full) AST.
 *
 */
class BodyTemplate {
   public:
    BodyTemplate(std::string name, BodyTemplate* parent_body_template);
    ~BodyTemplate();


    bool isTopBody() const { return name_ == "main"; }
    bool isRetStackFull() const { return !return_stack_.empty() && !isTopBody(); }
    bool isRetStackEmpty() const { return return_stack_.empty(); }



    void push(ExValue_ifs*);
    ExValue_ifs* pop();
    stack<ExValue_ifs*> pop(size_t length);

    void addLine(ExLine* line);

    // TODO is necessary to add returned status_ value with line ,pos end error code and string;
    void addReturn();

    // create call
    void addCall(BodyTemplate* body);
    void addTailCall();

    std::map<std::string /*name*/, std::string /*link name*/> getParameterLinkNames(bool hide_unused = false) const;

    const stack<ExValue_ifs*>& getRet() const { return return_stack_; }

    int getArgCount() const { return arg_count_; }

    ExLine* getLastReferenceByName(const std::string& name) const;

    const std::string& getName() const { return name_; }

    BodyTemplate* getParent() const { return parent_body_template_; }

    BodyTemplate* getFunctionBody(const std::string& name) const;

    GarbageContainer* getGarbageContainer() const { return garbage_container_; }

    // tree walker methods

    std::string print(const std::string& tab = "", bool dst_ena = false, bool hide_unused_lines = false) const;

    std::list<std::string> getNamesOfDefinedFunctions() const;

    Body* genBodyByTemplate(Body* parent_body, stack<ExValue_ifs*> args, bool is_recursive_function) const;

    untyped_t genConstRecursiveByTemplate(stack<ExValue_ifs*>& args) const;

    std::list<BodyTemplate*> child_body_template_list_;

    /**
     * if any operator occurs in corresponding ASG unit or any
     * child BodyTemplate objects are marked as operator
     * this field is set to true, It is necessary to determine
     * that function which is representing by current ASG unit is pure
     * and it is possible to use tail call or anything else...
     */
    bool is_operator_ = false;

    /**
     * if any tail call occurs inside body it sets to true,
     * otherwise it equal false
     */
    bool is_tail_callable_ = false;


    bool is_recurrence_relation_ = false;


   private:
    const std::string name_;

    int arg_count_ = 0;

    stack<ExValue_ifs*> var_stack_;
    stack<ExLine*> references_;
    stack<ExValue_ifs*> return_stack_;

    BodyTemplate* parent_body_template_ = nullptr;
    GarbageContainer* garbage_container_;

    friend BodyGenContext;
};

#endif  // !BODY_TEMPLATE_H
