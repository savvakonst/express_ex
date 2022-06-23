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
  %offset_alloca_ = alloca i64, align 8
  store i64 -1, i64* %offset_alloca_, align 4
  %external_untyped_buffer_ptr_0 = getelementptr inbounds i64*, i64** %0, i32 0
  %external_untyped_buffer_0 = load volatile i64*, i64** %external_untyped_buffer_ptr_0, align 8
  %external_buffer_0 = bitcast i64* %external_untyped_buffer_0 to float*
  %internal_untyped_buffer_ptr_1 = getelementptr inbounds i64*, i64** %0, i32 1
  %internal_untyped_buffer_1 = load volatile i64*, i64** %internal_untyped_buffer_ptr_1, align 8
  %internal_buffer_1 = bitcast i64* %internal_untyped_buffer_1 to double*
  %offset_alloca_1 = alloca i64, align 8
  store i64 0, i64* %offset_alloca_1, align 4
  %internal_untyped_buffer_ptr_2 = getelementptr inbounds i64*, i64** %0, i32 2
  %internal_untyped_buffer_2 = load volatile i64*, i64** %internal_untyped_buffer_ptr_2, align 8
  %internal_buffer_2 = bitcast i64* %internal_untyped_buffer_2 to double*
  %offset_alloca_4 = alloca i64, align 8
  store i64 0, i64* %offset_alloca_4, align 4
  %offset_alloca_5 = alloca i64, align 8
  store i64 0, i64* %offset_alloca_5, align 4
  br label %intermediate_block_0x183d80_level_0_0

intermediate_block_0x183d80_level_0_0:            ; preds = %init_block
  br label %load_block_0x183d80_level_0_0

load_block_0x183d80_level_0_0:                    ; preds = %store_block_0x183d80_level_0_0, %intermediate_block_0x183d80_level_0_0
  %offset = load i64, i64* %offset_alloca_, align 4
  %offset_arg_incr_ = getelementptr inbounds float, float* %external_buffer_0, i64 %offset
  %arg_buffer_val_ = load volatile float, float* %offset_arg_incr_, align 4
  br label %calc_block_0x183d80_level_0_0

calc_block_0x183d80_level_0_0:                    ; preds = %load_block_0x183d80_level_0_0
  %vb1 = fpext float %arg_buffer_val_ to double
  br label %store_block_0x183d80_level_0_0

store_block_0x183d80_level_0_0:                   ; preds = %calc_block_0x183d80_level_0_0
  %1 = add i64 %offset, 1
  store i64 %1, i64* %offset_alloca_, align 4
  %2 = icmp slt i64 %1, 524289
  %offset_op_incr_ = getelementptr inbounds double, double* %internal_buffer_1, i64 %offset
  store double %vb1, double* %offset_op_incr_, align 8
  br i1 %2, label %load_block_0x183d80_level_0_0, label %intermediate_block_0x183d80_level_1_0

intermediate_block_0x183d80_level_1_0:            ; preds = %store_block_0x183d80_level_0_0
  br label %load_block_0x183d80_level_1_0

load_block_0x183d80_level_1_0:                    ; preds = %store_block_0x183d80_level_1_0, %intermediate_block_0x183d80_level_1_0
  %offset2 = load i64, i64* %offset_alloca_1, align 4
  %offset_val_incr_ = getelementptr inbounds double, double* %internal_buffer_1, i64 %offset2
  %buffer_val_ = load volatile double, double* %offset_val_incr_, align 8
  br label %calc_block_0x183d80_level_1_0

calc_block_0x183d80_level_1_0:                    ; preds = %load_block_0x183d80_level_1_0
  %vb8 = call double @convolveFunction(double* %offset_val_incr_, double* inttoptr (i64 2682824874432 to double*), i64 3, i64 -1)
  br label %store_block_0x183d80_level_1_0

store_block_0x183d80_level_1_0:                   ; preds = %calc_block_0x183d80_level_1_0
  %3 = add i64 %offset2, 1
  store i64 %3, i64* %offset_alloca_1, align 4
  %4 = icmp slt i64 %3, 524288
  %offset_op_incr_3 = getelementptr inbounds double, double* %internal_buffer_2, i64 %offset2
  store double %vb8, double* %offset_op_incr_3, align 8
  br i1 %4, label %load_block_0x183d80_level_1_0, label %update_buffer_block

update_buffer_block:                              ; preds = %store_block_0x183d80_level_1_0
  %5 = call i32 @buffer_update_function(i32 0)
  br label %loop_enter_block

loop_enter_block:                                 ; preds = %cycle_exit_block, %update_buffer_block
  %glob_index = load i64, i64* %offset_alloca_4, align 4
  br label %intermediate_block_0x183d80_level_0

intermediate_block_0x183d80_level_0:              ; preds = %loop_enter_block
  store i64 1, i64* %offset_alloca_5, align 4
  br label %load_block_0x183d80_level_0

load_block_0x183d80_level_0:                      ; preds = %store_block_0x183d80_level_0, %intermediate_block_0x183d80_level_0
  %common_offset_ = load i64, i64* %offset_alloca_5, align 4
  %offset_arg_incr_6 = getelementptr inbounds float, float* %external_buffer_0, i64 %common_offset_
  %arg_buffer_val_7 = load volatile float, float* %offset_arg_incr_6, align 4
  br label %calc_block_0x183d80_level_0

calc_block_0x183d80_level_0:                      ; preds = %load_block_0x183d80_level_0
  %vb18 = fpext float %arg_buffer_val_7 to double
  br label %store_block_0x183d80_level_0

