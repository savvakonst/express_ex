#ifndef PARSER_H
#define PARSER_H


#include "EGrammarBaseListener.h"
#include "EGrammarLexer.h"
#include "EGrammarParser.h"
#include "antlr4-runtime.h"
//
#include <QtWidgets>
#include <sstream>
#include <utility>

#include "highlightStyle.h"
#include "highlighter.h"

typedef struct {
    int start;
    int count;
    QTextCharFormat* format;
} HighlightUint;



class Namespace {
   public:
    explicit Namespace(std::string name, Namespace* parent = nullptr) {
        name_ = std::move(name);
        parent_ = parent;
    }
    ~Namespace() = default;

    void addId(const std::string& id) { variable_list_.push_back(id); }

    bool checkIdExistens(const std::string& id) {
        for (std::string& i : variable_list_)
            if (i == id) {
                return true;
            }



        return false;
    }
    std::string getName() { return name_; }

   private:
    std::string name_;
    std::vector<std::string> variable_list_;
    Namespace* parent_ = nullptr;
};


class EErrorListener : public antlr4::BaseErrorListener {
   public:
    EErrorListener(HighlightStyle* style) { highlight_style_ = style; }

   private:
    void syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line,
                     size_t charPositionInLine, const std::string& msg, std::exception_ptr e) override {
        // qDebug()<<"line " << line << "; pos " << charPositionInLine << ":" << "_" << ". \n";
        // errorMessage = "line " << line << "; pos " << charPositionInLine << ":" << "_" << ". ";

        int start = (int)charPositionInLine;
        int length = (int)(offendingSymbol->getText().size());
        auto format = &highlight_style_->unknownIdFormat;

        format_map_[line].push_back({start, length, format});

        out_ << "line " << line << "; pos " << charPositionInLine << ":" << length << ". ";
        out_ << "\n\terror: ";
        out_ << msg.c_str() << ".\n";
        // out_ << "\t" << offendingSymbol->getText().c_str() << "\n";
        error_ = true;
    }

   public:
    QString getErrorMessage() { return QString::fromStdString(out_.str()); }
    bool hasError() const { return error_; }

   private:
    bool error_ = false;
    std::stringstream out_;
    HighlightStyle* highlight_style_;

   public:
    std::map<int, std::vector<HighlightUint>> format_map_;
    // QString errorMessage;
};



class TreeShapeListener : public EGrammarBaseListener {
   public:
    TreeShapeListener(HighlightStyle* style, const std::vector<Namespace*>& lib_namespace) : EGrammarBaseListener() {
        highlight_style_ = style;

        if (lib_namespace.empty()) namespase_list_.push_back(new Namespace("main"));
        else
            for (auto i : lib_namespace) namespase_list_.push_back(new Namespace(*i));

        activ_namespace_ = namespase_list_[0];
    }

    ~TreeShapeListener() override {
        for (auto& i : namespase_list_) {
            delete i;
        }
    };

    void addFormatUint(antlr4::Token* ctx, QTextCharFormat* format) {
        int line = (int)ctx->getLine();
        int start = (int)ctx->getCharPositionInLine();
        int length = (int)ctx->getStopIndex() - (int)ctx->getStartIndex() + 1;
        // qDebug()<<line<<", "<<start<<", "<<length<<"\n";
        if (ctx->getLine() == 0) return;
        format_map_[line].push_back({start, length, format});
    }

    void addFormatUint(antlr4::ParserRuleContext* ctx, int length, QTextCharFormat* format) {
        int line = (int)ctx->getStart()->getLine();
        int start = (int)ctx->getStart()->getCharPositionInLine();
        format_map_[line].push_back({start, length, format});
    }

    bool checkFunkExistens(const std::string& id) {
        for (auto& i : namespase_list_)
            if (i->getName() == id) return true;
        return false;
    }



    void exitAssign(EGrammarParser::AssignContext* ctx) override {
        std::string txt = ctx->ID()->getText();
        activ_namespace_->addId(txt);
        addFormatUint(ctx->ID()->getSymbol(), &highlight_style_->idFormat);
    }

    void exitAssignParam(EGrammarParser::AssignParamContext* ctx) override {
        if ((ctx->getStart() == nullptr) || (ctx->SPEC_SYM == nullptr)) return;
        for (auto& i : ctx->ID()) {
            if (i != nullptr) {
                std::string txt = i->getText();
                if (activ_namespace_->checkIdExistens(txt))
                    addFormatUint(i->getSymbol(), &highlight_style_->unknownIdFormat);
                else addFormatUint(i->getSymbol(), &highlight_style_->idFormat);
                activ_namespace_->addId(txt);
            }
        }
        addFormatUint(ctx->SPEC_SYM, &highlight_style_->specSymbolFormat);
        for (auto& i : ctx->STRINGLITERAL()) {
            if (i != nullptr) addFormatUint(i->getSymbol(), &highlight_style_->stringLiteralFormat);
        }
    }

