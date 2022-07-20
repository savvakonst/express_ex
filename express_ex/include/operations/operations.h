#ifndef OPERATION_H
#define OPERATION_H

#include <vector>

//#include "ExValue.h"
#include "common/types_jty.h"
class GarbageContainer;
class BodyTemplate;
class ExValue_ifs;

// Value* newInvOperation(GarbageContainer* garbage_container, Value* arg);
ExValue_ifs* newBuiltInFuncOperation(GarbageContainer* garbage_container, TypeEn target_type, ExValue_ifs* arg,
                                     OpCodeEn op_type);

ExValue_ifs* newIntegrateOperation(GarbageContainer* garbage_container, ExValue_ifs* value);

ExValue_ifs* newArithmeticOperation(GarbageContainer* garbage_container, TypeEn target_type, ExValue_ifs* arg_a,
                                    ExValue_ifs* arg_b, OpCodeEn op_type);

ExValue_ifs* newComparisonOperation(GarbageContainer* garbage_container, TypeEn target_type, ExValue_ifs* arg_a,
                                    ExValue_ifs* arg_b, OpCodeEn op_type);

ExValue_ifs* newConvolveOperation(GarbageContainer* garbage_container, TypeEn target_type, ExValue_ifs* arg1,
                                  ExValue_ifs* arg_b, int64_t shift = 0, OpCodeEn op_type = OpCodeEn::convolve);

ExValue_ifs* newTypeConvOp(GarbageContainer* garbage_container, TypeEn target_type, ExValue_ifs* arg);

ExValue_ifs* newTernaryOperation(GarbageContainer* garbage_container, TypeEn target_type, ExValue_ifs* arg_a,
                                 ExValue_ifs* arg_b, ExValue_ifs* arg_c, bool rec_call = false);

// Value* newSliceOp(GarbageContainer* garbage_container, Value* arg_a, Value* arg_b, OpCodeEn op_type);
// Value* newSliceOp(GarbageContainer* garbage_container, Value* arg_a, int64_t int_val, OpCodeEn type_op);

// ExValue* newSmallArrayDefOp(GarbageContainer* garbage_container, stack<ExValue*>& args,
//                             OpCodeEn op_type = OpCodeEn::smallArrayDef, bool is_template = false);



ExValue_ifs* newTypeConvOp(BodyTemplate* body_template, TypeEn target_type);
ExValue_ifs* newBuiltInFuncOperation(BodyTemplate* body_template, OpCodeEn op_type);
ExValue_ifs* newIntegrateOperation(BodyTemplate* body_template);
ExValue_ifs* newInversionOperation(BodyTemplate* body_template);
ExValue_ifs* newArithmeticOperation(BodyTemplate* body_template, OpCodeEn u_type_op);
ExValue_ifs* newComparisonOperation(BodyTemplate* body_template, OpCodeEn op_type);
ExValue_ifs* newConvolveOperation(BodyTemplate* body_template, OpCodeEn u_type_op, uint32_t shift);
ExValue_ifs* newTernaryOperation(BodyTemplate* body_template);
ExValue_ifs* newRangeOp(BodyTemplate* body_template, size_t arg_count);
ExValue_ifs* newSmallArrayDefOp(BodyTemplate* body_template, size_t arg_count);


#endif