store_block_0x183d80_level_0:                     ; preds = %calc_block_0x183d80_level_0
  %6 = add i64 %common_offset_, 1
  store i64 %6, i64* %offset_alloca_5, align 4
  %7 = icmp slt i64 %6, 524289
  %offset_op_incr_9 = getelementptr inbounds double, double* %internal_buffer_1, i64 %common_offset_
  store double %vb18, double* %offset_op_incr_9, align 8
  br i1 %7, label %load_block_0x183d80_level_0, label %intermediate_block_0x183d80_level_1

intermediate_block_0x183d80_level_1:              ; preds = %store_block_0x183d80_level_0
  store i64 0, i64* %offset_alloca_5, align 4
  br label %load_block_0x183d80_level_1

load_block_0x183d80_level_1:                      ; preds = %store_block_0x183d80_level_1, %intermediate_block_0x183d80_level_1
  %common_offset_10 = load i64, i64* %offset_alloca_5, align 4
  %offset_val_incr_11 = getelementptr inbounds double, double* %internal_buffer_1, i64 %common_offset_10
  %buffer_val_12 = load volatile double, double* %offset_val_incr_11, align 8
  br label %calc_block_0x183d80_level_1

calc_block_0x183d80_level_1:                      ; preds = %load_block_0x183d80_level_1
  %vb813 = call double @convolveFunction(double* %offset_val_incr_11, double* inttoptr (i64 2682824874432 to double*), i64 3, i64 -1)
  br label %store_block_0x183d80_level_1

store_block_0x183d80_level_1:                     ; preds = %calc_block_0x183d80_level_1
  %8 = add i64 %common_offset_10, 1
  store i64 %8, i64* %offset_alloca_5, align 4
  %9 = icmp slt i64 %8, 524288
  %offset_op_incr_14 = getelementptr inbounds double, double* %internal_buffer_2, i64 %common_offset_10
  store double %vb813, double* %offset_op_incr_14, align 8
  br i1 %9, label %load_block_0x183d80_level_1, label %cycle_exit_block

cycle_exit_block:                                 ; preds = %store_block_0x183d80_level_1
  %10 = add i64 %glob_index, 1
  store i64 %10, i64* %offset_alloca_4, align 4
  %11 = icmp slt i64 %10, 2
  %12 = call i32 @buffer_update_function(i32 0)
  br i1 %11, label %loop_enter_block, label %terminal_loop_enter

terminal_loop_enter:                              ; preds = %cycle_exit_block
  br label %intermediate_block_0x183d80_level_015

intermediate_block_0x183d80_level_015:            ; preds = %terminal_loop_enter
  store i64 1, i64* %offset_alloca_5, align 4
  br label %load_block_0x183d80_level_016

load_block_0x183d80_level_016:                    ; preds = %store_block_0x183d80_level_018, %intermediate_block_0x183d80_level_015
  %common_offset_19 = load i64, i64* %offset_alloca_5, align 4
  %offset_arg_incr_20 = getelementptr inbounds float, float* %external_buffer_0, i64 %common_offset_19
  %arg_buffer_val_21 = load volatile float, float* %offset_arg_incr_20, align 4
  br label %calc_block_0x183d80_level_017

calc_block_0x183d80_level_017:                    ; preds = %load_block_0x183d80_level_016
  %vb122 = fpext float %arg_buffer_val_21 to double
  br label %store_block_0x183d80_level_018

store_block_0x183d80_level_018:                   ; preds = %calc_block_0x183d80_level_017
  %13 = add i64 %common_offset_19, 1
  store i64 %13, i64* %offset_alloca_5, align 4
  %14 = icmp slt i64 %13, 15745
  %offset_op_incr_23 = getelementptr inbounds double, double* %internal_buffer_1, i64 %common_offset_19
  store double %vb122, double* %offset_op_incr_23, align 8
  br i1 %14, label %load_block_0x183d80_level_016, label %intermediate_block_0x183d80_level_124

intermediate_block_0x183d80_level_124:            ; preds = %store_block_0x183d80_level_018
  store i64 0, i64* %offset_alloca_5, align 4
  br label %load_block_0x183d80_level_125

load_block_0x183d80_level_125:                    ; preds = %store_block_0x183d80_level_127, %intermediate_block_0x183d80_level_124
  %common_offset_28 = load i64, i64* %offset_alloca_5, align 4
  %offset_val_incr_29 = getelementptr inbounds double, double* %internal_buffer_1, i64 %common_offset_28
  %buffer_val_30 = load volatile double, double* %offset_val_incr_29, align 8
  br label %calc_block_0x183d80_level_126

calc_block_0x183d80_level_126:                    ; preds = %load_block_0x183d80_level_125
  %vb831 = call double @convolveFunction(double* %offset_val_incr_29, double* inttoptr (i64 2682824874432 to double*), i64 3, i64 -1)
  br label %store_block_0x183d80_level_127

store_block_0x183d80_level_127:                   ; preds = %calc_block_0x183d80_level_126
  %15 = add i64 %common_offset_28, 1
  store i64 %15, i64* %offset_alloca_5, align 4
  %16 = icmp slt i64 %15, 15744
  %offset_op_incr_32 = getelementptr inbounds double, double* %internal_buffer_2, i64 %common_offset_28
  store double %vb831, double* %offset_op_incr_32, align 8
  br i1 %16, label %load_block_0x183d80_level_125, label %exit_block

exit_block:                                       ; preds = %store_block_0x183d80_level_127
  %17 = call i32 @buffer_update_function(i32 0)
  ret i32 1
}

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
