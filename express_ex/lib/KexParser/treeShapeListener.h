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
#include "common/undefWarningIgnore.h"
#include "parser/bodyTemplate.h"
#include "parser/line.h"

using namespace antlr4;
// using namespace  parser_cpp;



class EErrorListener : public BaseErrorListener {
    void syntaxError(Recognizer* recognizer, Token* offendingSymbol, size_t line, size_t charPositionInLine,
                     const std::string& msg, std::exception_ptr e) override;
};

class TreeShapeListener : public EGrammarBaseListener {
   public:
    TreeShapeListener();
    explicit TreeShapeListener(BodyTemplate* body, const std::vector<BodyTemplate*>& context = {});
    ~TreeShapeListener() override;

    // void enterAssignParam(EGrammarParser::AssignParamContext* ctx) override {}
    void exitAssignParam(EGrammarParser::AssignParamContext* ctx) override;

    // void enterOutParam(EGrammarParser::OutParamContext* ctx) override { }
    // void exitOutParam(EGrammarParser::OutParamContext* ctx) override { }

    void enterFunc(EGrammarParser::FuncContext* ctx) override;
    void exitFunc(EGrammarParser::FuncContext* ctx) override;

    // void enterAssign(EGrammarParser::AssignContext* ctx) override { }
    void exitAssign(EGrammarParser::AssignContext* ctx) override;

    // void enterAssignRetParam(EGrammarParser::AssignRetParamContext* ctx) override { }
    void exitAssignRetParam(EGrammarParser::AssignRetParamContext* ctx) override;

    // void enterArgs(EGrammarParser::ArgsContext* ctx) override { }
    // void exitArgs(EGrammarParser::ArgsContext* ctx) override { }

    // void enterUnaryBInFunc(EGrammarParser::UnaryBInFuncContext* ctx) override { }
    // void exitUnaryBInFunc(EGrammarParser::UnaryBInFuncContext* ctx) override { }

    // void enterTConvBInFunc(EGrammarParser::TConvBInFuncContext* ctx) override { }
    // void exitTConvBInFunc(EGrammarParser::TConvBInFuncContext* ctx) override { }

    // void enterLeft_expr(EGrammarParser::Left_exprContext* ctx) override { }
    // void exitLeft_expr(EGrammarParser::Left_exprContext* ctx) override { }

    // void enterRight_expr(EGrammarParser::Right_exprContext* ctx) override { }
    // void exitRight_expr(EGrammarParser::Right_exprContext* ctx) override { }

    // void enterParens(EGrammarParser::ParensContext* ctx) override { }
    // void exitParens(EGrammarParser::ParensContext* ctx) override { }

    // void enterNeighborhoodPoint(EGrammarParser::NeighborhoodPointContext* ctx) override { }
    void exitNeighborhoodPoint(EGrammarParser::NeighborhoodPointContext* ctx) override;

    // void enterConst(EGrammarParser::ConstContext* ctx) override { }
    void exitConst(EGrammarParser::ConstContext* ctx) override;

    // void enterShift(EGrammarParser::ShiftContext* ctx) override { }
    void exitShift(EGrammarParser::ShiftContext* ctx) override;

    // void enterBitwiseXor(EGrammarParser::BitwiseXorContext* ctx) override { }
    //  TODO: implement
    // void exitBitwiseXor(EGrammarParser::BitwiseXorContext* ctx) override { }

    // void enterCondExpr(EGrammarParser::CondExprContext* ctx) override { }
    void exitCondExpr(EGrammarParser::CondExprContext* ctx) override;

    // void enterRange(EGrammarParser::RangeContext* ctx) override { }
    void exitRange(EGrammarParser::RangeContext* ctx) override;

    // void enterCallRecConvolve(EGrammarParser::CallRecConvolveContext* ctx) override { }
    //  TODO: remove
    // void exitCallRecConvolve(EGrammarParser::CallRecConvolveContext* ctx) override { }

    // void enterSmallArrayDefinition(EGrammarParser::SmallArrayDefinitionContext* ctx) override { }
    void exitSmallArrayDefinition(EGrammarParser::SmallArrayDefinitionContext* ctx) override;

    // void enterMoreeqLesseq(EGrammarParser::MoreeqLesseqContext* ctx) override { }
    void exitMoreeqLesseq(EGrammarParser::MoreeqLesseqContext* ctx) override;

    // void enterBitwiseAnd(EGrammarParser::BitwiseAndContext* ctx) override { }
    //  TODO: implement
    // void exitBitwiseAnd(EGrammarParser::BitwiseAndContext* ctx) override { }

    // void enterMulDiv(EGrammarParser::MulDivContext* ctx) override { }
    void exitMulDiv(EGrammarParser::MulDivContext* ctx) override;

    // void enterDecimation(EGrammarParser::DecimationContext* ctx) override { }
    void exitDecimation(EGrammarParser::DecimationContext* ctx) override;

    // void enterCallTConvBInFunc(EGrammarParser::CallTConvBInFuncContext* ctx) override { }
    void exitCallTConvBInFunc(EGrammarParser::CallTConvBInFuncContext* ctx) override;

    // void enterBitwiseShift(EGrammarParser::BitwiseShiftContext* ctx) override { }
    //  TODO: implement
    // void exitBitwiseShift(EGrammarParser::BitwiseShiftContext* ctx) override { }

    // void enterPow(EGrammarParser::PowContext* ctx) override { }
    void exitPow(EGrammarParser::PowContext* ctx) override;


    // void enterBitwiseOr(EGrammarParser::BitwiseOrContext* ctx) override { }
    //  TODO: implement
    // void exitBitwiseOr(EGrammarParser::BitwiseOrContext* ctx) override { }

    void enterId(EGrammarParser::IdContext* ctx) override;
    // void exitId(EGrammarParser::IdContext* ctx) override { }

    // void enterEquality(EGrammarParser::EqualityContext* ctx) override { }
    void exitEquality(EGrammarParser::EqualityContext* ctx) override;

    // void enterCallConvolve(EGrammarParser::CallConvolveContext* ctx) override { }
    void exitCallConvolve(EGrammarParser::CallConvolveContext* ctx) override;

    // void enterMoreLess(EGrammarParser::MoreLessContext* ctx) override { }
    void exitMoreLess(EGrammarParser::MoreLessContext* ctx) override;

    // void enterCallUnaryBInFunc(EGrammarParser::CallUnaryBInFuncContext* ctx) override { }
    void exitCallUnaryBInFunc(EGrammarParser::CallUnaryBInFuncContext* ctx) override;

    // void enterAddSub(EGrammarParser::AddSubContext* ctx) override { }
    void exitAddSub(EGrammarParser::AddSubContext* ctx) override;

    // void enterCallFunc(EGrammarParser::CallFuncContext* ctx) override { }
    void exitCallFunc(EGrammarParser::CallFuncContext* ctx) override;

    // void enterInv(EGrammarParser::InvContext* ctx) override { }
    void exitInv(EGrammarParser::InvContext* ctx) override;

    // void enterCallIntegrate(EGrammarParser::CallIntegrateContext* ctx) override { }
    void exitCallIntegrate(EGrammarParser::CallIntegrateContext* ctx) override;

    /////////////////////////


    BodyTemplate* getMainBody();
    BodyTemplate* current_body_ = nullptr;

   private:
    static void setPos(ParserRuleContext* ctx);
};



#endif