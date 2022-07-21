#include "treeShapeListener.h"

#include "ifs/ExStreamIfs.h"
#include "operations/ExConstValue.h"
#include "operations/operations.h"

PosInText g_pos;

// TODO: need to remove this
extern ExStreamIfs* g_error_stream;

void EErrorListener::syntaxError(Recognizer* recognizer, Token* offendingSymbol, size_t line, size_t charPositionInLine,
                                 const std::string& msg, std::exception_ptr e) {
    ExStreamIfs& s = *g_error_stream;

    s << ExColors::RED << "line " << line << "; pos " << charPositionInLine << ":"
      << "_"
      << ". ";
    s << "error: " << ExColors::RESET;
    s << msg << ".\n";
    s << ExColors::GREEN << "\t" << offendingSymbol->getText() << "\n" << ExColors::RESET;


    throw size_t(1);
}


TreeShapeListener::TreeShapeListener() : EGrammarBaseListener() {
    current_body_ = new BodyTemplate("main", current_body_);
}

TreeShapeListener::TreeShapeListener(BodyTemplate* body, const std::vector<BodyTemplate*>& context)
    : EGrammarBaseListener() {
    current_body_ = body;
}

TreeShapeListener::~TreeShapeListener() = default;


void TreeShapeListener::exitAssignParam(EGrammarParser::AssignParamContext* ctx) {
    setPos(ctx);
    const auto id = ctx->ID();
    const auto stl = ctx->STRINGLITERAL();

    if (stl.empty())
        for (const auto i : id) current_body_->addParam(i->getText(), "", DataStructureTypeEn::kLargeArr);

    else if (id.size() == stl.size())
        for (size_t i = 0; i < id.size(); i++) {
            // activ_body_->addParam(id[i]->getText(), TypeEn::double_jty, DataStructureTypeEn::kLargeArr,
            // stoi(stl[i]->getText().substr(1)));
            std::string s = stl[i]->getText();
            current_body_->addParam(id[i]->getText(), s.substr(1, s.length() - 2), DataStructureTypeEn::kLargeArr);
        }
    else print_error("there are invalid signature ");
}


void TreeShapeListener::enterFunc(EGrammarParser::FuncContext* ctx) {
    setPos(ctx);

    auto body = new BodyTemplate(ctx->ID()->getText(), current_body_);
    if (current_body_) {
        current_body_->child_body_template_list_.push_back(body);
    }
    current_body_ = body;

    for (auto i : ctx->args()->ID()) {
        current_body_->addArg(i->getText());
    }
}

void TreeShapeListener::exitFunc(EGrammarParser::FuncContext* ctx) {
    setPos(ctx);
    if (current_body_->isRetStackEmpty())
        print_error("there are no returned value in func: " + current_body_->getName());
    current_body_ = current_body_->getParent();
}

void TreeShapeListener::exitAssign(EGrammarParser::AssignContext* ctx) {
    setPos(ctx);
    current_body_->addLine(ctx->ID()->getText(), current_body_->pop());
}

void TreeShapeListener::exitAssignRetParam(EGrammarParser::AssignRetParamContext* ctx) {
    setPos(ctx);
    for (auto k : ctx->expr())
        if (current_body_->isRetStackFull()) print_error("there are more then one returned value");
        else current_body_->addReturn("return", current_body_->pop());
}

void TreeShapeListener::exitNeighborhoodPoint(EGrammarParser::NeighborhoodPointContext* ctx) {
    setPos(ctx);
    if (current_body_->isTopBody())
        print_error("it is impossible to take neighbor point in top scope, only inside functions");

    const std::string id = ctx->ID()->getText();

    if (current_body_->getName() == id) {
        // TODO add recursive neighbor point operation
    } else {
        current_body_->getLastLineFromName(ctx->ID()->getText());
        //  TODO: neighbor point operation
    }
}

void TreeShapeListener::exitConst(EGrammarParser::ConstContext* ctx) {
    setPos(ctx);
    auto target_type = TypeEn::unknown_jty;
#define CONV_TY(depend, target) \
    case (depend):              \
        target_type = (target); \
        break
    switch (ctx->start->getType()) {
        CONV_TY(EGrammarParser::FLOAT, TypeEn::float_jty);
        CONV_TY(EGrammarParser::DOUBLE, TypeEn::double_jty);
        CONV_TY(EGrammarParser::INT, TypeEn::int32_jty);
        CONV_TY(EGrammarParser::INT64, TypeEn::int64_jty);
    default:;
    }
    current_body_->push(new ExConstValue(ctx->getText(), target_type));
#undef CONV_TY
}

void TreeShapeListener::exitShift(EGrammarParser::ShiftContext* ctx) {
    setPos(ctx);
    print_error("addShiftOp");
    // activ_body_->addShiftOp();
}

void TreeShapeListener::exitCondExpr(EGrammarParser::CondExprContext* ctx) {
    setPos(ctx);
    current_body_->push(newTernaryOperation(current_body_));
}

void TreeShapeListener::exitRange(EGrammarParser::RangeContext* ctx) {
    setPos(ctx);
    current_body_->push(newRangeOp(current_body_, ctx->expr().size()));
}

void TreeShapeListener::exitSmallArrayDefinition(EGrammarParser::SmallArrayDefinitionContext* ctx) {
    setPos(ctx);
    current_body_->push(newSmallArrayDefOp(current_body_, ctx->expr().size()));
}

