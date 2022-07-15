//
// Created by SVK on 17.06.2022.
//

#include "parser/KexParser.h"

#include "treeShapeListener.h"


KEXParser::KEXParser(BodyTemplate* body, const std::string& str, bool is_file_name) {
    listener_ = new TreeShapeListener(body);
    init(str, is_file_name);
}

KEXParser::KEXParser(const std::string& str, bool is_file_name,
                     const std::list<std::pair<std::string, bool /*is_file_name*/>>& lib_str_map) {
    listener_ = new TreeShapeListener();
    // auto body = listener_->activ_body_;

    for (auto i : lib_str_map) init(i.first, i.second);

    init(str, is_file_name);
}

KEXParser::~KEXParser() { delete listener_; }

BodyTemplate* KEXParser::getActivBody() { return listener_->current_body_; }

void KEXParser::init(std::string str, bool is_file_name) {
    std::string content;
    if (is_file_name) {
        std::ifstream ifs(str);
        content = std::string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    } else content = str;

    ANTLRInputStream* input = new ANTLRInputStream(content + "\n");
    EGrammarLexer* lexer = new EGrammarLexer(input);
    CommonTokenStream* tokens = new CommonTokenStream(lexer);
    EGrammarParser* parser = new EGrammarParser(tokens);

    parser->removeErrorListeners();
    EErrorListener* error_listner_ = new EErrorListener;
    parser->addErrorListener(error_listner_);

    tree::ParseTree* tree = parser->start();
    tree::ParseTreeWalker::DEFAULT.walk(listener_, tree);

    delete error_listner_;
    delete parser;
    delete tokens;
    delete lexer;
    delete input;
}
