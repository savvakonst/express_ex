

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
  %external_untyped_buffer_ptr_1 = getelementptr inbounds i64*, i64** %0, i32 1
  %external_untyped_buffer_1 = load volatile i64*, i64** %external_untyped_buffer_ptr_1, align 8
  %external_buffer_1 = bitcast i64* %external_untyped_buffer_1 to float*
  %internal_untyped_buffer_ptr_2 = getelementptr inbounds i64*, i64** %0, i32 2
  %internal_untyped_buffer_2 = load volatile i64*, i64** %internal_untyped_buffer_ptr_2, align 8
  %internal_buffer_2 = bitcast i64* %internal_untyped_buffer_2 to float*
  br label %intermediate_block_0x635b_level_0_0

intermediate_block_0x635b_level_0_0:              ; preds = %init_block
  store i64 0, i64* %common_offset_alloca_, align 4
  br label %load_block_0x635b_level_0_0

load_block_0x635b_level_0_0:                      ; preds = %store_block_0x635b_level_0_0, %intermediate_block_0x635b_level_0_0
  %common_offset_ = load i64, i64* %common_offset_alloca_, align 4
  %offset_arg_incr_ = getelementptr inbounds float, float* %external_buffer_0, i64 %common_offset_
  %arg_buffer_val_ = load volatile float, float* %offset_arg_incr_, align 4
  %offset_arg_incr_1 = getelementptr inbounds float, float* %external_buffer_1, i64 %common_offset_
  %arg_buffer_val_2 = load volatile float, float* %offset_arg_incr_1, align 4
  %offset_op_incr_ = getelementptr inbounds float, float* %internal_buffer_2, i64 %common_offset_
  br label %calc_block_0x635b_level_0_0

calc_block_0x635b_level_0_0:                      ; preds = %load_block_0x635b_level_0_0
  %vb1 = fpext float %arg_buffer_val_ to double
  %vb3 = fpext float %arg_buffer_val_2 to double
  %vb5 = call double @llvm.pow.f64(double %vb3, double 2.000000e+00)
  %vb6 = fadd double %vb1, %vb5
  %vb8 = fptrunc double %vb6 to float
  br label %store_block_0x635b_level_0_0

store_block_0x635b_level_0_0:                     ; preds = %calc_block_0x635b_level_0_0
  %2 = add i64 %common_offset_, 1
  store i64 %2, i64* %common_offset_alloca_, align 4
  %3 = icmp slt i64 %2, 8192
  store float %vb8, float* %offset_op_incr_, align 4
  br i1 %3, label %load_block_0x635b_level_0_0, label %update_buffer_block

update_buffer_block:                              ; preds = %store_block_0x635b_level_0_0
  %4 = call i32 @buffer_update_function(i32 0)
  br label %loop_enter_block

loop_enter_block:                                 ; preds = %cycle_exit_block, %update_buffer_block
  %glob_index = load i64, i64* %glob_index_alloca, align 4
  br label %intermediate_block_0x635b_level_0

intermediate_block_0x635b_level_0:                ; preds = %loop_enter_block
  store i64 0, i64* %common_offset_alloca_, align 4
  br label %load_block_0x635b_level_0

load_block_0x635b_level_0:                        ; preds = %store_block_0x635b_level_0, %intermediate_block_0x635b_level_0
  %common_offset_3 = load i64, i64* %common_offset_alloca_, align 4
  %offset_arg_incr_4 = getelementptr inbounds float, float* %external_buffer_0, i64 %common_offset_3
  %arg_buffer_val_5 = load volatile float, float* %offset_arg_incr_4, align 4
  %offset_arg_incr_6 = getelementptr inbounds float, float* %external_buffer_1, i64 %common_offset_3
  %arg_buffer_val_7 = load volatile float, float* %offset_arg_incr_6, align 4
  %offset_op_incr_13 = getelementptr inbounds float, float* %internal_buffer_2, i64 %common_offset_3
  br label %calc_block_0x635b_level_0

calc_block_0x635b_level_0:                        ; preds = %load_block_0x635b_level_0
  %vb18 = fpext float %arg_buffer_val_5 to double
  %vb39 = fpext float %arg_buffer_val_7 to double
  %vb510 = call double @llvm.pow.f64(double %vb39, double 2.000000e+00)
  %vb611 = fadd double %vb18, %vb510
  %vb812 = fptrunc double %vb611 to float
  br label %store_block_0x635b_level_0

store_block_0x635b_level_0:                       ; preds = %calc_block_0x635b_level_0
  %5 = add i64 %common_offset_3, 1
  store i64 %5, i64* %common_offset_alloca_, align 4
  %6 = icmp slt i64 %5, 8192
  store float %vb812, float* %offset_op_incr_13, align 4
  br i1 %6, label %load_block_0x635b_level_0, label %cycle_exit_block

cycle_exit_block:                                 ; preds = %store_block_0x635b_level_0
  %7 = add i64 %glob_index, 1
  store i64 %7, i64* %glob_index_alloca, align 4
  %8 = icmp slt i64 %7, 2
  %9 = call i32 @buffer_update_function(i32 0)
  br i1 %8, label %loop_enter_block, label %terminal_loop_enter

terminal_loop_enter:                              ; preds = %cycle_exit_block
  br label %intermediate_block_0x635b_level_014

intermediate_block_0x635b_level_014:              ; preds = %terminal_loop_enter
  store i64 0, i64* %common_offset_alloca_, align 4
  br label %load_block_0x635b_level_015

load_block_0x635b_level_015:                      ; preds = %store_block_0x635b_level_017, %intermediate_block_0x635b_level_014
  %common_offset_18 = load i64, i64* %common_offset_alloca_, align 4
  %offset_arg_incr_19 = getelementptr inbounds float, float* %external_buffer_0, i64 %common_offset_18
  %arg_buffer_val_20 = load volatile float, float* %offset_arg_incr_19, align 4
  %offset_arg_incr_21 = getelementptr inbounds float, float* %external_buffer_1, i64 %common_offset_18
  %arg_buffer_val_22 = load volatile float, float* %offset_arg_incr_21, align 4
  %offset_op_incr_28 = getelementptr inbounds float, float* %internal_buffer_2, i64 %common_offset_18
  br label %calc_block_0x635b_level_016

calc_block_0x635b_level_016:                      ; preds = %load_block_0x635b_level_015
  %vb123 = fpext float %arg_buffer_val_20 to double
  %vb324 = fpext float %arg_buffer_val_22 to double
  %vb525 = call double @llvm.pow.f64(double %vb324, double 2.000000e+00)
  %vb626 = fadd double %vb123, %vb525
  %vb827 = fptrunc double %vb626 to float
  br label %store_block_0x635b_level_017

store_block_0x635b_level_017:                     ; preds = %calc_block_0x635b_level_016
  %10 = add i64 %common_offset_18, 1
  store i64 %10, i64* %common_offset_alloca_, align 4
  %11 = icmp slt i64 %10, 859
  store float %vb827, float* %offset_op_incr_28, align 4
  br i1 %11, label %load_block_0x635b_level_015, label %exit_block

exit_block:                                       ; preds = %store_block_0x635b_level_017
  %12 = call i32 @buffer_update_function(i32 0)
  ret i32 1
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare double @llvm.pow.f64(double, double) #0

attributes #0 = { nofree nosync nounwind readnone speculatable willreturn }


complete
