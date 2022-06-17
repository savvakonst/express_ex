#ifndef PRINTIFS_H
#define PRINTIFS_H

#include <map>
#include "ifs/parameterIfs.h"


#include "parser/defWarningIgnore.h"
#include "llvm/Support/raw_ostream.h"

#include "parser/undefWarningIgnore.h"


extern bool g_ansi_escape_codes;


enum class Delimiter {
    BLACK = 0,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    SAVEDCOLOR,


    AnsiBLACK = 32,
    AnsiRED,
    AnsiGREEN,
    AnsiYELLOW,
    AnsiBLUE,
    AnsiMAGENTA,
    AnsiCYAN,
    AnsiWHITE
};

enum class ExColors {
    BLACK = 0,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    SAVEDCOLOR,
    RESET,

    AnsiBLACK = 32,
    AnsiRED,
    AnsiGREEN,
    AnsiYELLOW,
    AnsiBLUE,
    AnsiMAGENTA,
    AnsiCYAN,
    AnsiWHITE,
    AnsiSAVEDCOLOR,
    AnsiRESET
};

template<typename T>
inline llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, const std::vector<T> & arg) {
    for (auto i : arg)
        OS << i << " ";
    return OS;
}


inline llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, const std::vector<ParameterIfs*> & arg) {
    std::stringstream stream;
    for (auto i : arg)
        if (i!=nullptr)
            OS << i->stream(stream).str() << " ";
    return OS;
}

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, ParameterIfs & arg) {
    std::stringstream stream;
    OS << arg.stream(stream).str() << " ";
    return OS;
}


template<typename Key_T, typename _T>
inline llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, const std::map<Key_T, _T> & arg) {
    for (auto i : arg)
        OS << i.first << ": " << i.second << " ";
    return OS;
}


llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, const ExColors & arg);

llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, const Delimiter & arg);

llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, const PrmTypesEn & arg);



#endif //PRINTIFS_H