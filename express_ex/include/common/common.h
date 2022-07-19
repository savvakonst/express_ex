#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string>
#include <vector>

#include "ifs/parameterIfs.h"
#include "types_jty.h"



void print_error(const std::string& content);
void print_IR_error(const std::string& content);
void print_SA_error(const std::string& content);


class ExLine;
class ExValue_ifs;

typedef struct {
    int64_t start_line = -1;
    int64_t stop_line = -1;
    int64_t start_char_pos = -1;
    int64_t stop_char_pos = -1;
    std::string txt;
} PosInText;

template <typename T>
class stack : public std::vector<T> {
   public:
    T pop() {
        T res = std::vector<T>::back();
        std::vector<T>::pop_back();

        return res;
    }

    stack<T> pop(size_t length) {
        stack<T> op;
        for (size_t i = 0; i < length; i++) op.push(pop());
        std::reverse(op.begin(), op.end());
        return op;
    }

    void push(T var) { std::vector<T>::push_back(var); }
};

bool operator==(const ExValue_ifs* var_a, DataStructureTypeEn var_b);
bool operator==(DataStructureTypeEn var_a, const ExValue_ifs* var_b);
bool operator<(TypeEn var_a, const ExValue_ifs* var_b);
bool operator<(const ExValue_ifs* var_a, TypeEn var_b);
/*
bool isConst(const Value* var_a);
bool isSmallArr(const Value* var_a);
bool isLargeArr(const Value* var_a);
bool isSimilar(const Value* var_a, const Value* var_b);
bool isCompatible(const Value* var_a, const Value* var_b);

bool isUnknownTy(TypeEn type);
bool isFloating(TypeEn type);
bool isInteger(TypeEn type);
bool isUInteger(TypeEn type);
*/
bool isUnknownTy(const ExValue_ifs* var);
bool isFloating(const ExValue_ifs* var);
bool isInteger(const ExValue_ifs* var);
bool isUInteger(const ExValue_ifs* var);

template <typename Int_T_>
inline Int_T_ maxInt(Int_T_ var1, Int_T_ var2) {
    return (var1 < var2) ? var2 : var1;
}
template <typename Int_T_>
inline Int_T_ minInt(Int_T_ var1, Int_T_ var2) {
    return (var1 < var2) ? var2 : var2;
}

ExValue_ifs* maxTypeVar(ExValue_ifs* var_a, ExValue_ifs* var_b);
ExValue_ifs* maxDSVar(ExValue_ifs* var_a, ExValue_ifs* var_b);
ExValue_ifs* maxLevelVar(ExValue_ifs* var_a, ExValue_ifs* var_b);
ExValue_ifs* minLevelVar(ExValue_ifs* var_a, ExValue_ifs* var_b);

#define CASE_OP_GLOBAL(depend, target) \
case (depend):                         \
    target;                            \
    break

#define SWITCH_TYPE_OP(TYPE, DEFAULT)                   \
    switch (TYPE) {                                     \
        CASE_OP_GLOBAL(TypeEn::double_jty, OP(double)); \
        CASE_OP_GLOBAL(TypeEn::float_jty, OP(float));   \
        CASE_OP_GLOBAL(TypeEn::int64_jty, OP(int64_t)); \
        CASE_OP_GLOBAL(TypeEn::int32_jty, OP(int32_t)); \
        CASE_OP_GLOBAL(TypeEn::int16_jty, OP(int16_t)); \
        CASE_OP_GLOBAL(TypeEn::int8_jty, OP(int8_t));   \
        CASE_OP_GLOBAL(TypeEn::int1_jty, OP(bool));     \
    default:                                            \
        DEFAULT;                                        \
        break;                                          \
    }

#define SWITCH_UINT(CASE_ARG, LOOP, X) \
case CASE_ARG:                         \
    LOOP { X }
