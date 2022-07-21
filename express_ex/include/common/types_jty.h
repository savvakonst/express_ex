#ifndef TYPES_JTS_H
#define TYPES_JTS_H


#include <string>
#include <vector>


typedef int64_t untyped_t;


typedef int32_t (*jit_simple)();
typedef int (*jit_f)(char*, char*);
typedef int (*jit_fptr)(char, char*);
typedef int32_t (*Jit_Call_t)(char***, char* const*);

enum class TypeEn
{
    int1_jty = 0,
    int8_jty,
    int16_jty,
    int32_jty,
    int64_jty,
    float_jty = 16,
    double_jty,

    unknown_jty = 32
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
    kCall,
    kTailCall,
    kTailCallTernary,
    kRecursiveNeighborPoint
};



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
        ENUM2STR(int1_jty);
        ENUM2STR(int8_jty);
        ENUM2STR(int16_jty);
        ENUM2STR(int32_jty);
        ENUM2STR(int64_jty);
        ENUM2STR(float_jty);
        ENUM2STR(double_jty);
        ENUM2STR(unknown_jty);
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

inline bool isUnknownTy(TypeEn type) { return type == TypeEn::unknown_jty; }
inline bool isFloating(TypeEn type) { return (type >= TypeEn::float_jty) && !isUnknownTy(type); }
inline bool isInteger(TypeEn type) { return type <= TypeEn::int64_jty; }
inline bool isUInteger(TypeEn type) { return false; }
inline bool isBool(TypeEn type) { return type == TypeEn::int1_jty; }

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