#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <iostream>
#include <list>
#include <strstream>

#include "defWarningIgnore.h"
#pragma warning(push)
#pragma warning(disable : 4251)
#include "EGrammarBaseListener.h"
#include "EGrammarLexer.h"
#include "EGrammarParser.h"
#include "antlr4-runtime.h"
#pragma warning(pop)
#include "bodyTemplate.h"
#include "line.h"
#include "undefWarningIgnore.h"

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

class KEXParser {
   public:
    KEXParser(BodyTemplate* body, const std::string& str, bool is_file_name = true) {
        listener_ = TreeShapeListener(body);
        init(str, is_file_name);
    }

    KEXParser(const std::string& str, bool is_file_name = true,
              const std::map<std::string, bool /*is_file_name*/>& lib_str_map = {}) {
        auto body = listener_.activ_body_;

        for (auto i : lib_str_map) init(i.first, i.second);

        init(str, is_file_name);
    }

    ~KEXParser() {}

    TreeShapeListener* getListener() { return &listener_; }

    BodyTemplate* getActivBody() { return listener_.activ_body_; }

   private:
    /*
    std::vector<BodyTemplate*> & getContext() {
        print_error("KEXParser::getContext()");
        return {nullptr};
        //return listener_.context_;
    }
    */

    void init(std::string str, bool is_file_name = true) {
        std::string content;
        if (is_file_name) {
            std::ifstream ifs(str);
            content = std::string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
        } else
            content = str;

        ANTLRInputStream* input_   = new ANTLRInputStream(content + "\n");
        EGrammarLexer* lexer_      = new EGrammarLexer(input_);
        CommonTokenStream* tokens_ = new CommonTokenStream(lexer_);
        EGrammarParser* parser_    = new EGrammarParser(tokens_);

        parser_->removeErrorListeners();
        EErrorListener* error_listner_ = new EErrorListener;
        parser_->addErrorListener(error_listner_);

        tree::ParseTree* tree_ = parser_->start();
        tree::ParseTreeWalker::DEFAULT.walk(&listener_, tree_);

        delete error_listner_;
        delete parser_;
        delete tokens_;
        delete lexer_;
        delete input_;
    }

    TreeShapeListener listener_;
};

#endif