#define AR_SWITCH_OP(OP, LOOP, RET, ARG_A, ARG_B)                                     \
    switch (OP) {                                                                     \
        SWITCH_UINT(OpCodeEn::add, LOOP, RET = ARG_A + ARG_B;) break;                 \
        SWITCH_UINT(OpCodeEn::sub, LOOP, RET = ARG_A - ARG_B;) break;                 \
        SWITCH_UINT(OpCodeEn::mul, LOOP, RET = ARG_A * ARG_B;) break;                 \
        SWITCH_UINT(OpCodeEn::sdiv, LOOP, RET = ARG_A / ARG_B;) break;                \
        SWITCH_UINT(OpCodeEn::srem, LOOP, RET = (T)fmod(ARG_A, ARG_B);) break;        \
        SWITCH_UINT(OpCodeEn::pow, LOOP, RET = (T)pow((double)ARG_A, ARG_B);) break;  \
        SWITCH_UINT(OpCodeEn::fadd, LOOP, RET = ARG_A + ARG_B;) break;                \
        SWITCH_UINT(OpCodeEn::fsub, LOOP, RET = ARG_A - ARG_B;) break;                \
        SWITCH_UINT(OpCodeEn::fmul, LOOP, RET = ARG_A * ARG_B;) break;                \
        SWITCH_UINT(OpCodeEn::fdiv, LOOP, RET = ARG_A / ARG_B;) break;                \
        SWITCH_UINT(OpCodeEn::frem, LOOP, RET = (T)fmod(ARG_A, ARG_B);) break;        \
        SWITCH_UINT(OpCodeEn::fpow, LOOP, RET = (T)pow((double)ARG_A, ARG_B);) break; \
    default:                                                                          \
        break;                                                                        \
    }

/*
    oeq, // ordered and equal
    ogt, // ordered and greater than
    oge, // ordered and greater than or equal
    olt, // ordered and less than
    ole, // ordered and less than or equal
    one, // ordered and not equal
    ord, // ordered (no nans)
*/

#define CMP_SWITCH_OP(OP, LOOP, RET, ARG_A, ARG_B)                     \
    switch (OP) {                                                      \
        SWITCH_UINT(OpCodeEn::eq, LOOP, RET = ARG_A == ARG_B;) break;  \
        SWITCH_UINT(OpCodeEn::ne, LOOP, RET = ARG_A != ARG_B;) break;  \
        SWITCH_UINT(OpCodeEn::sgt, LOOP, RET = ARG_A > ARG_B;) break;  \
        SWITCH_UINT(OpCodeEn::sge, LOOP, RET = ARG_A >= ARG_B;) break; \
        SWITCH_UINT(OpCodeEn::slt, LOOP, RET = ARG_A < ARG_B;) break;  \
        SWITCH_UINT(OpCodeEn::sle, LOOP, RET = ARG_A <= ARG_B;) break; \
                                                                       \
        SWITCH_UINT(OpCodeEn::oeq, LOOP, RET = ARG_A == ARG_B;) break; \
        SWITCH_UINT(OpCodeEn::one, LOOP, RET = ARG_A != ARG_B;) break; \
        SWITCH_UINT(OpCodeEn::ogt, LOOP, RET = ARG_A > ARG_B;) break;  \
        SWITCH_UINT(OpCodeEn::oge, LOOP, RET = ARG_A >= ARG_B;) break; \
        SWITCH_UINT(OpCodeEn::olt, LOOP, RET = ARG_A < ARG_B;) break;  \
        SWITCH_UINT(OpCodeEn::ole, LOOP, RET = ARG_A <= ARG_B;) break; \
    default:                                                           \
        break;                                                         \
    }

#define BI_SWITCH_OP(OP, LOOP, RET, ARG_A)                                \
    switch (OP) {                                                         \
        SWITCH_UINT(OpCodeEn::log, LOOP, RET = (T)log(ARG_A);) break;     \
        SWITCH_UINT(OpCodeEn::log2, LOOP, RET = (T)log2(ARG_A);) break;   \
        SWITCH_UINT(OpCodeEn::log10, LOOP, RET = (T)log10(ARG_A);) break; \
        SWITCH_UINT(OpCodeEn::cos, LOOP, RET = (T)cos(ARG_A);) break;     \
        SWITCH_UINT(OpCodeEn::sin, LOOP, RET = (T)sin(ARG_A);) break;     \
        SWITCH_UINT(OpCodeEn::exp, LOOP, RET = (T)exp(ARG_A);) break;     \
    default:                                                              \
        break;                                                            \
    }

