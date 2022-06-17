//
// Created by SVK on 17.06.2022.
//

#include "parser/KexParser.h"
#include "treeShapeListener.h"


KEXParser::KEXParser(BodyTemplate *body, const std::string &str, bool is_file_name) {
    listener_ = new TreeShapeListener(body);
    init(str, is_file_name);
}

KEXParser::KEXParser(const std::string &str, bool is_file_name, const std::map<std::string, bool> &lib_str_map) {
    auto body = listener_->activ_body_;

    for (auto i : lib_str_map) init(i.first, i.second);

    init(str, is_file_name);
}

KEXParser::~KEXParser() {delete listener_;}

BodyTemplate *KEXParser::getActivBody() { return listener_->activ_body_; }

void KEXParser::init(std::string str, bool is_file_name) {
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
    tree::ParseTreeWalker::DEFAULT.walk(listener_, tree_);

    delete error_listner_;
    delete parser_;
    delete tokens_;
    delete lexer_;
    delete input_;
}
