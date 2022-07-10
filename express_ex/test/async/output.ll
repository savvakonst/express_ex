

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
  %external_buffer_0 = bitcast i64* %external_untyped_buffer_0 to double*
  %internal_untyped_buffer_ptr_1 = getelementptr inbounds i64*, i64** %0, i32 1
  %internal_untyped_buffer_1 = load volatile i64*, i64** %internal_untyped_buffer_ptr_1, align 8
  %internal_buffer_1 = bitcast i64* %internal_untyped_buffer_1 to double*
  br label %intermediate_block_0x13b4_level_0_0

intermediate_block_0x13b4_level_0_0:              ; preds = %init_block
  store i64 0, i64* %common_offset_alloca_, align 4
  br label %load_block_0x13b4_level_0_0

load_block_0x13b4_level_0_0:                      ; preds = %store_block_0x13b4_level_0_0, %intermediate_block_0x13b4_level_0_0
  %common_offset_ = load i64, i64* %common_offset_alloca_, align 4
  %offset_arg_incr_ = getelementptr inbounds double, double* %external_buffer_0, i64 %common_offset_
  %arg_buffer_val_ = load volatile double, double* %offset_arg_incr_, align 8
  br label %calc_block_0x13b4_level_0_0

calc_block_0x13b4_level_0_0:                      ; preds = %load_block_0x13b4_level_0_0
  %vb2 = fmul double %arg_buffer_val_, 3.000000e+00
  br label %store_block_0x13b4_level_0_0

store_block_0x13b4_level_0_0:                     ; preds = %calc_block_0x13b4_level_0_0
  %1 = add i64 %common_offset_, 1
  store i64 %1, i64* %common_offset_alloca_, align 4
  %2 = icmp slt i64 %1, 2048
  %offset_op_incr_ = getelementptr inbounds double, double* %internal_buffer_1, i64 %common_offset_
  store double %vb2, double* %offset_op_incr_, align 8
  br i1 %2, label %load_block_0x13b4_level_0_0, label %update_buffer_block

update_buffer_block:                              ; preds = %store_block_0x13b4_level_0_0
  %3 = call i32 @buffer_update_function(i32 0)
  br label %loop_enter_block

loop_enter_block:                                 ; preds = %cycle_exit_block, %update_buffer_block
  %glob_index = load i64, i64* %glob_index_alloca, align 4
  br label %intermediate_block_0x13b4_level_0

intermediate_block_0x13b4_level_0:                ; preds = %loop_enter_block
  store i64 0, i64* %common_offset_alloca_, align 4
  br label %load_block_0x13b4_level_0

load_block_0x13b4_level_0:                        ; preds = %store_block_0x13b4_level_0, %intermediate_block_0x13b4_level_0
  %common_offset_1 = load i64, i64* %common_offset_alloca_, align 4
  %offset_arg_incr_2 = getelementptr inbounds double, double* %external_buffer_0, i64 %common_offset_1
  %arg_buffer_val_3 = load volatile double, double* %offset_arg_incr_2, align 8
  br label %calc_block_0x13b4_level_0

calc_block_0x13b4_level_0:                        ; preds = %load_block_0x13b4_level_0
  %vb24 = fmul double %arg_buffer_val_3, 3.000000e+00
  br label %store_block_0x13b4_level_0

store_block_0x13b4_level_0:                       ; preds = %calc_block_0x13b4_level_0
  %4 = add i64 %common_offset_1, 1
  store i64 %4, i64* %common_offset_alloca_, align 4
  %5 = icmp slt i64 %4, 2048
  %offset_op_incr_5 = getelementptr inbounds double, double* %internal_buffer_1, i64 %common_offset_1
  store double %vb24, double* %offset_op_incr_5, align 8
  br i1 %5, label %load_block_0x13b4_level_0, label %cycle_exit_block

cycle_exit_block:                                 ; preds = %store_block_0x13b4_level_0
  %6 = add i64 %glob_index, 1
  store i64 %6, i64* %glob_index_alloca, align 4
  %7 = icmp slt i64 %6, 1
  %8 = call i32 @buffer_update_function(i32 0)
  br i1 %7, label %loop_enter_block, label %terminal_loop_enter

terminal_loop_enter:                              ; preds = %cycle_exit_block
  br label %intermediate_block_0x13b4_level_06

intermediate_block_0x13b4_level_06:               ; preds = %terminal_loop_enter
  store i64 0, i64* %common_offset_alloca_, align 4
  br label %load_block_0x13b4_level_07

load_block_0x13b4_level_07:                       ; preds = %store_block_0x13b4_level_09, %intermediate_block_0x13b4_level_06
  %common_offset_10 = load i64, i64* %common_offset_alloca_, align 4
  %offset_arg_incr_11 = getelementptr inbounds double, double* %external_buffer_0, i64 %common_offset_10
  %arg_buffer_val_12 = load volatile double, double* %offset_arg_incr_11, align 8
  br label %calc_block_0x13b4_level_08

calc_block_0x13b4_level_08:                       ; preds = %load_block_0x13b4_level_07
  %vb213 = fmul double %arg_buffer_val_12, 3.000000e+00
  br label %store_block_0x13b4_level_09

store_block_0x13b4_level_09:                      ; preds = %calc_block_0x13b4_level_08
  %9 = add i64 %common_offset_10, 1
  store i64 %9, i64* %common_offset_alloca_, align 4
  %10 = icmp slt i64 %9, 948
  %offset_op_incr_14 = getelementptr inbounds double, double* %internal_buffer_1, i64 %common_offset_10
  store double %vb213, double* %offset_op_incr_14, align 8
  br i1 %10, label %load_block_0x13b4_level_07, label %exit_block

exit_block:                                       ; preds = %store_block_0x13b4_level_09
  %11 = call i32 @buffer_update_function(i32 0)
  ret i32 1
}


complete