#define OP_LV1(T, ARG_TYPE)                                    \
    switch (ARG_TYPE) {                                        \
        CASE_OP_GLOBAL(TypeEn::double_jty, OP_LV2(T, double)); \
        CASE_OP_GLOBAL(TypeEn::float_jty, OP_LV2(T, float));   \
        CASE_OP_GLOBAL(TypeEn::int64_jty, OP_LV2(T, int64_t)); \
        CASE_OP_GLOBAL(TypeEn::int32_jty, OP_LV2(T, int32_t)); \
        CASE_OP_GLOBAL(TypeEn::int16_jty, OP_LV2(T, int16_t)); \
        CASE_OP_GLOBAL(TypeEn::int8_jty, OP_LV2(T, int8_t));   \
        CASE_OP_GLOBAL(TypeEn::int1_jty, OP_LV2(T, bool));     \
    }

#define CONV_TYPE_OP(ARG_TYPE, RET_TYPE)                                  \
    if (!isUnknownTy(ARG_TYPE)) {                                         \
        uint64_t V = 0;                                                   \
        switch (RET_TYPE) {                                               \
            CASE_OP_GLOBAL(TypeEn::double_jty, OP_LV1(double, ARG_TYPE)); \
            CASE_OP_GLOBAL(TypeEn::float_jty, OP_LV1(float, ARG_TYPE));   \
            CASE_OP_GLOBAL(TypeEn::int64_jty, OP_LV1(int64_t, ARG_TYPE)); \
            CASE_OP_GLOBAL(TypeEn::int32_jty, OP_LV1(int32_t, ARG_TYPE)); \
            CASE_OP_GLOBAL(TypeEn::int16_jty, OP_LV1(int16_t, ARG_TYPE)); \
            CASE_OP_GLOBAL(TypeEn::int8_jty, OP_LV1(int8_t, ARG_TYPE));   \
            CASE_OP_GLOBAL(TypeEn::int1_jty, OP_LV1(bool, ARG_TYPE));     \
        }                                                                 \
    } else {                                                              \
    }

/*
template< typename T >
uint64_t calcArithmeticOperation(T arg1, T arg2, opCodeEn uTypeOp)
{
    T value;
    uint64_t binaryValue = 0;
#define CONV_OP(depend,target) case (depend):  value=(target) ;  break
    switch (uTypeOp)
    {
        CONV_OP(opCodeEn::ADD, arg1 + arg2);
        CONV_OP(opCodeEn::SUB, arg1 - arg2);
        CONV_OP(opCodeEn::MUL, arg1 * arg2);
        CONV_OP(opCodeEn::SDIV, arg1 / arg2);
        CONV_OP(opCodeEn::SREM, (T)fmod(arg1, arg2));
        CONV_OP(opCodeEn::POW, (T)pow((double)arg1, arg2));
    }
    *((T*)(&binaryValue)) = value;
#undef CONV_OP
    return binaryValue;
}
*/

template <typename T>
T* SmallArrayAlloc(int n, T* ptr = nullptr) {
    if (ptr != nullptr) return ptr;
    return new T[n];
}

template <typename T>
uint64_t calcArithmeticOperation(T arg1, T arg2, OpCodeEn uTypeOp) {
    T value = (T)0;
    ;
    uint64_t binary_value = 0;
    AR_SWITCH_OP(uTypeOp, ;, value, arg1, arg2);
    *((T*)(&binary_value)) = value;
    return binary_value;
}

template <typename T>
uint64_t calcComparisonOperation(T arg1, T arg2, OpCodeEn uTypeOp) {
    bool value = (bool)0;
    ;
    uint64_t binaryValue = 0;
    CMP_SWITCH_OP(uTypeOp, ;, value, arg1, arg2);
    *((bool*)(&binaryValue)) = value;
    return binaryValue;
}

