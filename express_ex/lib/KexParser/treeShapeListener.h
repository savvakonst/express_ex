#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <iostream>
#include <list>
#include <strstream>

#include "parser/defWarningIgnore.h"
#pragma warning(push)
#pragma warning(disable : 4251)
#include "EGrammarBaseListener.h"
#include "EGrammarLexer.h"
#include "EGrammarParser.h"
#include "antlr4-runtime.h"
#pragma warning(pop)
#include "parser/bodyTemplate.h"
#include "parser/line.h"
#include "common/undefWarningIgnore.h"

using namespace antlr4;
// using namespace  parser_cpp;

extern ParserRuleContext* g_err_context;

class EErrorListener : public BaseErrorListener {
    void syntaxError(Recognizer* recognizer, Token* offendingSymbol, size_t line, size_t charPositionInLine,
                     const std::string& msg, std::exception_ptr e) override;
};

class TreeShapeListener : public EGrammarBaseListener {
   public:
    TreeShapeListener();
    TreeShapeListener(BodyTemplate* body, const std::vector<BodyTemplate*>& context = {});
    ~TreeShapeListener() override;

    void exitAssign(EGrammarParser::AssignContext* ctx) override;
    void exitAssignParam(EGrammarParser::AssignParamContext* ctx) override;
    void exitAssignRetParam(EGrammarParser::AssignRetParamContext* ctx) override;
    void enterId(EGrammarParser::IdContext* ctx) override;
    void exitConst(EGrammarParser::ConstContext* ctx) override;
    void exitCallTConvBInFunc(EGrammarParser::CallTConvBInFuncContext* ctx) override;
    void exitCallUnaryBInFunc(EGrammarParser::CallUnaryBInFuncContext* ctx) override;
    void exitCallIntegrate(EGrammarParser::CallIntegrateContext* ctx) override;
    // arithmetic operations
    void exitInv(EGrammarParser::InvContext* ctx) override;
    void exitMulDiv(EGrammarParser::MulDivContext* ctx) override;
    void exitAddSub(EGrammarParser::AddSubContext* ctx) override;
    void exitPow(EGrammarParser::PowContext* ctx) override;
    void exitMoreLess(EGrammarParser::MoreLessContext* ctx) override;
    void exitMoreeqLesseq(EGrammarParser::MoreeqLesseqContext* ctx) override;
    void exitEquality(EGrammarParser::EqualityContext* ctx) override;
    // conditional
    void exitCondExpr(EGrammarParser::CondExprContext* ctx) override;
    void exitCallConvolve(EGrammarParser::CallConvolveContext* ctx) override;
    void exitRange(EGrammarParser::RangeContext* ctx) override;
    void exitShift(EGrammarParser::ShiftContext* ctx) override;
    void exitDecimation(EGrammarParser::DecimationContext* ctx) override;
    // call function
    void exitCallFunc(EGrammarParser::CallFuncContext* ctx) override;
    void enterFunc(EGrammarParser::FuncContext* ctx) override;
    void exitFunc(EGrammarParser::FuncContext* ctx) override;
    // array definition
    void exitSmallArrayDefinition(EGrammarParser::SmallArrayDefinitionContext* ctx) override;

    BodyTemplate* getMainBody();

    BodyTemplate* activ_body_ = nullptr;

   private:
    static void setPos(ParserRuleContext* ctx);
};



#endif