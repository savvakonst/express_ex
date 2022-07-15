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

    for (const auto& i : lib_str_map) init(i.first, i.second);

    init(str, is_file_name);
}

KEXParser::~KEXParser() { delete listener_; }

BodyTemplate* KEXParser::getCurrentBody() { return listener_->current_body_; }

void KEXParser::init(const std::string& str, bool is_file_name) {
    std::string content;
    if (is_file_name) {
        std::ifstream ifs(str);
        content = std::string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    } else content = str;

    auto* input = new ANTLRInputStream(content + "\n");
    auto* lexer = new EGrammarLexer(input);
    auto* tokens = new CommonTokenStream(lexer);
    auto* parser = new EGrammarParser(tokens);

    parser->removeErrorListeners();
    auto* error_listener = new EErrorListener;
    parser->addErrorListener(error_listener);

    tree::ParseTree* tree = parser->start();
    tree::ParseTreeWalker::DEFAULT.walk(listener_, tree);

    delete error_listener;
    delete parser;
    delete tokens;
    delete lexer;
    delete input;
}
