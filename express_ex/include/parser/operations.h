#ifndef OPERATION_H
#define OPERATION_H

#include <vector>

#include "common/types_jty.h"
#include "variable.h"


// Value* newInvOperation(GarbageContainer* garbage_container, Value* arg);
Value* newBuiltInFuncOperation(GarbageContainer* garbage_container, TypeEn target_type, Value* arg, OpCodeEn op_type);
Value* newIntegrateOperation(GarbageContainer* garbage_container, Value* value);
Value* newArithmeticOperation(GarbageContainer* garbage_container, TypeEn target_type, Value* arg_a, Value* arg_b,
                              OpCodeEn op_type);
Value* newComparisonOperation(GarbageContainer* garbage_container, TypeEn target_type, Value* arg_a, Value* arg_b,
                              OpCodeEn op_type);
Value* newConvolveOperation(GarbageContainer* garbage_container, TypeEn target_type, Value* arg1, Value* arg_b,
                            int64_t shift = 0, OpCodeEn op_type = OpCodeEn::convolve);
Value* newTypeConvOp(GarbageContainer* garbage_container, TypeEn target_type, Value* arg);
Value* newSelectOp(GarbageContainer* garbage_container, TypeEn target_type, Value* arg_a, Value* arg_b, Value* arg_c,
                   bool rec_call = false);
// Value* newSliceOp(GarbageContainer* garbage_container, Value* arg_a, Value* arg_b, OpCodeEn op_type);
// Value* newSliceOp(GarbageContainer* garbage_container, Value* arg_a, int64_t int_val, OpCodeEn type_op);
Value* newSmallArrayDefOp(GarbageContainer* garbage_container, stack<Value*>& args,
                          OpCodeEn op_type = OpCodeEn::smallArrayDef, bool is_template = false);

#endif