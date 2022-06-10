
#include "parser/common.h"

#include "parser/variable.h"

char *calcSmallArrayAlloc(TypeEn target_type, int N, char *ptr) {
    char *ret = nullptr;
#define OP(T) ret = (char *)SmallArrayAlloc<T>(N, (T *)ptr)
    SWITCH_TYPE_OP(target_type, ;);
#undef OP
    return ret;
}

char *calcArithmeticSmallArray(OpCodeEn op, TypeEn target_type, char *ret, char *a, char *b, int n) {
    ret = calcSmallArrayAlloc(target_type, n, ret);
#define OP(T) arithmeticTemplate<T>(op, (T *)ret, (T *)a, (T *)b, n)
    SWITCH_TYPE_OP(target_type, ;);
#undef OP
    return ret;
}

untyped_t calcArithmeticConst(OpCodeEn op, TypeEn target_type, untyped_t a, untyped_t b) {
    untyped_t ret = 0;
#define OP(T) arithmeticTemplate<T>(op, (T *)&ret, (T *)&a, (T *)&b, 1)
    SWITCH_TYPE_OP(target_type, ;);
#undef OP
    return ret;
}

char *calcArithmeticSmallArray(OpCodeEn op, TypeEn target_type, char *ret, char *a, int64_t b, int n) {
    ret = calcSmallArrayAlloc(target_type, n, ret);
#define OP(T) arithmeticConstTemplate<T>(op, (T *)ret, (T *)a, *((T *)(&b)), n)
    SWITCH_TYPE_OP(target_type, ;);
#undef OP
    return ret;
}

char *calcArithmeticSmallArray(OpCodeEn op, TypeEn target_type, char *ret, int64_t a, char *b, int n) {
    ret = calcSmallArrayAlloc(target_type, n, ret);
#define OP(T) arithmeticConstTemplate<T>(op, (T *)ret, *((T *)(&a)), (T *)b, n)
    SWITCH_TYPE_OP(target_type, ;);
#undef OP
    return ret;
}

char *calcComparisonSmallArray(OpCodeEn op, TypeEn target_type, char *ret, char *a, char *b, int n) {
    ret = calcSmallArrayAlloc(target_type, n, ret);
#define OP(T) comparisonTemplate<T>(op, (T *)ret, (T *)a, (T *)b, n)
    SWITCH_TYPE_OP(target_type, ;);
#undef OP
    return ret;
}

untyped_t calcComparisonConst(OpCodeEn op, TypeEn target_type, untyped_t a, untyped_t b) {
    untyped_t ret = 0;
#define OP(T) comparisonTemplate<T>(op, (T *)&ret, (T *)&a, (T *)&b, 1)
    SWITCH_TYPE_OP(target_type, ;);
#undef OP
    return ret;
}

char *calcComparisonSmallArray(OpCodeEn op, TypeEn target_type, char *ret, char *a, int64_t b, int n) {
    ret = calcSmallArrayAlloc(target_type, n, ret);
#define OP(T) comparisonConstTemplate<T>(op, (T *)ret, (T *)a, *((T *)(&b)), n)
    SWITCH_TYPE_OP(target_type, ;);
#undef OP
    return ret;
}

char *calcComparisonSmallArray(OpCodeEn op, TypeEn target_type, char *ret, int64_t a, char *b, int n) {
    ret = calcSmallArrayAlloc(target_type, n, ret);
#define OP(T) comparisonConstTemplate<T>(op, (T *)ret, *((T *)(&a)), (T *)b, n)
    SWITCH_TYPE_OP(target_type, ;);
#undef OP
    return ret;
}

char *invArithmeticSmallArray(TypeEn target_type, char *ret, char *a, int n) {
    ret = calcSmallArrayAlloc(target_type, n, ret);
#define OP(T) invTemplate<T>((T *)ret, (T *)a, n)
    SWITCH_TYPE_OP(target_type, ;);
#undef OP
    return ret;
}

untyped_t invArithmeticConst(TypeEn target_type, untyped_t a) {
    untyped_t ret = 0;
#define OP(T) invTemplate<T>((T *)&ret, (T *)&a, 1)
    SWITCH_TYPE_OP(target_type, ;);
#undef OP
    return ret;
}

char *calcBuiltInFuncSmallArray(OpCodeEn op, TypeEn target_type, char *ret, char *a, int n) {
    ret = calcSmallArrayAlloc(target_type, n, ret);
#define OP(T) builtInFuncTemplate<T>(op, (T *)ret, (T *)a, n)
    SWITCH_TYPE_OP(target_type, ;);
#undef OP
    return ret;
}

