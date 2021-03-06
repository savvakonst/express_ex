#ifndef TYPES_JIT_H
#define TYPES_JIT_H


#include <string>
#include <vector>


typedef int64_t untyped_t;


typedef int32_t (*jit_simple)();
typedef int (*jit_f)(char*, char*);
typedef int (*jit_fptr)(char, char*);
typedef int32_t (*Jit_Call_t)(char***, char* const*);


enum class TypeEn
{
    i1 = 0,
    i8,
    i16,
    i32,
    i64,
    f32 = 16,
    f64,

    unknown = 32
};


enum class DataStructureTypeEn
{
    kConstant,
    kVariable,
    kSmallArr,
    kLargeArr,
};

enum class NodeTypeEn
{
    kValue,
    kOperation,
    kLine,
    kArgument,
    kCall,
    kTailCall,
    kTailCallTernary,
    kRecursiveNeighborPoint,
    kRecurrenceRelation
};



// TODO this design isn't useful, only several operations really require enumeration
enum class OpCodeEn
{
#define EX_DEFINE_OPERATION(OP_NAME, OP_STR_REPR) OP_NAME,
#include "operations/OpCode.def"
#undef EX_DEFINE_OPERATION
};

const char* const kOpCodesStr[] = {
#define EX_DEFINE_OPERATION(OP_NAME, OP_STR_REPR) (" " #OP_STR_REPR " "),
#include "operations/OpCode.def"
#undef EX_DEFINE_OPERATION
};



inline std::string toString(TypeEn type) {
    std::string t = "pass";
#define ENUM2STR(x) \
case (TypeEn::x):   \
    t = #x;         \
    break
    switch (type) {
        ENUM2STR(i1);
        ENUM2STR(i8);
        ENUM2STR(i16);
        ENUM2STR(i32);
        ENUM2STR(i64);
        ENUM2STR(f32);
        ENUM2STR(f64);
        ENUM2STR(unknown);
    }
    return t;
#undef ENUM2STR
}

inline std::string toString(DataStructureTypeEn type) {
    std::string t = "pass";

#define ENUM2STR(x)            \
case (DataStructureTypeEn::x): \
    t = #x;                    \
    break

    switch (type) {
        ENUM2STR(kConstant);
        ENUM2STR(kVariable);
        ENUM2STR(kSmallArr);
        ENUM2STR(kLargeArr);
    }
    return t;
#undef ENUM2STR
}

inline bool isUnknownTy(TypeEn type) { return type == TypeEn::unknown; }
inline bool isFloating(TypeEn type) { return (type >= TypeEn::f32) && !isUnknownTy(type); }
inline bool isInteger(TypeEn type) { return type <= TypeEn::i64; }
inline bool isUInteger(TypeEn type) { return false; }
inline bool isBool(TypeEn type) { return type == TypeEn::i1; }

inline DataStructureTypeEn maxDS(const DataStructureTypeEn& var_a, const DataStructureTypeEn& var_b) {
    return var_a < var_b ? var_b : var_a;
}



class Signature : protected std::vector<TypeEn> {
   public:
    using std::vector<TypeEn>::push_back;

    friend bool operator==(const Signature& a, const Signature& b);
    const std::vector<TypeEn>& getList() const {
        std::vector<TypeEn>& ret = *((std::vector<TypeEn>*)(this));
        return ret;
    }
};

inline bool operator==(const Signature& a, const Signature& b) {
    if (a.size() != b.size()) return false;

    for (size_t i = 0, size_c = a.size(); i < size_c; i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

#endif  // !1