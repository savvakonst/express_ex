#include "parser/body.h"

#include <list>
#include <string>
#include <utility>

#include "jit/IR_generator.h"
#include "jit/TableGenContext.h"
#include "operations/ExLine.h"
#include "parser/bodyTemplate.h"

Body::Body(std::string name, const std::list<std::string>& names_of_defined_functions, Body* parent, bool is_operator)
    : declared_bodies_map_(names_of_defined_functions),
      parent_body_(parent),
      is_operator_(is_operator),
      name_(std::move(name)) {
    garbage_container_ = new GarbageContainer;
    // lines_.reserve(30);
}

Body::~Body() { delete garbage_container_; }

// varStack push/pop
void Body::push(ExValue_ifs* line) {
    garbage_container_->add(line);
    var_stack_.push_back(line);
}

ExValue_ifs* Body::pop() {
    if (var_stack_.empty()) print_error("stack is empty");
    return var_stack_.pop();
}

stack<ExValue_ifs*> Body::pop(size_t length) { return var_stack_.pop(length); }


void Body::addLine(ExLine* line) {  //?delete
    garbage_container_->add(line);
    lines_.push_back(line);
}

void Body::addReturn(const std::string& name, ExValue_ifs* var) {  //?remove Value param
    auto line = new ExLine(name, var);

    if (is_tail_callable_) {
        var = var->getAssignedVal(true);

        bool valid_recursion = var->getNodeType() == NodeTypeEn::kTailCallTernary;
        if (!valid_recursion) print_error("it isn't tail recursion");
    }

    garbage_container_->add(line);
    return_stack_.push_back(line);
}



void Body::setFunctionBody(Body* body) {  // NOLINT
    if (!declared_bodies_map_.setFunctionBody(body)) {
        if (parent_body_) parent_body_->setFunctionBody(body);
    }
}

std::map<std::string, std::string> Body::getParameterLinkNames(bool hide_unused) const {
    std::map<std::string, std::string> ret;
    for (auto& value : lines_)
        if (value->isArg())
            if (!(hide_unused && value->isUnused())) ret[value->getName(true)] = value->getLinkName();
    return ret;
}

ExLine* Body::getLastLineFromName(const std::string& name) const {
    if (lines_.empty()) return nullptr;
    for (auto l = lines_.rbegin(); l != lines_.rend(); l++) {
        if ((*l)->checkName(name)) return *l;
    }

    print_error("unknown symbol " + name);
    return nullptr;
}

std::list<ParameterIfs*> Body::getOutputParameterList() const {
    std::list<ParameterIfs*> p_list;
    for (auto i : return_stack_) {
        p_list.push_back(i->getAssignedVal(true)->getParameter());
    }
    return p_list;
}

Signature Body::getSignature() const {
    Signature ret;
    for (auto& arg : lines_)
        if (arg->isArg()) ret.push_back(arg->type_);
    return ret;
}

Body* Body::getFunctionBody(const std::string& name, const Signature& signature) const {  // NOLINT
    Body* body = declared_bodies_map_.getFunctionBody(name, signature);
    if (body) return body;

    if (parent_body_) return parent_body_->getFunctionBody(name, signature);
    return nullptr;
}



// tree walker methods
std::string Body::print(const string& tab, bool DSTEna, bool hide_unused_lines) {
    PrintBodyContext context(tab, DSTEna, hide_unused_lines);
    stack<ExValue_ifs*> visitor_stack;

    context.setName(getName());

    for (auto& value : lines_) {
        if (value->isArg()) {
            context.createArg(value);
        } else {
            visitor_stack.push(value->getAssignedVal());
            do {
                auto var = visitor_stack.pop();
                if (var->isVisited()) var->printVisitExit(&context);
                else var->visitEnter(&visitor_stack);
            } while (!visitor_stack.empty());

            context.createLine(value);
        }
    }

    for (auto& value : return_stack_) {
        visitor_stack.push(value->getAssignedVal());
        do {
            auto var = visitor_stack.pop();
            if (var->isVisited()) var->printVisitExit(&context);
            else var->visitEnter(&visitor_stack);
        } while (!visitor_stack.empty());
        context.createReturn(value);
    }

    return context.getResult();
}



void Body::reverseTraversal() {
    stack<ExValue_ifs*> visitor_stack;
    for (auto& value : return_stack_) {
        visitor_stack.push(value->getAssignedVal());
        do {
            auto var = visitor_stack.pop();
            var->reverseTraversalVisitEnter(&visitor_stack);
        } while (!visitor_stack.empty());
    }
}


