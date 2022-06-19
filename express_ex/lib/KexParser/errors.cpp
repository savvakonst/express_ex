#include <iostream>

#include "treeShapeListener.h"



#include "parser/defWarningIgnore.h"
#include "common/undefWarningIgnore.h"

#include "common/common.h"
#include "ifs/ExStreamIfs.h"




extern PosInText g_pos;



ExStreamIfs * g_error_stream;




void print_error(const std::string &content) {

    ExStreamIfs& s = *g_error_stream;
    s << "line " << uint8_t (1);
    s << "line " << g_pos.start_line << "; pos " << g_pos.start_char_pos << ":" << g_pos.stop_char_pos << ". ";
    s << "error: " << content << ".\n";
    s << g_pos.txt << "\n";
    s.finalize();


    throw  content.length();
}

void print_IR_error(const std::string &content) {

    ExStreamIfs& s = *g_error_stream;
    s << ExColors::RED << "IR generation error: \n    " << ExColors::RESET << content <<"\n";
    s.finalize();

    throw  content.length();
}

void print_SA_error(const std::string &content) {
    ExStreamIfs& s = *g_error_stream;
    s << ExColors::RED << "samallarray calc error: \n    " << ExColors::RESET << content <<"\n";
    s.finalize();

    throw  content.length();
}