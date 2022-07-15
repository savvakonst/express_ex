#ifndef OPERATION_H
#define OPERATION_H

#include <vector>

//#include "ExValue.h"
#include "common/types_jty.h"
class GarbageContainer;
class BodyTemplate;
class ExValue;

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

// ExValue* newSmallArrayDefOp(GarbageContainer* garbage_container, stack<ExValue*>& args,
//                             OpCodeEn op_type = OpCodeEn::smallArrayDef, bool is_template = false);



ExValue* newTypeConvOp(BodyTemplate* body_template, TypeEn target_type);
ExValue* newBuiltInFuncOperation(BodyTemplate* body_template, OpCodeEn op_type);
ExValue* newIntegrateOperation(BodyTemplate* body_template);
ExValue* newInversionOperation(BodyTemplate* body_template);
ExValue* newArithmeticOperation(BodyTemplate* body_template, OpCodeEn u_type_op);
ExValue* newComparisonOperation(BodyTemplate* body_template, OpCodeEn op_type);
ExValue* newConvolveOperation(BodyTemplate* body_template, OpCodeEn u_type_op, uint32_t shift);
ExValue* newSelectOp(BodyTemplate* body_template);
ExValue* newRangeOp(BodyTemplate* body_template, size_t arg_count);
ExValue* newSmallArrayDefOp(BodyTemplate* body_template, size_t arg_count);


#endif