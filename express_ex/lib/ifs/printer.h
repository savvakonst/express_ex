#ifndef PRINTIFS_H
#define PRINTIFS_H

#include <map>
#include "ifs/parameterIfs.h"
#include "ifs/ExStreamIfs.h"

#include "parser/defWarningIgnore.h"
#include "llvm/Support/raw_ostream.h"

#include "common/undefWarningIgnore.h"


extern bool g_ansi_escape_codes;



/*
template<typename T>
inline llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, const std::vector<T> & arg) {
    for (auto i : arg)
        OS << i << " ";
    return OS;
}


 * TODO remove
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

*/



llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, const ExColors & arg);

llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, const Delimiter & arg);

llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, const PrmTypesEn & arg);



#endif //PRINTIFS_H