    void exitAssignRetParam(EGrammarParser::AssignRetParamContext* ctx) override {
        if (ctx->SPEC_SYM != nullptr) addFormatUint(ctx->SPEC_SYM, &highlight_style_->specSymbolFormat);
    }

    void exitNeighborhoodPoint(EGrammarParser::NeighborhoodPointContext* ctx) override {
        if (activ_namespace_->checkIdExistens(ctx->ID()->getText()))
            addFormatUint(ctx->ID()->getSymbol(), &highlight_style_->idFormat);
        else addFormatUint(ctx->ID()->getSymbol(), &highlight_style_->unknownIdFormat);
    }

    void enterId(EGrammarParser::IdContext* ctx) override {
        if (activ_namespace_->checkIdExistens(ctx->ID()->getText()))
            addFormatUint(ctx->ID()->getSymbol(), &highlight_style_->idFormat);
        else addFormatUint(ctx->ID()->getSymbol(), &highlight_style_->unknownIdFormat);
    }

    void exitConst(EGrammarParser::ConstContext* ctx) override {
        addFormatUint(ctx->getStart(), &highlight_style_->constFormat);
    }

    void exitCallTConvBInFunc(EGrammarParser::CallTConvBInFuncContext* ctx) override {
        addFormatUint(ctx->tConvBInFunc()->getStart(), &highlight_style_->builtInFuncFormat);
    }

    void exitCallUnaryBInFunc(EGrammarParser::CallUnaryBInFuncContext* ctx) override {
        addFormatUint(ctx->unaryBInFunc()->getStart(), &highlight_style_->builtInFuncFormat);
    }

    void exitCallConvolve(EGrammarParser::CallConvolveContext* ctx) override {
        addFormatUint(ctx->SPEC_SYM, &highlight_style_->builtInFuncFormat);
    }

    void exitRange(EGrammarParser::RangeContext* ctx) override {
        addFormatUint(ctx->SPEC_SYM, &highlight_style_->builtInFuncFormat);
    }

    void exitShift(EGrammarParser::ShiftContext* ctx) override {
        addFormatUint(ctx->SPEC_SYM, &highlight_style_->builtInFuncFormat);
    }

    void exitDecimation(EGrammarParser::DecimationContext* ctx) override {
        addFormatUint(ctx->SPEC_SYM, &highlight_style_->builtInFuncFormat);
    }

    void exitCallFunc(EGrammarParser::CallFuncContext* ctx) override {
        if (checkFunkExistens(ctx->ID()->getText()))
            addFormatUint(ctx->ID()->getSymbol(), &highlight_style_->funcFormat);
        else addFormatUint(ctx->ID()->getSymbol(), &highlight_style_->unknownFuncFormat);
    }

    void enterFunc(EGrammarParser::FuncContext* ctx) override {
        if ((ctx->ID() == nullptr) || (ctx->getStart() == nullptr) || (ctx->args() == nullptr)) {
            return;
        }

        activ_namespace_ = new Namespace(ctx->ID()->getText());

        namespase_list_.push_back(activ_namespace_);

        addFormatUint(ctx->getStart(), &highlight_style_->specSymbolFormat);
        addFormatUint(ctx->ID()->getSymbol(), &highlight_style_->funcFormat);

        for (auto& i : ctx->args()->ID()) {
            std::string txt = i->getText();
            if (activ_namespace_->checkIdExistens(txt))
                addFormatUint(i->getSymbol(), &highlight_style_->unknownIdFormat);
            else addFormatUint(i->getSymbol(), &highlight_style_->idFormat);
            activ_namespace_->addId(txt);
        }

        if (ctx->SPEC_SYM) addFormatUint(ctx->SPEC_SYM, &highlight_style_->specSymbolFormat);

        activ_namespace_->addId(ctx->ID()->getText());
    }

    void exitFunc(EGrammarParser::FuncContext* ctx) override {
        if ((ctx->ID() == nullptr) || (ctx->getStart() == nullptr) || (ctx->args() == nullptr)) return;
        activ_namespace_ = namespase_list_[0];
    }


    std::vector<Namespace*> getNamespaceList() {
        std::vector<Namespace*> namespase_list;
        for (auto i : namespase_list_) namespase_list.push_back(new Namespace(*i));
        return namespase_list;
    }

   private:
    // namespase
    Namespace* activ_namespace_;
    HighlightStyle* highlight_style_;
    std::vector<Namespace*> namespase_list_;

   public:
    std::map<int, std::vector<HighlightUint>> format_map_;
};


#endif