void TreeShapeListener::exitMoreeqLesseq(EGrammarParser::MoreeqLesseqContext* ctx) {
    setPos(ctx);
    OpCodeEn op = (EGrammarParser::MOREEQ == ctx->op->getType()) ? OpCodeEn::sge : OpCodeEn::sle;
    current_body_->push(newComparisonOperation(current_body_, op));
}

void TreeShapeListener::exitMulDiv(EGrammarParser::MulDivContext* ctx) {
    setPos(ctx);
    OpCodeEn op = (EGrammarParser::MUL == ctx->op->getType()) ? OpCodeEn::mul : OpCodeEn::sdiv;
    current_body_->push(newArithmeticOperation(current_body_, op));
}

void TreeShapeListener::exitDecimation(EGrammarParser::DecimationContext* ctx) {
    setPos(ctx);
    print_error("addDecimationOp");
    // activ_body_->addDecimationOp();
}

void TreeShapeListener::exitCallTConvBInFunc(EGrammarParser::CallTConvBInFuncContext* ctx) {
    setPos(ctx);
    auto target_type = TypeEn::unknown_jty;
#define CONV_TY(depend, target) \
    case (depend):              \
        target_type = (target); \
        break
    switch (ctx->tConvBInFunc()->start->getType()) {
        CONV_TY(EGrammarParser::C_FLOAT, TypeEn::float_jty);
        CONV_TY(EGrammarParser::C_DOUBLE, TypeEn::double_jty);
        CONV_TY(EGrammarParser::C_INT, TypeEn::int32_jty);
        CONV_TY(EGrammarParser::C_INT64, TypeEn::int64_jty);
    default:;
    }
    current_body_->push(newTypeConvOp(current_body_, target_type));
#undef CONV_TY
}

void TreeShapeListener::exitPow(EGrammarParser::PowContext* ctx) {
    setPos(ctx);
    current_body_->push(newArithmeticOperation(current_body_, OpCodeEn::pow));
}

void TreeShapeListener::enterId(EGrammarParser::IdContext* ctx) {
    setPos(ctx);
    current_body_->push(current_body_->getLastLineFromName(ctx->ID()->getText()));
}

void TreeShapeListener::exitEquality(EGrammarParser::EqualityContext* ctx) {
    setPos(ctx);
    OpCodeEn op = (EGrammarParser::EQ == ctx->op->getType()) ? OpCodeEn::eq : OpCodeEn::ne;
    current_body_->push(newComparisonOperation(current_body_, op));
}

void TreeShapeListener::exitCallConvolve(EGrammarParser::CallConvolveContext* ctx) {
    setPos(ctx);
    current_body_->push(newConvolveOperation(current_body_, OpCodeEn::convolve, 0));
}

void TreeShapeListener::exitMoreLess(EGrammarParser::MoreLessContext* ctx) {
    setPos(ctx);
    OpCodeEn op = (EGrammarParser::MORE_ == ctx->op->getType()) ? OpCodeEn::sgt : OpCodeEn::slt;
    current_body_->push(newComparisonOperation(current_body_, op));
}

void TreeShapeListener::exitCallUnaryBInFunc(EGrammarParser::CallUnaryBInFuncContext* ctx) {
    setPos(ctx);
    auto op = OpCodeEn::none_op;
#define CONV_TY(depend, target) \
    case (depend):              \
        op = (target);          \
        break
    switch (ctx->unaryBInFunc()->start->getType()) {
        CONV_TY(EGrammarParser::LOG, OpCodeEn::log);
        CONV_TY(EGrammarParser::LOG10, OpCodeEn::log10);
        CONV_TY(EGrammarParser::COS, OpCodeEn::cos);
        CONV_TY(EGrammarParser::SIN, OpCodeEn::sin);
        CONV_TY(EGrammarParser::EXP, OpCodeEn::exp);
    default:;
    }
#undef CONV_TY
    current_body_->push(newBuiltInFuncOperation(current_body_, op));
}

void TreeShapeListener::exitAddSub(EGrammarParser::AddSubContext* ctx) {
    setPos(ctx);
    OpCodeEn op = (EGrammarParser::ADD == ctx->op->getType()) ? OpCodeEn::add : OpCodeEn::sub;
    current_body_->push(newArithmeticOperation(current_body_, op));
}

void TreeShapeListener::exitCallFunc(EGrammarParser::CallFuncContext* ctx) {
    setPos(ctx);
    auto expr = ctx->expr();

    const std::string function_name = ctx->ID()->getText();
    BodyTemplate* called_body = current_body_->getFunctionBody(function_name);


    if (called_body) {
        if (called_body->getArgCount() != int(expr.size()))
            print_error("there are invalid signature call in function: " + function_name + " ");

        if (current_body_->getName() == function_name) {
            current_body_->addTailCall();
        } else {
            current_body_->addCall(called_body);
        }

    } else print_error("there are no function with same name");
}

void TreeShapeListener::exitInv(EGrammarParser::InvContext* ctx) {
    setPos(ctx);
    current_body_->push(newInversionOperation(current_body_));
}

void TreeShapeListener::exitCallIntegrate(EGrammarParser::CallIntegrateContext* ctx) {
    setPos(ctx);
    current_body_->push(newIntegrateOperation(current_body_));
}


void TreeShapeListener::setPos(ParserRuleContext* ctx) {
    g_pos.start_line = (int64_t)ctx->getStart()->getLine();
    g_pos.stop_line = (int64_t)ctx->getStop()->getLine();
    g_pos.start_char_pos = (int64_t)ctx->getStart()->getCharPositionInLine();
    g_pos.stop_char_pos = (int64_t)ctx->getStop()->getCharPositionInLine();
    g_pos.txt = ctx->getText();
}
