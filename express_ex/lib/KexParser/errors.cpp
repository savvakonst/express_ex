#include <iostream>

#include "treeShapeListener.h"
#include "ifs/printer.h"

#include "parser/defWarningIgnore.h"
#include "common/undefWarningIgnore.h"
#include "common/common.h"



std::string g_error_str="";
extern PosInText g_pos;
extern std::string pos_file;


class StandardErrorIo: public ErrorIoIfs{
public:
    virtual ~StandardErrorIo() = default;
    virtual void errorIo(const std::string & msg_a, const std::string & msg_b,const std::string & msg_c) {
        auto errLine  = g_pos.start_line;
        auto startPos = g_pos.start_char_pos;
        auto stopPos  = g_pos.stop_char_pos;

        std::stringstream s;
        s << "line " << errLine << "; pos " << startPos << ":" << stopPos << ". ";
        s << "error: " << msg_a << ".\n";
        s << g_err_context->getText() << "\n";
        g_error_str = s.str();

    }
};


class ConsoleStandardErrorIo: public ErrorIoIfs{
public:
    virtual ~ConsoleStandardErrorIo() = default;
    virtual void errorIo(const std::string & msg_a, const std::string & msg_b,const std::string & msg_c) {
        auto errLine  = g_pos.start_line;
        auto startPos = g_pos.start_char_pos;
        auto stopPos  = g_pos.stop_char_pos;

        llvm::outs() << ExColors::RED << "line " << errLine << "; pos " << startPos << ":" << stopPos << ". ";

        llvm::outs() << "error: " << ExColors::RESET;
        llvm::outs() << msg_a << ".\n" ;

        llvm::outs() << ExColors::GREEN << "    " << g_err_context->getText() << "\n" <<ExColors::RESET;
    }
};





class SyntaxErrorIo: public ErrorIoIfs{
public:
    virtual ~SyntaxErrorIo() = default;
    virtual void errorIo(const std::string & msg_a, const std::string & msg_b,const std::string & msg_c) {
        std::stringstream s;
        s << "line " << msg_b << ":" << "_" << ". ";
        s << "error: ";
        s << msg_b << ".\n";
        s  << "\t" << msg_c << "\n" ;
        g_error_str = s.str();
    }
};


class ConsoleSyntaxErrorIo: public ErrorIoIfs{
public:
    virtual ~ConsoleSyntaxErrorIo() = default;
    virtual void errorIo(const std::string & msg_a, const std::string & msg_b,const std::string & msg_c) {

        llvm::outs() << ExColors::RED << "line " << msg_a<< ":" << "_" << ". ";
        llvm::outs() << "error: "<< ExColors::RESET;
        llvm::outs() << msg_b << ".\n" ;
        llvm::outs() << ExColors::GREEN << "\t" << msg_c << "\n" << ExColors::RESET;
    }
};


class CommonPartSyntaxErrorIo: public ErrorIoIfs{
public:
    explicit CommonPartSyntaxErrorIo(const std::string& common_part):common_part_(common_part){
    }

    virtual ~CommonPartSyntaxErrorIo() = default;
    virtual void errorIo(const std::string & msg_a, const std::string & msg_b,const std::string & msg_c) {
        std::stringstream s;
        s << common_part_ << msg_a << "\n";
        g_error_str = s.str();
    }

    const std::string common_part_;
};

class ConsoleCommonPartSyntaxErrorIo: public ErrorIoIfs{
public:
    explicit ConsoleCommonPartSyntaxErrorIo(const std::string& common_part):common_part_(common_part){
    }

    virtual ~ConsoleCommonPartSyntaxErrorIo() = default;
    virtual void errorIo(const std::string & msg_a, const std::string & msg_b,const std::string & msg_c) {
        std::stringstream s;
        s << common_part_ << msg_a << "\n";
        g_error_str = s.str();
    }

    const std::string common_part_;
};




extern ErrorIoIfs *g_standard_error_io;
extern ErrorIoIfs *g_syntax_error_io;


void print_error(const std::string &content) {

    if (g_standard_error_io)
        g_standard_error_io->errorIo(content);

    throw  content.length();
}

void print_IR_error(const std::string &content) {
#ifdef CONSOLE_APP_
    llvm::outs() << ExColors::RED << "IR generation error: \n    " << ExColors::RESET << content <<"\n";
#else
    std::stringstream s; 
    s << "IR generation error: \n    "  << content << "\n";
    g_error_str = s.str(); 
#endif
    throw  content.length();
}

void print_SA_error(const std::string &content) {
#ifdef CONSOLE_APP_
    llvm::outs() << ExColors::RED << "samallarray calc error: \n    " << ExColors::RESET << content << "\n";
#else
    std::stringstream s;
    s << "samallarray calc error: \n    " << content << "\n";
    g_error_str = s.str();
#endif
    throw  content.length();
}

void EErrorListener::syntaxError(Recognizer* recognizer, Token* offendingSymbol, size_t line, size_t charPositionInLine, const std::string & msg, std::exception_ptr e){
    std::stringstream s;
    s << "line " << line << "; pos " << charPositionInLine << ":" << "_" << ". ";

    if(g_syntax_error_io)
        g_syntax_error_io->errorIo(s.str(),msg,offendingSymbol->getText());

    throw  size_t(1);
}