template <typename T>
uint64_t calcBuiltInFuncOperation(T arg, OpCodeEn uTypeOp) {
    T value = (T)0;
    uint64_t binaryValue = 0;
    BI_SWITCH_OP(uTypeOp, ;, value, arg);
    *((T*)(&binaryValue)) = value;
    return binaryValue;
}

template <typename T>
void arithmeticTemplate(OpCodeEn op, T* ret, T* a, T* b, int n) {
    AR_SWITCH_OP(op, for (int i = 0; i < n; i++), ret[i], a[i], b[i]);
}

template <typename T>
void arithmeticConstTemplate(OpCodeEn op, T* ret, T* a, T b, int n) {
    AR_SWITCH_OP(op, for (int i = 0; i < n; i++), ret[i], a[i], b);
}

template <typename T>
void arithmeticConstTemplate(OpCodeEn op, T* ret, T a, T* b, int n) {
    AR_SWITCH_OP(op, for (int i = 0; i < n; i++), ret[i], a, b[i]);
}

template <typename T>
void comparisonTemplate(OpCodeEn op, T* ret, T* a, T* b, int n) {
    CMP_SWITCH_OP(op, for (int i = 0; i < n; i++), ret[i], a[i], b[i]);
}

template <typename T>
void comparisonConstTemplate(OpCodeEn op, T* ret, T* a, T b, int n) {
    CMP_SWITCH_OP(op, for (int i = 0; i < n; i++), ret[i], a[i], b);
}

template <typename T>
void comparisonConstTemplate(OpCodeEn op, T* ret, T a, T* b, int n) {
    CMP_SWITCH_OP(op, for (int i = 0; i < n; i++), ret[i], a, b[i]);
}

template <typename T>
void invTemplate(T* ret, T* a, int n) {
    for (int i = 0; i < n; i++) {
        ret[i] = -a[i];
    }
}

template <typename T>
void builtInFuncTemplate(OpCodeEn op, T* ret, T* a, int n) {
    BI_SWITCH_OP(op, for (int i = 0; i < n; i++), ret[i], a[i]);
}

template <typename T>
void selectTemplate(OpCodeEn op, T* ret, bool* a, T* b, T* c, int n) {
    for (int i = 0; i < n; i++) ret[i] = a[i] ? b[i] : c[i];
}

template <typename T>
void selectTemplate(OpCodeEn op, T* ret, bool* a, T* b, T c, int n) {
    for (int i = 0; i < n; i++) ret[i] = a[i] ? b[i] : c;
}

template <typename T>
void selectTemplate(OpCodeEn op, T* ret, bool* a, T b, T* c, int n) {
    for (int i = 0; i < n; i++) ret[i] = a[i] ? b : c[i];
}

template <typename T>
void selectTemplate(OpCodeEn op, T* ret, bool* a, T b, T c, int n) {
    for (int i = 0; i < n; i++) ret[i] = a[i] ? b : c;
}

template <typename T>
void convolveTemplate_(T* ret, T* a, T* b, int na, int nb, int offset = 0) {
    int n_max = na, n_min = nb;
    T *main_arr = a, *sub_arr = b;
    if (nb > na) {
        n_max = nb;
        n_min = na;
        main_arr = b;
        sub_arr = a;
    }

    int left_offset = (n_min / 2) + offset - (n_min - 1);
    for (int i = 0; i < n_max; i++) {
        ret[i] = (T)0;
        /*

        for (int j=0; j < nMin; j++) {
            int k=i + j - left_offset;
            if ((k >= 0) && (k < nMax))
                ret[i]+= mainArr[k] * subArr[nMin-1-j];
        }
        */

        for (int j = n_min - 1; j > -1; j--) {
            int k = i - j - left_offset;
            if ((k >= 0) && (k < n_max)) ret[i] += main_arr[k] * sub_arr[j];
        }
    }
}