untyped_t calcBuiltInFuncConst(OpCodeEn op, TypeEn target_type, untyped_t a) {
    untyped_t ret = 0;
#define OP(T) builtInFuncTemplate<T>(op, (T *)&ret, (T *)&a, 1)
    SWITCH_TYPE_OP(target_type, ;);
#undef OP
    return ret;
}

char *calcSelectSmallArray(OpCodeEn op, TypeEn target_type, char *ret, char *a, char *b, char *c, int n) {
    ret = calcSmallArrayAlloc(target_type, n, ret);
#define OP(T) selectTemplate<T>(op, (T *)ret, (bool *)a, (T *)b, (T *)c, n)
    SWITCH_TYPE_OP(target_type, ;);
#undef OP
    return ret;
}

untyped_t calcSelectConst(OpCodeEn op, TypeEn target_type, untyped_t a, untyped_t b, untyped_t c) {
    untyped_t ret = 0;
#define OP(T) selectTemplate<T>(op, (T *)&ret, (bool *)&a, (T *)&b, (T *)&c, 1)
    SWITCH_TYPE_OP(target_type, ;);
#undef OP
    return ret;
}

char *calcSelectSmallArray(OpCodeEn op, TypeEn target_type, char *ret, char *a, char *b, int64_t c, int n) {
    ret = calcSmallArrayAlloc(target_type, n, ret);
#define OP(T) selectTemplate<T>(op, (T *)ret, (bool *)a, (T *)b, *((T *)(&c)), n)
    SWITCH_TYPE_OP(target_type, ;);
#undef OP
    return ret;
}

char *calcSelectSmallArray(OpCodeEn op, TypeEn target_type, char *ret, char *a, int64_t b, char *c, int n) {
    ret = calcSmallArrayAlloc(target_type, n, ret);
#define OP(T) selectTemplate<T>(op, (T *)ret, (bool *)a, *((T *)(&b)), (T *)c, n)
    SWITCH_TYPE_OP(target_type, ;);
#undef OP
    return ret;
}

char *calcSelectSmallArray(OpCodeEn op, TypeEn target_type, char *ret, char *a, int64_t b, int64_t c, int n) {
    ret = calcSmallArrayAlloc(target_type, n, ret);
#define OP(T) selectTemplate<T>(op, (T *)ret, (bool *)a, *((T *)(&b)), *((T *)(&c)), n)
    SWITCH_TYPE_OP(target_type, ;);
#undef OP
    return ret;
}

char *calcConvolveSmallArray(TypeEn target_type, char *ret, char *a, char *b, int aN, int bN) {
    ret = calcSmallArrayAlloc(target_type, maxInt(aN, bN), ret);
#define OP(T) convolveTemplate_<T>((T *)ret, (T *)a, (T *)b, aN, bN)
    SWITCH_TYPE_OP(target_type, ;);
#undef OP
    return ret;
}

char *calcTypeConvSmallArray(TypeEn ret_type, TypeEn arg_type, char *ret, char *arg, int n) {
    ret = calcSmallArrayAlloc(ret_type, n, ret);

#define OP_LV2(T1, T2)                     \
    {                                      \
        T1 *typedRet = (T1 *)ret;          \
        T2 *typedArg = (T2 *)arg;          \
        for (int i = 0; i < n; i++) {      \
            typedRet[i] = (T1)typedArg[i]; \
        }                                  \
    }

    CONV_TYPE_OP(arg_type, ret_type);
#undef OP_LV2
    return ret;
}

untyped_t calcTypeConvConst(TypeEn ret_type, TypeEn arg_type, untyped_t arg_int) {
    untyped_t return_int = 0;
#define OP_LV2(T1, T2)                    \
    {                                     \
        T1 *typedRet = (T1 *)&return_int; \
        T2 *typedArg = (T2 *)&arg_int;    \
        typedRet[0] = (T1)typedArg[0];    \
    }
    CONV_TYPE_OP(arg_type, ret_type);
#undef OP_LV2
    return return_int;
}

char *calcSmallArrayDef(TypeEn targetType, const std::vector<Value *> &operand) {
    char *ret = calcSmallArrayAlloc(targetType, (int)operand.size());

#define OP(T)                                                      \
    {                                                              \
        T *ptr = (T *)ret;                                         \
        for (auto &j : operand) {                                  \
            int64_t v = j->getAssignedVal(true)->getBinaryValue(); \
            *ptr++ = *((T *)(&v));                                 \
        }                                                          \
    }

    SWITCH_TYPE_OP(targetType, ;);
#undef OP
    return ret;
}
