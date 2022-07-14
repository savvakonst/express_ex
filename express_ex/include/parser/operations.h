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



// TODO: replace with ExValue* newTypeConvOp(BodyTemplate* body_template, TypeEn target_type);
// TODO: replace with ExValue* newBuiltInFuncOperation(BodyTemplate* body_template, OpCodeEn op_type);
// TODO: replace with ExValue* newIntegrateOperation(BodyTemplate* body_template);
// TODO: replace with ExValue* newInversionOperation(BodyTemplate* body_template);
// TODO: replace with ExValue* newArithmeticOperation(BodyTemplate* body_template, OpCodeEn u_type_op);
// TODO: replace with ExValue* newComparisonOperation(BodyTemplate* body_template, OpCodeEn op_type);
// TODO: replace with ExValue* newConvolveOperation(BodyTemplate* body_template, OpCodeEn u_type_op, uint32_t shift);
// TODO: replace with ExValue* newSelectOp(BodyTemplate* body_template);
// TODO: replace with ExValue* newRangeOp(BodyTemplate* body_template, size_t arg_count);
// TODO: replace with ExValue* newSmallArrayDefOp(BodyTemplate* body_template, size_t arg_count);


#endif