template <typename Ty>
Ty typeCastConstValue(TypeEn arg_type, untyped_t arg_int) {
    Ty return_val = 0;
#define OP_LV2(T1, T2)                \
    {                                 \
        T2* typedArg = (T2*)&arg_int; \
        return_val = (Ty)*typedArg;   \
    }
    OP_LV1(arg_type, arg_type);
#undef OP_LV2
    return return_val;
}

#undef SWITCH_UINT
#undef AR_SWITCH_OP
#undef BI_SWITCH_OP

char* calcSmallArrayAlloc(TypeEn target_type, int N, char* ptr = nullptr);

char* calcArithmeticSmallArray(OpCodeEn op, TypeEn target_type, char* ret, char* a, char* b, int n);
untyped_t calcArithmeticConst(OpCodeEn op, TypeEn target_type, untyped_t a, untyped_t b);
char* calcArithmeticSmallArray(OpCodeEn op, TypeEn target_type, char* ret, int64_t a, char* b, int n);
char* calcArithmeticSmallArray(OpCodeEn op, TypeEn target_type, char* ret, char* a, int64_t b, int n);

char* calcComparisonSmallArray(OpCodeEn op, TypeEn target_type, char* ret, char* a, char* b, int n);
untyped_t calcComparisonConst(OpCodeEn op, TypeEn target_type, untyped_t a, untyped_t b);
char* calcComparisonSmallArray(OpCodeEn op, TypeEn target_type, char* ret, int64_t a, char* b, int n);
char* calcComparisonSmallArray(OpCodeEn op, TypeEn target_type, char* ret, char* a, int64_t b, int n);

char* invArithmeticSmallArray(TypeEn target_type, char* ret, char* a, int n);
untyped_t invArithmeticConst(TypeEn target_type, untyped_t a);

char* calcBuiltInFuncSmallArray(OpCodeEn op, TypeEn target_type, char* ret, char* a, int n);
untyped_t calcBuiltInFuncConst(OpCodeEn op, TypeEn target_type, untyped_t a);

char* calcSelectSmallArray(OpCodeEn op, TypeEn target_type, char* ret, char* a, char* b, char* c, int n);
untyped_t calcSelectConst(OpCodeEn op, TypeEn target_type, untyped_t a, untyped_t b, untyped_t c);
char* calcSelectSmallArray(OpCodeEn op, TypeEn target_type, char* ret, char* a, char* b, int64_t c, int n);
char* calcSelectSmallArray(OpCodeEn op, TypeEn target_type, char* ret, char* a, int64_t b, char* c, int n);
char* calcSelectSmallArray(OpCodeEn op, TypeEn target_type, char* ret, char* a, int64_t b, int64_t c, int n);
char* calcConvolveSmallArray(TypeEn target_type, char* ret, char* a, char* b, int aN, int bN);

char* calcTypeConvSmallArray(TypeEn ret_type, TypeEn arg_type, char* ret, char* arg, int n);
untyped_t calcTypeConvConst(TypeEn ret_type, TypeEn arg_type, untyped_t arg_int);
char* calcSmallArrayDef(TypeEn targetType, const std::vector<ExValue_ifs*>& operand);

inline size_t tEnSizeof(TypeEn type) {
    size_t size = 0;
#define OP(T) size = sizeof(T)
    SWITCH_TYPE_OP(type, ;)
#undef OP
    return size;
}

inline size_t sizeOfTy(TypeEn type) {
    size_t size = 0;
#define OP(T) size = sizeof(T)
    SWITCH_TYPE_OP(type, ;)
#undef OP
    return size;
}

TypeEn PRMType2JITType(PrmTypesEn arg);
PrmTypesEn JITType2PRMType(TypeEn arg);

template <typename T>
void calcMinMax(char* carg, int64_t Number, double& dmax, double& dmin, bool init) {
    T* arg = (T*)carg;

    T max = (T)dmax;
    T min = (T)dmin;
    if (init & (Number != 0)) {
        max = arg[0];
        min = arg[0];
    }
    for (int i = 0; i < Number; i++) {
        T var = arg[i];
        max = max > var ? max : var;
        min = min < var ? min : var;
    }
    dmax = (double)max;
    dmin = (double)min;
}

#endif  // COMMON_H