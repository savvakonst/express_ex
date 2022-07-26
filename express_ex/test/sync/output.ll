

----------------------LLVM module:--------------------
; ModuleID = 'test'
source_filename = "test"

declare i32 @buffer_update_function(i32)

define i32 @main(i64*** %0, i64** %1) {
init_block:
  %offset_incr = getelementptr inbounds i64**, i64*** %0, i32 0
  %2 = load i64**, i64*** %offset_incr, align 8
  %3 = call i32 @sub_main(i64** %2, i64** %1)
  ret i32 1
}

define i32 @sub_main(i64** %0, i64** %1) {
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
  %offset_op_incr_ = getelementptr inbounds double, double* %internal_buffer_1, i64 %common_offset_
  br label %calc_block_0x183d80_level_0_0

calc_block_0x183d80_level_0_0:                    ; preds = %load_block_0x183d80_level_0_0
  %vb1 = fpext float %arg_buffer_val_ to double
  %vb4 = call double @llvm.pow.f64(double %vb1, double 2.000000e+00)
  %vb6 = call double @llvm.cos.f64(double %vb1)
  %vb7 = fadd double %vb4, %vb6
  br label %store_block_0x183d80_level_0_0

store_block_0x183d80_level_0_0:                   ; preds = %calc_block_0x183d80_level_0_0
  %2 = add i64 %common_offset_, 1
  store i64 %2, i64* %common_offset_alloca_, align 4
  %3 = icmp slt i64 %2, 524289
  store double %vb7, double* %offset_op_incr_, align 8
  br i1 %3, label %load_block_0x183d80_level_0_0, label %intermediate_block_0x183d80_level_1_0

intermediate_block_0x183d80_level_1_0:            ; preds = %store_block_0x183d80_level_0_0
  store i64 0, i64* %common_offset_alloca_, align 4
  br label %load_block_0x183d80_level_1_0

load_block_0x183d80_level_1_0:                    ; preds = %store_block_0x183d80_level_1_0, %intermediate_block_0x183d80_level_1_0
  %common_offset_1 = load i64, i64* %common_offset_alloca_, align 4
  %offset_val_incr_ = getelementptr inbounds double, double* %internal_buffer_1, i64 %common_offset_1
  %buffer_val_ = load volatile double, double* %offset_val_incr_, align 8
  %offset_op_incr_2 = getelementptr inbounds double, double* %internal_buffer_2, i64 %common_offset_1
  br label %calc_block_0x183d80_level_1_0

calc_block_0x183d80_level_1_0:                    ; preds = %load_block_0x183d80_level_1_0
  %vb14 = call double @convolveFunction(double* %offset_val_incr_, double* inttoptr (i64 3089307718256 to double*), i64 3, i64 -1)
  br label %store_block_0x183d80_level_1_0

store_block_0x183d80_level_1_0:                   ; preds = %calc_block_0x183d80_level_1_0
  %4 = add i64 %common_offset_1, 1
  store i64 %4, i64* %common_offset_alloca_, align 4
  %5 = icmp slt i64 %4, 524288
  store double %vb14, double* %offset_op_incr_2, align 8
  br i1 %5, label %load_block_0x183d80_level_1_0, label %update_buffer_block

update_buffer_block:                              ; preds = %store_block_0x183d80_level_1_0
  %6 = call i32 @buffer_update_function(i32 0)
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
  %offset_op_incr_10 = getelementptr inbounds double, double* %internal_buffer_1, i64 %common_offset_3
  br label %calc_block_0x183d80_level_0

calc_block_0x183d80_level_0:                      ; preds = %load_block_0x183d80_level_0
  %vb16 = fpext float %arg_buffer_val_5 to double
  %vb47 = call double @llvm.pow.f64(double %vb16, double 2.000000e+00)
  %vb68 = call double @llvm.cos.f64(double %vb16)
  %vb79 = fadd double %vb47, %vb68
  br label %store_block_0x183d80_level_0

store_block_0x183d80_level_0:                     ; preds = %calc_block_0x183d80_level_0
  %7 = add i64 %common_offset_3, 1
  store i64 %7, i64* %common_offset_alloca_, align 4
  %8 = icmp slt i64 %7, 524289
  store double %vb79, double* %offset_op_incr_10, align 8
  br i1 %8, label %load_block_0x183d80_level_0, label %intermediate_block_0x183d80_level_1

intermediate_block_0x183d80_level_1:              ; preds = %store_block_0x183d80_level_0
  store i64 0, i64* %common_offset_alloca_, align 4
  br label %load_block_0x183d80_level_1

load_block_0x183d80_level_1:                      ; preds = %store_block_0x183d80_level_1, %intermediate_block_0x183d80_level_1
  %common_offset_11 = load i64, i64* %common_offset_alloca_, align 4
  %offset_val_incr_12 = getelementptr inbounds double, double* %internal_buffer_1, i64 %common_offset_11
  %buffer_val_13 = load volatile double, double* %offset_val_incr_12, align 8
  %offset_op_incr_15 = getelementptr inbounds double, double* %internal_buffer_2, i64 %common_offset_11
  br label %calc_block_0x183d80_level_1

calc_block_0x183d80_level_1:                      ; preds = %load_block_0x183d80_level_1
  %vb1414 = call double @convolveFunction(double* %offset_val_incr_12, double* inttoptr (i64 3089307718256 to double*), i64 3, i64 -1)
  br label %store_block_0x183d80_level_1

store_block_0x183d80_level_1:                     ; preds = %calc_block_0x183d80_level_1
  %9 = add i64 %common_offset_11, 1
  store i64 %9, i64* %common_offset_alloca_, align 4
  %10 = icmp slt i64 %9, 524288
  store double %vb1414, double* %offset_op_incr_15, align 8
  br i1 %10, label %load_block_0x183d80_level_1, label %cycle_exit_block

cycle_exit_block:                                 ; preds = %store_block_0x183d80_level_1
  %11 = add i64 %glob_index, 1
  store i64 %11, i64* %glob_index_alloca, align 4
  %12 = icmp slt i64 %11, 2
  %13 = call i32 @buffer_update_function(i32 0)
  br i1 %12, label %loop_enter_block, label %terminal_loop_enter

terminal_loop_enter:                              ; preds = %cycle_exit_block
  br label %intermediate_block_0x183d80_level_016

intermediate_block_0x183d80_level_016:            ; preds = %terminal_loop_enter
  store i64 1, i64* %common_offset_alloca_, align 4
  br label %load_block_0x183d80_level_017

load_block_0x183d80_level_017:                    ; preds = %store_block_0x183d80_level_019, %intermediate_block_0x183d80_level_016
  %common_offset_20 = load i64, i64* %common_offset_alloca_, align 4
  %offset_arg_incr_21 = getelementptr inbounds float, float* %external_buffer_0, i64 %common_offset_20
  %arg_buffer_val_22 = load volatile float, float* %offset_arg_incr_21, align 4
  %offset_op_incr_27 = getelementptr inbounds double, double* %internal_buffer_1, i64 %common_offset_20
  br label %calc_block_0x183d80_level_018

calc_block_0x183d80_level_018:                    ; preds = %load_block_0x183d80_level_017
  %vb123 = fpext float %arg_buffer_val_22 to double
  %vb424 = call double @llvm.pow.f64(double %vb123, double 2.000000e+00)
  %vb625 = call double @llvm.cos.f64(double %vb123)
  %vb726 = fadd double %vb424, %vb625
  br label %store_block_0x183d80_level_019

store_block_0x183d80_level_019:                   ; preds = %calc_block_0x183d80_level_018
  %14 = add i64 %common_offset_20, 1
  store i64 %14, i64* %common_offset_alloca_, align 4
  %15 = icmp slt i64 %14, 15745
  store double %vb726, double* %offset_op_incr_27, align 8
  br i1 %15, label %load_block_0x183d80_level_017, label %intermediate_block_0x183d80_level_128

intermediate_block_0x183d80_level_128:            ; preds = %store_block_0x183d80_level_019
  store i64 0, i64* %common_offset_alloca_, align 4
  br label %load_block_0x183d80_level_129

load_block_0x183d80_level_129:                    ; preds = %store_block_0x183d80_level_131, %intermediate_block_0x183d80_level_128
  %common_offset_32 = load i64, i64* %common_offset_alloca_, align 4
  %offset_val_incr_33 = getelementptr inbounds double, double* %internal_buffer_1, i64 %common_offset_32
  %buffer_val_34 = load volatile double, double* %offset_val_incr_33, align 8
  %offset_op_incr_36 = getelementptr inbounds double, double* %internal_buffer_2, i64 %common_offset_32
  br label %calc_block_0x183d80_level_130

calc_block_0x183d80_level_130:                    ; preds = %load_block_0x183d80_level_129
  %vb1435 = call double @convolveFunction(double* %offset_val_incr_33, double* inttoptr (i64 3089307718256 to double*), i64 3, i64 -1)
  br label %store_block_0x183d80_level_131

store_block_0x183d80_level_131:                   ; preds = %calc_block_0x183d80_level_130
  %16 = add i64 %common_offset_32, 1
  store i64 %16, i64* %common_offset_alloca_, align 4
  %17 = icmp slt i64 %16, 15744
  store double %vb1435, double* %offset_op_incr_36, align 8
  br i1 %17, label %load_block_0x183d80_level_129, label %exit_block

exit_block:                                       ; preds = %store_block_0x183d80_level_131
  %18 = call i32 @buffer_update_function(i32 0)
  ret i32 1
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare double @llvm.pow.f64(double, double) #0

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare double @llvm.cos.f64(double) #0

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


complete
