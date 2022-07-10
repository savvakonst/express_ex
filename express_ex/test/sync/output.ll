

----------------------LLVM module:--------------------
; ModuleID = 'test'
source_filename = "test"

declare i32 @buffer_update_function(i32)

define i32 @main(i64*** %0) {
init_block:
  %offset_incr = getelementptr inbounds i64**, i64*** %0, i32 0
  %1 = load i64**, i64*** %offset_incr, align 8
  %2 = call i32 @sub_main(i64** %1)
  ret i32 1
}

define i32 @sub_main(i64** %0) {
init_block:
  %glob_index_alloca = alloca i64, align 8
  store i64 0, i64* %glob_index_alloca, align 4
  %common_offset_alloca_ = alloca i64, align 8
  store i64 0, i64* %common_offset_alloca_, align 4
  %external_untyped_buffer_ptr_0 = getelementptr inbounds i64*, i64** %0, i32 0
  %external_untyped_buffer_0 = load volatile i64*, i64** %external_untyped_buffer_ptr_0, align 8
  %external_buffer_0 = bitcast i64* %external_untyped_buffer_0 to float*
  %internal_untyped_buffer_ptr_1 = getelementptr inbounds i64*, i64** %0, i32 1
  %internal_untyped_buffer_1 = load volatile i64*, i64** %internal_untyped_buffer_ptr_1, align 8
  %internal_buffer_1 = bitcast i64* %internal_untyped_buffer_1 to double*
  %internal_untyped_buffer_ptr_2 = getelementptr inbounds i64*, i64** %0, i32 2
  %internal_untyped_buffer_2 = load volatile i64*, i64** %internal_untyped_buffer_ptr_2, align 8
  %internal_buffer_2 = bitcast i64* %internal_untyped_buffer_2 to double*
  br label %intermediate_block_0x183d80_level_0_0

intermediate_block_0x183d80_level_0_0:            ; preds = %init_block
  store i64 -1, i64* %common_offset_alloca_, align 4
  br label %load_block_0x183d80_level_0_0

load_block_0x183d80_level_0_0:                    ; preds = %store_block_0x183d80_level_0_0, %intermediate_block_0x183d80_level_0_0
  %common_offset_ = load i64, i64* %common_offset_alloca_, align 4
  %offset_arg_incr_ = getelementptr inbounds float, float* %external_buffer_0, i64 %common_offset_
  %arg_buffer_val_ = load volatile float, float* %offset_arg_incr_, align 4
  br label %calc_block_0x183d80_level_0_0

calc_block_0x183d80_level_0_0:                    ; preds = %load_block_0x183d80_level_0_0
  %vb2 = call float @llvm.pow.f32(float %arg_buffer_val_, float 2.000000e+00)
  %vb4 = call float @llvm.cos.f32(float %arg_buffer_val_)
  %vb5 = fadd float %vb2, %vb4
  %vb6 = fpext float %vb5 to double
  br label %store_block_0x183d80_level_0_0

store_block_0x183d80_level_0_0:                   ; preds = %calc_block_0x183d80_level_0_0
  %1 = add i64 %common_offset_, 1
  store i64 %1, i64* %common_offset_alloca_, align 4
  %2 = icmp slt i64 %1, 524289
  %offset_op_incr_ = getelementptr inbounds double, double* %internal_buffer_1, i64 %common_offset_
  store double %vb6, double* %offset_op_incr_, align 8
  br i1 %2, label %load_block_0x183d80_level_0_0, label %intermediate_block_0x183d80_level_1_0

intermediate_block_0x183d80_level_1_0:            ; preds = %store_block_0x183d80_level_0_0
  store i64 0, i64* %common_offset_alloca_, align 4
  br label %load_block_0x183d80_level_1_0

load_block_0x183d80_level_1_0:                    ; preds = %store_block_0x183d80_level_1_0, %intermediate_block_0x183d80_level_1_0
  %common_offset_1 = load i64, i64* %common_offset_alloca_, align 4
  %offset_val_incr_ = getelementptr inbounds double, double* %internal_buffer_1, i64 %common_offset_1
  %buffer_val_ = load volatile double, double* %offset_val_incr_, align 8
  br label %calc_block_0x183d80_level_1_0

calc_block_0x183d80_level_1_0:                    ; preds = %load_block_0x183d80_level_1_0
  %vb13 = call double @convolveFunction(double* %offset_val_incr_, double* inttoptr (i64 1595572236192 to double*), i64 3, i64 -1)
  br label %store_block_0x183d80_level_1_0

store_block_0x183d80_level_1_0:                   ; preds = %calc_block_0x183d80_level_1_0
  %3 = add i64 %common_offset_1, 1
  store i64 %3, i64* %common_offset_alloca_, align 4
  %4 = icmp slt i64 %3, 524288
  %offset_op_incr_2 = getelementptr inbounds double, double* %internal_buffer_2, i64 %common_offset_1
  store double %vb13, double* %offset_op_incr_2, align 8
  br i1 %4, label %load_block_0x183d80_level_1_0, label %update_buffer_block

update_buffer_block:                              ; preds = %store_block_0x183d80_level_1_0
  %5 = call i32 @buffer_update_function(i32 0)
  br label %loop_enter_block

loop_enter_block:                                 ; preds = %cycle_exit_block, %update_buffer_block
  %glob_index = load i64, i64* %glob_index_alloca, align 4
  br label %intermediate_block_0x183d80_level_0

intermediate_block_0x183d80_level_0:              ; preds = %loop_enter_block
  store i64 1, i64* %common_offset_alloca_, align 4
  br label %load_block_0x183d80_level_0

load_block_0x183d80_level_0:                      ; preds = %store_block_0x183d80_level_0, %intermediate_block_0x183d80_level_0
  %common_offset_3 = load i64, i64* %common_offset_alloca_, align 4
  %offset_arg_incr_4 = getelementptr inbounds float, float* %external_buffer_0, i64 %common_offset_3
  %arg_buffer_val_5 = load volatile float, float* %offset_arg_incr_4, align 4
  br label %calc_block_0x183d80_level_0

calc_block_0x183d80_level_0:                      ; preds = %load_block_0x183d80_level_0
  %vb26 = call float @llvm.pow.f32(float %arg_buffer_val_5, float 2.000000e+00)
  %vb47 = call float @llvm.cos.f32(float %arg_buffer_val_5)
  %vb58 = fadd float %vb26, %vb47
  %vb69 = fpext float %vb58 to double
  br label %store_block_0x183d80_level_0

store_block_0x183d80_level_0:                     ; preds = %calc_block_0x183d80_level_0
  %6 = add i64 %common_offset_3, 1
  store i64 %6, i64* %common_offset_alloca_, align 4
  %7 = icmp slt i64 %6, 524289
  %offset_op_incr_10 = getelementptr inbounds double, double* %internal_buffer_1, i64 %common_offset_3
  store double %vb69, double* %offset_op_incr_10, align 8
  br i1 %7, label %load_block_0x183d80_level_0, label %intermediate_block_0x183d80_level_1

intermediate_block_0x183d80_level_1:              ; preds = %store_block_0x183d80_level_0
  store i64 0, i64* %common_offset_alloca_, align 4
  br label %load_block_0x183d80_level_1

load_block_0x183d80_level_1:                      ; preds = %store_block_0x183d80_level_1, %intermediate_block_0x183d80_level_1
  %common_offset_11 = load i64, i64* %common_offset_alloca_, align 4
  %offset_val_incr_12 = getelementptr inbounds double, double* %internal_buffer_1, i64 %common_offset_11
  %buffer_val_13 = load volatile double, double* %offset_val_incr_12, align 8
  br label %calc_block_0x183d80_level_1

calc_block_0x183d80_level_1:                      ; preds = %load_block_0x183d80_level_1
  %vb1314 = call double @convolveFunction(double* %offset_val_incr_12, double* inttoptr (i64 1595572236192 to double*), i64 3, i64 -1)
  br label %store_block_0x183d80_level_1

store_block_0x183d80_level_1:                     ; preds = %calc_block_0x183d80_level_1
  %8 = add i64 %common_offset_11, 1
  store i64 %8, i64* %common_offset_alloca_, align 4
  %9 = icmp slt i64 %8, 524288
  %offset_op_incr_15 = getelementptr inbounds double, double* %internal_buffer_2, i64 %common_offset_11
  store double %vb1314, double* %offset_op_incr_15, align 8
  br i1 %9, label %load_block_0x183d80_level_1, label %cycle_exit_block

cycle_exit_block:                                 ; preds = %store_block_0x183d80_level_1
  %10 = add i64 %glob_index, 1
  store i64 %10, i64* %glob_index_alloca, align 4
  %11 = icmp slt i64 %10, 2
  %12 = call i32 @buffer_update_function(i32 0)
  br i1 %11, label %loop_enter_block, label %terminal_loop_enter

terminal_loop_enter:                              ; preds = %cycle_exit_block
  br label %intermediate_block_0x183d80_level_016

intermediate_block_0x183d80_level_016:            ; preds = %terminal_loop_enter
  store i64 1, i64* %common_offset_alloca_, align 4
  br label %load_block_0x183d80_level_017

load_block_0x183d80_level_017:                    ; preds = %store_block_0x183d80_level_019, %intermediate_block_0x183d80_level_016
  %common_offset_20 = load i64, i64* %common_offset_alloca_, align 4
  %offset_arg_incr_21 = getelementptr inbounds float, float* %external_buffer_0, i64 %common_offset_20
  %arg_buffer_val_22 = load volatile float, float* %offset_arg_incr_21, align 4
  br label %calc_block_0x183d80_level_018

calc_block_0x183d80_level_018:                    ; preds = %load_block_0x183d80_level_017
  %vb223 = call float @llvm.pow.f32(float %arg_buffer_val_22, float 2.000000e+00)
  %vb424 = call float @llvm.cos.f32(float %arg_buffer_val_22)
  %vb525 = fadd float %vb223, %vb424
  %vb626 = fpext float %vb525 to double
  br label %store_block_0x183d80_level_019

store_block_0x183d80_level_019:                   ; preds = %calc_block_0x183d80_level_018
  %13 = add i64 %common_offset_20, 1
  store i64 %13, i64* %common_offset_alloca_, align 4
  %14 = icmp slt i64 %13, 15745
  %offset_op_incr_27 = getelementptr inbounds double, double* %internal_buffer_1, i64 %common_offset_20
  store double %vb626, double* %offset_op_incr_27, align 8
  br i1 %14, label %load_block_0x183d80_level_017, label %intermediate_block_0x183d80_level_128

intermediate_block_0x183d80_level_128:            ; preds = %store_block_0x183d80_level_019
  store i64 0, i64* %common_offset_alloca_, align 4
  br label %load_block_0x183d80_level_129

load_block_0x183d80_level_129:                    ; preds = %store_block_0x183d80_level_131, %intermediate_block_0x183d80_level_128
  %common_offset_32 = load i64, i64* %common_offset_alloca_, align 4
  %offset_val_incr_33 = getelementptr inbounds double, double* %internal_buffer_1, i64 %common_offset_32
  %buffer_val_34 = load volatile double, double* %offset_val_incr_33, align 8
  br label %calc_block_0x183d80_level_130

calc_block_0x183d80_level_130:                    ; preds = %load_block_0x183d80_level_129
  %vb1335 = call double @convolveFunction(double* %offset_val_incr_33, double* inttoptr (i64 1595572236192 to double*), i64 3, i64 -1)
  br label %store_block_0x183d80_level_131

store_block_0x183d80_level_131:                   ; preds = %calc_block_0x183d80_level_130
  %15 = add i64 %common_offset_32, 1
  store i64 %15, i64* %common_offset_alloca_, align 4
  %16 = icmp slt i64 %15, 15744
  %offset_op_incr_36 = getelementptr inbounds double, double* %internal_buffer_2, i64 %common_offset_32
  store double %vb1335, double* %offset_op_incr_36, align 8
  br i1 %16, label %load_block_0x183d80_level_129, label %exit_block

exit_block:                                       ; preds = %store_block_0x183d80_level_131
  %17 = call i32 @buffer_update_function(i32 0)
  ret i32 1
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare float @llvm.pow.f32(float, float) #0

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare float @llvm.cos.f32(float) #0

define double @convolveFunction(double* %0, double* %1, i64 %2, i64 %3) {
entry_block:
  %big_arr_base_ptr = getelementptr inbounds double, double* %0, i64 %3
  %index_ptr = alloca i64, align 8
  %sum_ptr = alloca double, align 8
  store double 0.000000e+00, double* %sum_ptr, align 8
  store i64 0, i64* %index_ptr, align 4
  br label %loop_body_block

loop_body_block:                                  ; preds = %loop_body_block, %entry_block
  %index = load i64, i64* %index_ptr, align 4
  %next_index = add i64 %index, 1
  %inv_index = sub i64 %2, %next_index
  %4 = getelementptr inbounds double, double* %big_arr_base_ptr, i64 %index
  %5 = getelementptr inbounds double, double* %1, i64 %inv_index
  %a = load double, double* %4, align 8
  %b = load double, double* %5, align 8
  %sum_v = load double, double* %sum_ptr, align 8
  %mull = fmul double %a, %b
  %next_sum = fadd double %mull, %sum_v
  store double %next_sum, double* %sum_ptr, align 8
  store i64 %next_index, i64* %index_ptr, align 4
  %6 = icmp slt i64 %next_index, %2
  br i1 %6, label %loop_body_block, label %exit_block

exit_block:                                       ; preds = %loop_body_block
  %7 = load double, double* %sum_ptr, align 8
  ret double %7
}

attributes #0 = { nofree nosync nounwind readnone speculatable willreturn }