void Body::genTable(TableGenContext* table_gen_context) {
    stack<ExValue_ifs*> visitor_stack;

    if (name_ == "main")
        for (auto& value : return_stack_) value->getAssignedVal(true)->setReturned();

    for (auto& value : lines_) {
        if (!value->isUnused()) {
            if (value->isArg()) {
                table_gen_context->setUint(value);
            } else {
                visitor_stack.push(value->getAssignedVal());
                do {
                    auto var = visitor_stack.pop();
                    if (var->isVisited()) var->genBlocksVisitExit(table_gen_context);
                    else var->visitEnter(&visitor_stack);
                } while (!visitor_stack.empty());
            }
        }
    }
    int64_t max_buffer_length = 0;
    for (auto& value : return_stack_) {
        visitor_stack.push(value->getAssignedVal());
        do {
            auto var = visitor_stack.pop();
            if (var->isVisited()) var->genBlocksVisitExit(table_gen_context);
            else var->visitEnter(&visitor_stack);
        } while (!visitor_stack.empty());


        auto temp = value->getLength();
        if (max_buffer_length < temp) max_buffer_length = temp;
    }

    if (name_ == "main") table_gen_context->setMaxBufferLength(max_buffer_length);
}


Body* DeclaredBodiesMap::getFunctionBody(const std::string& name, const Signature& signature) const {
    auto a = find(name);
    if (a == end()) return nullptr;

    for (auto i : a->second) {
        if (signature == i->getSignature()) return i;
    }
    return nullptr;
}

bool DeclaredBodiesMap::setFunctionBody(Body* body) {
    auto a = find(body->getName());
    if (a == end()) return false;
    a->second.push_back(body);
    return true;
}


llvm::Function* Body::getOrGenIRPureFunction(IRGenerator& builder) {
    if (function_) return function_;

    llvm::LLVMContext& context = builder.getContext();
    IRGenerator local_builder(context, nullptr, true);

    std::vector<llvm::Type*> params;
    const std::vector<TypeEn> signature = getSignature().getList();
    for (auto i : signature) params.push_back(local_builder.getLLVMType(i));

    function_ = llvm::Function::Create(
        llvm::FunctionType::get(local_builder.getLLVMType(return_stack_.front()->type_), params, false),
        llvm::Function::ExternalLinkage, getName(), builder.getCurrentModule());

    local_builder.setCurrentFunction(function_);

    local_builder.setInitInsertPoint(llvm::BasicBlock::Create(context, "init_block", function_));
    local_builder.setCalcInsertPoint(llvm::BasicBlock::Create(context, "loop_block", function_));
    local_builder.setExitInsertPoint(llvm::BasicBlock::Create(context, "exit_block", function_));
    local_builder.setCalcInsertPoint();

    stack<ExValue_ifs*> visitor_stack;
    RecursiveGenContext gen_context(is_tail_callable_, false);

    for (auto& line : lines_) {
        if (line->isArg()) {
            gen_context.addArg(line);
        } else {
            visitor_stack.push(line->getAssignedVal());
            do {
                auto var = visitor_stack.pop();
                if (var->isVisited()) var->genRecursiveVisitExit(&gen_context);
                else var->visitEnter(&visitor_stack);
            } while (!visitor_stack.empty());
            gen_context.setUint(line);
        }
    }

    for (auto& value : return_stack_) {
        visitor_stack.push(value->getAssignedVal());
        do {
            auto var = visitor_stack.pop();
            if (var->isVisited()) var->genRecursiveVisitExit(&gen_context);
            else var->visitEnter(&visitor_stack);
        } while (!visitor_stack.empty());
        gen_context.setUint(value);
    }

    size_t size = gen_context.instructions_list_.size();
    for (size_t index = 0; index < size; index++) gen_context.instructions_list_[index]->setupIR(local_builder);

    if (!local_builder.getCurrentBlock()->back().isTerminator()) {
        local_builder.setCalcInsertPoint();
        local_builder.CreateBr(builder.getExitBlock());
        local_builder.setExitInsertPoint();
        local_builder.CreateRet(return_stack_.front()->getAssignedVal(true)->getIRValue(builder, 0));
    }

    local_builder.setInitInsertPoint();
    local_builder.CreateBr(local_builder.getCalcBlock());

    return function_;
}