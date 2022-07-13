#ifndef OPERATION_H
#define OPERATION_H

#include <vector>

#include "ExValue.h"
#include "common/types_jty.h"


// Value* newInvOperation(GarbageContainer* garbage_container, Value* arg);
ExValue* newBuiltInFuncOperation(GarbageContainer* garbage_container, TypeEn target_type, ExValue* arg,
                                 OpCodeEn op_type);
ExValue* newIntegrateOperation(GarbageContainer* garbage_container, ExValue* value);
ExValue* newArithmeticOperation(GarbageContainer* garbage_container, TypeEn target_type, ExValue* arg_a, ExValue* arg_b,
                                OpCodeEn op_type);
ExValue* newComparisonOperation(GarbageContainer* garbage_container, TypeEn target_type, ExValue* arg_a, ExValue* arg_b,
                                OpCodeEn op_type);
ExValue* newConvolveOperation(GarbageContainer* garbage_container, TypeEn target_type, ExValue* arg1, ExValue* arg_b,
                              int64_t shift = 0, OpCodeEn op_type = OpCodeEn::convolve);
ExValue* newTypeConvOp(GarbageContainer* garbage_container, TypeEn target_type, ExValue* arg);
ExValue* newSelectOp(GarbageContainer* garbage_container, TypeEn target_type, ExValue* arg_a, ExValue* arg_b,
                     ExValue* arg_c, bool rec_call = false);
// Value* newSliceOp(GarbageContainer* garbage_container, Value* arg_a, Value* arg_b, OpCodeEn op_type);
// Value* newSliceOp(GarbageContainer* garbage_container, Value* arg_a, int64_t int_val, OpCodeEn type_op);
ExValue* newSmallArrayDefOp(GarbageContainer* garbage_container, stack<ExValue*>& args,
                            OpCodeEn op_type = OpCodeEn::smallArrayDef, bool is_template = false);

#endif