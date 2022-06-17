#include <math.h>
#include <stdint.h>
#include <vadefs.h>

#include <cstring>

namespace llvm {
namespace Intrinsic {
// Intrinsic ID type. This is an opaque typedef to facilitate splitting up
// the enum into target-specific enums.
typedef unsigned ID;

enum IndependentIntrinsics : unsigned
{
    not_intrinsic = 0,  // Must be zero

    // Get the intrinsic enums generated from Intrinsics.td
#define GET_INTRINSIC_ENUM_VALUES
#include "llvm/IR/IntrinsicEnums.inc"
#undef GET_INTRINSIC_ENUM_VALUES
};
};  // namespace Intrinsic

};  // namespace llvm

uint64_t getWin32Intrinsinc(char* ret_name, llvm::Intrinsic::ID code, bool is_double) {
#define CONFIG(ARG)             \
    {                           \
        strcpy(ret_name, #ARG); \
        return uintptr_t(ARG);  \
    }

    if (is_double) {
        switch (code) {
        case llvm::Intrinsic::pow:
            CONFIG(pow);

        case llvm::Intrinsic::sin:
            CONFIG(sin);
        case llvm::Intrinsic::cos:
            CONFIG(cos);

        case llvm::Intrinsic::exp:
            CONFIG(exp);

        case llvm::Intrinsic::log:
            CONFIG(log);

        case llvm::Intrinsic::log2:
            CONFIG(log2);

        case llvm::Intrinsic::log10:
            CONFIG(log10);

        case llvm::Intrinsic::abs:
            CONFIG(fabs);
        default:
            return (uint64_t) nullptr;
        }
    } else {
        switch (code) {
        case llvm::Intrinsic::pow:
            CONFIG(powf);

        case llvm::Intrinsic::sin:
            CONFIG(sinf);
        case llvm::Intrinsic::cos:
            CONFIG(cosf);

        case llvm::Intrinsic::exp:
            CONFIG(expf);

        case llvm::Intrinsic::log:
            CONFIG(logf);

        case llvm::Intrinsic::log2:
            CONFIG(log2f);

        case llvm::Intrinsic::log10:
            CONFIG(log10f);

        case llvm::Intrinsic::abs:
            CONFIG(fabsf);
        default:
            return (uint64_t) nullptr;
        }
    }
    return (uint64_t) nullptr;
}

/* pastae this code into table.cpp
uint64_t getWin32Intrinsinc(char* ret_name, llvm::Intrinsic::ID code, bool is_double);

#include <math.h>
llvm::Function* getWin32Intrinsinc(llvm::Module* M, llvm::Intrinsic::ID code, llvm::Type* Ty) {
    bool is_double = Ty->isDoubleTy();
    char name[10];
    uint64_t intptr = getWin32Intrinsinc(name, code, is_double);

    llvm::FunctionType* FT = code == llvm::Intrinsic::pow ? llvm::FunctionType::get(Ty, {Ty, Ty}, false)
                                                          : llvm::FunctionType::get(Ty, Ty, false);
    llvm::Function* f = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, name, M);

    return f;
}
*/