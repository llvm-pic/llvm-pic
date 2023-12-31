; NOTE: Assertions have been autogenerated by utils/update_test_checks.py UTC_ARGS: --version 2
; RUN: opt -passes=gvn -S < %s | FileCheck %s


define <4 x float> @ConvertVectors_ByRef(ptr %loc) {
; CHECK-LABEL: define <4 x float> @ConvertVectors_ByRef
; CHECK-SAME: (ptr [[LOC:%.*]]) {
; CHECK-NEXT:    [[LOAD_VEC:%.*]] = load <4 x float>, ptr [[LOC]], align 16
; CHECK-NEXT:    [[SHUF:%.*]] = shufflevector <4 x float> [[LOAD_VEC]], <4 x float> poison, <4 x i32> <i32 0, i32 poison, i32 poison, i32 poison>
; CHECK-NEXT:    [[GEP1:%.*]] = getelementptr inbounds [4 x float], ptr [[LOC]], i64 0, i64 1
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast <4 x float> [[LOAD_VEC]] to i128
; CHECK-NEXT:    [[TMP2:%.*]] = lshr i128 [[TMP1]], 32
; CHECK-NEXT:    [[TMP3:%.*]] = trunc i128 [[TMP2]] to i32
; CHECK-NEXT:    [[TMP4:%.*]] = bitcast i32 [[TMP3]] to float
; CHECK-NEXT:    [[INS1:%.*]] = insertelement <4 x float> [[SHUF]], float [[TMP4]], i64 1
; CHECK-NEXT:    [[GEP2:%.*]] = getelementptr inbounds [4 x float], ptr [[LOC]], i64 0, i64 2
; CHECK-NEXT:    [[TMP5:%.*]] = lshr i128 [[TMP1]], 64
; CHECK-NEXT:    [[TMP6:%.*]] = trunc i128 [[TMP5]] to i32
; CHECK-NEXT:    [[TMP7:%.*]] = bitcast i32 [[TMP6]] to float
; CHECK-NEXT:    [[INS2:%.*]] = insertelement <4 x float> [[INS1]], float [[TMP7]], i64 2
; CHECK-NEXT:    [[INS3:%.*]] = insertelement <4 x float> [[INS2]], float [[TMP7]], i64 3
; CHECK-NEXT:    ret <4 x float> [[INS3]]
;
  %load_vec = load <4 x float>, ptr %loc, align 16
  %shuf = shufflevector <4 x float> %load_vec, <4 x float> poison, <4 x i32> <i32 0, i32 poison, i32 poison, i32 poison>
  %gep1 = getelementptr inbounds [4 x float], ptr %loc, i64 0, i64 1
  %load1 = load float, ptr %gep1, align 4
  %ins1 = insertelement <4 x float> %shuf, float %load1, i64 1
  %gep2 = getelementptr inbounds [4 x float], ptr %loc, i64 0, i64 2
  %load2 = load float, ptr %gep2, align 8
  %ins2 = insertelement <4 x float> %ins1, float %load2, i64 2
  %ins3 = insertelement <4 x float> %ins2, float %load2, i64 3
  ret <4 x float> %ins3
}

define i64 @store_element_smaller_than_load(ptr %loc, <4 x i32> %v) {
; CHECK-LABEL: define i64 @store_element_smaller_than_load
; CHECK-SAME: (ptr [[LOC:%.*]], <4 x i32> [[V:%.*]]) {
; CHECK-NEXT:  entry:
; CHECK-NEXT:    store <4 x i32> [[V]], ptr [[LOC]], align 16
; CHECK-NEXT:    [[GEP:%.*]] = getelementptr inbounds [4 x i32], ptr [[LOC]], i64 0, i64 2
; CHECK-NEXT:    [[TMP0:%.*]] = bitcast <4 x i32> [[V]] to i128
; CHECK-NEXT:    [[TMP1:%.*]] = lshr i128 [[TMP0]], 64
; CHECK-NEXT:    [[TMP2:%.*]] = trunc i128 [[TMP1]] to i64
; CHECK-NEXT:    ret i64 [[TMP2]]
;
  entry:
  store <4 x i32> %v, ptr  %loc
  %gep = getelementptr inbounds [4 x i32], ptr %loc, i64 0, i64 2
  %ref = load i64, ptr %gep
  ret i64 %ref
}

define i64 @call_before_load(ptr %loc, <4 x i32> %v) {
; CHECK-LABEL: define i64 @call_before_load
; CHECK-SAME: (ptr [[LOC:%.*]], <4 x i32> [[V:%.*]]) {
; CHECK-NEXT:  entry:
; CHECK-NEXT:    store <4 x i32> [[V]], ptr [[LOC]], align 16
; CHECK-NEXT:    call void @f(<4 x i32> [[V]])
; CHECK-NEXT:    [[GEP:%.*]] = getelementptr inbounds [4 x i32], ptr [[LOC]], i64 0, i64 2
; CHECK-NEXT:    [[REF:%.*]] = load i64, ptr [[GEP]], align 4
; CHECK-NEXT:    ret i64 [[REF]]
;
  entry:
  store <4 x i32> %v, ptr  %loc
  call void @f(<4 x i32> %v)
  %gep = getelementptr inbounds [4 x i32], ptr %loc, i64 0, i64 2
  %ref = load i64, ptr %gep
  ret i64 %ref
}

define i64 @call_before_load_memory_none(ptr %loc, <4 x i32> %v) {
; CHECK-LABEL: define i64 @call_before_load_memory_none
; CHECK-SAME: (ptr [[LOC:%.*]], <4 x i32> [[V:%.*]]) {
; CHECK-NEXT:  entry:
; CHECK-NEXT:    store <4 x i32> [[V]], ptr [[LOC]], align 16
; CHECK-NEXT:    call void @f_no_mem(<4 x i32> [[V]])
; CHECK-NEXT:    [[GEP:%.*]] = getelementptr inbounds [4 x i32], ptr [[LOC]], i64 0, i64 2
; CHECK-NEXT:    [[TMP0:%.*]] = bitcast <4 x i32> [[V]] to i128
; CHECK-NEXT:    [[TMP1:%.*]] = lshr i128 [[TMP0]], 64
; CHECK-NEXT:    [[TMP2:%.*]] = trunc i128 [[TMP1]] to i64
; CHECK-NEXT:    ret i64 [[TMP2]]
;
  entry:
  store <4 x i32> %v, ptr  %loc
  call void @f_no_mem(<4 x i32> %v)
  %gep = getelementptr inbounds [4 x i32], ptr %loc, i64 0, i64 2
  %ref = load i64, ptr %gep
  ret i64 %ref
}

define i64 @call_after_load(ptr %loc, <4 x i32> %v) {
; CHECK-LABEL: define i64 @call_after_load
; CHECK-SAME: (ptr [[LOC:%.*]], <4 x i32> [[V:%.*]]) {
; CHECK-NEXT:  entry:
; CHECK-NEXT:    store <4 x i32> [[V]], ptr [[LOC]], align 16
; CHECK-NEXT:    [[GEP:%.*]] = getelementptr inbounds [4 x i32], ptr [[LOC]], i64 0, i64 2
; CHECK-NEXT:    [[TMP0:%.*]] = bitcast <4 x i32> [[V]] to i128
; CHECK-NEXT:    [[TMP1:%.*]] = lshr i128 [[TMP0]], 64
; CHECK-NEXT:    [[TMP2:%.*]] = trunc i128 [[TMP1]] to i64
; CHECK-NEXT:    call void @f(<4 x i32> [[V]])
; CHECK-NEXT:    ret i64 [[TMP2]]
;
  entry:
  store <4 x i32> %v, ptr  %loc
  %gep = getelementptr inbounds [4 x i32], ptr %loc, i64 0, i64 2
  %ref = load i64, ptr %gep
  call void @f(<4 x i32> %v)
  ret i64 %ref
}

define double @store_element_smaller_than_load_float(ptr %loc, <4 x float> %v) {
; CHECK-LABEL: define double @store_element_smaller_than_load_float
; CHECK-SAME: (ptr [[LOC:%.*]], <4 x float> [[V:%.*]]) {
; CHECK-NEXT:    store <4 x float> [[V]], ptr [[LOC]], align 16
; CHECK-NEXT:    [[GEP:%.*]] = getelementptr inbounds [4 x float], ptr [[LOC]], i64 0, i64 2
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast <4 x float> [[V]] to i128
; CHECK-NEXT:    [[TMP2:%.*]] = lshr i128 [[TMP1]], 64
; CHECK-NEXT:    [[TMP3:%.*]] = trunc i128 [[TMP2]] to i64
; CHECK-NEXT:    [[TMP4:%.*]] = bitcast i64 [[TMP3]] to double
; CHECK-NEXT:    ret double [[TMP4]]
;
  store <4 x float> %v, ptr  %loc
  %gep = getelementptr inbounds [4 x float], ptr %loc, i64 0, i64 2
  %ref = load double, ptr %gep
  ret double %ref
}

define i64 @load_as_scalar(ptr %loc, <2 x i32> %v) {
; CHECK-LABEL: define i64 @load_as_scalar
; CHECK-SAME: (ptr [[LOC:%.*]], <2 x i32> [[V:%.*]]) {
; CHECK-NEXT:    store <2 x i32> [[V]], ptr [[LOC]], align 8
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast <2 x i32> [[V]] to i64
; CHECK-NEXT:    ret i64 [[TMP1]]
;
  store <2 x i32> %v, ptr  %loc
  %gep = getelementptr inbounds [4 x float], ptr %loc, i64 0
  %ref = load i64, ptr %gep
  ret i64 %ref
}


define i9 @load_as_scalar_larger(ptr %loc, <4 x i6> %v) {
; CHECK-LABEL: define i9 @load_as_scalar_larger
; CHECK-SAME: (ptr [[LOC:%.*]], <4 x i6> [[V:%.*]]) {
; CHECK-NEXT:    store <4 x i6> [[V]], ptr [[LOC]], align 4
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast <4 x i6> [[V]] to i24
; CHECK-NEXT:    [[TMP2:%.*]] = trunc i24 [[TMP1]] to i16
; CHECK-NEXT:    [[TMP3:%.*]] = trunc i16 [[TMP2]] to i9
; CHECK-NEXT:    ret i9 [[TMP3]]
;
  store <4 x i6> %v, ptr  %loc
  %gep = getelementptr i9, ptr %loc, i64 0
  %ref = load i9, ptr %gep
  ret i9 %ref
}


define i4 @load_as_scalar_smaller(ptr %loc, <4 x i6> %v) {
; CHECK-LABEL: define i4 @load_as_scalar_smaller
; CHECK-SAME: (ptr [[LOC:%.*]], <4 x i6> [[V:%.*]]) {
; CHECK-NEXT:    store <4 x i6> [[V]], ptr [[LOC]], align 4
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast <4 x i6> [[V]] to i24
; CHECK-NEXT:    [[TMP2:%.*]] = trunc i24 [[TMP1]] to i8
; CHECK-NEXT:    [[TMP3:%.*]] = trunc i8 [[TMP2]] to i4
; CHECK-NEXT:    ret i4 [[TMP3]]
;
  store <4 x i6> %v, ptr  %loc
  %gep = getelementptr i4, ptr %loc, i64 0
  %ref = load i4, ptr %gep
  ret i4 %ref
}


define i32 @load_vec_same_type(ptr %loc, <4 x i32> %v) {
; CHECK-LABEL: define i32 @load_vec_same_type
; CHECK-SAME: (ptr [[LOC:%.*]], <4 x i32> [[V:%.*]]) {
; CHECK-NEXT:  entry:
; CHECK-NEXT:    store <4 x i32> [[V]], ptr [[LOC]], align 16
; CHECK-NEXT:    [[R:%.*]] = extractelement <4 x i32> [[V]], i32 1
; CHECK-NEXT:    ret i32 [[R]]
;
  entry:
  store <4 x i32> %v, ptr  %loc
  %gep = getelementptr inbounds [4 x i32], ptr %loc, i64 0
  %ref = load <4 x i32>, ptr %gep
  %r = extractelement <4 x i32> %ref, i32 1
  ret i32 %r
}

define i64 @load_vec_same_size_different_type1(ptr %loc, <4 x i32> %v) {
; CHECK-LABEL: define i64 @load_vec_same_size_different_type1
; CHECK-SAME: (ptr [[LOC:%.*]], <4 x i32> [[V:%.*]]) {
; CHECK-NEXT:  entry:
; CHECK-NEXT:    store <4 x i32> [[V]], ptr [[LOC]], align 16
; CHECK-NEXT:    [[TMP0:%.*]] = bitcast <4 x i32> [[V]] to i128
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast i128 [[TMP0]] to <2 x i64>
; CHECK-NEXT:    [[R:%.*]] = extractelement <2 x i64> [[TMP1]], i32 1
; CHECK-NEXT:    ret i64 [[R]]
;
  entry:
  store <4 x i32> %v, ptr  %loc
  %gep = getelementptr inbounds [4 x i32], ptr %loc, i64 0
  %ref = load <2 x i64>, ptr %gep
  %r = extractelement <2 x i64> %ref, i32 1
  ret i64 %r
}

define double @load_vec_same_size_different_type2(ptr %loc, <4 x i32> %v) {
; CHECK-LABEL: define double @load_vec_same_size_different_type2
; CHECK-SAME: (ptr [[LOC:%.*]], <4 x i32> [[V:%.*]]) {
; CHECK-NEXT:  entry:
; CHECK-NEXT:    store <4 x i32> [[V]], ptr [[LOC]], align 16
; CHECK-NEXT:    [[TMP0:%.*]] = bitcast <4 x i32> [[V]] to i128
; CHECK-NEXT:    [[TMP1:%.*]] = bitcast i128 [[TMP0]] to <2 x double>
; CHECK-NEXT:    [[R:%.*]] = extractelement <2 x double> [[TMP1]], i32 1
; CHECK-NEXT:    ret double [[R]]
;
  entry:
  store <4 x i32> %v, ptr  %loc
  %gep = getelementptr inbounds [4 x i32], ptr %loc, i64 0
  %ref = load <2 x double>, ptr %gep
  %r = extractelement <2 x double> %ref, i32 1
  ret double %r
}

define i32 @load_subvector_same_type(ptr %loc, <4 x i32> %v) {
; CHECK-LABEL: define i32 @load_subvector_same_type
; CHECK-SAME: (ptr [[LOC:%.*]], <4 x i32> [[V:%.*]]) {
; CHECK-NEXT:  entry:
; CHECK-NEXT:    store <4 x i32> [[V]], ptr [[LOC]], align 16
; CHECK-NEXT:    [[TMP0:%.*]] = bitcast <4 x i32> [[V]] to i128
; CHECK-NEXT:    [[TMP1:%.*]] = trunc i128 [[TMP0]] to i64
; CHECK-NEXT:    [[TMP2:%.*]] = bitcast i64 [[TMP1]] to <2 x i32>
; CHECK-NEXT:    [[R:%.*]] = extractelement <2 x i32> [[TMP2]], i32 1
; CHECK-NEXT:    ret i32 [[R]]
;
  entry:
  store <4 x i32> %v, ptr  %loc
  %gep = getelementptr inbounds [4 x i32], ptr %loc, i64 0
  %ref = load <2 x i32>, ptr %gep
  %r = extractelement <2 x i32> %ref, i32 1
  ret i32 %r
}

define i64 @load_subvector_different_type(ptr %loc, <8 x i32> %v) {
; CHECK-LABEL: define i64 @load_subvector_different_type
; CHECK-SAME: (ptr [[LOC:%.*]], <8 x i32> [[V:%.*]]) {
; CHECK-NEXT:  entry:
; CHECK-NEXT:    store <8 x i32> [[V]], ptr [[LOC]], align 32
; CHECK-NEXT:    [[TMP0:%.*]] = bitcast <8 x i32> [[V]] to i256
; CHECK-NEXT:    [[TMP1:%.*]] = trunc i256 [[TMP0]] to i128
; CHECK-NEXT:    [[TMP2:%.*]] = bitcast i128 [[TMP1]] to <2 x i64>
; CHECK-NEXT:    [[R:%.*]] = extractelement <2 x i64> [[TMP2]], i32 1
; CHECK-NEXT:    ret i64 [[R]]
;
  entry:
  store <8 x i32> %v, ptr  %loc
  %gep = getelementptr inbounds [4 x i32], ptr %loc, i64 0
  %ref = load <2 x i64>, ptr %gep
  %r = extractelement <2 x i64> %ref, i32 1
  ret i64 %r
}

define i16 @load_subvector_different_type2(ptr %loc, <8 x i32> %v) {
; CHECK-LABEL: define i16 @load_subvector_different_type2
; CHECK-SAME: (ptr [[LOC:%.*]], <8 x i32> [[V:%.*]]) {
; CHECK-NEXT:  entry:
; CHECK-NEXT:    store <8 x i32> [[V]], ptr [[LOC]], align 32
; CHECK-NEXT:    [[TMP0:%.*]] = bitcast <8 x i32> [[V]] to i256
; CHECK-NEXT:    [[TMP1:%.*]] = trunc i256 [[TMP0]] to i32
; CHECK-NEXT:    [[TMP2:%.*]] = bitcast i32 [[TMP1]] to <2 x i16>
; CHECK-NEXT:    [[R:%.*]] = extractelement <2 x i16> [[TMP2]], i32 1
; CHECK-NEXT:    ret i16 [[R]]
;
  entry:
  store <8 x i32> %v, ptr  %loc
  %gep = getelementptr [2 x i16], ptr %loc, i64 0
  %ref = load <2 x i16>, ptr %gep
  %r = extractelement <2 x i16> %ref, i32 1
  ret i16 %r
}

define i4 @load_subvector_different_type3(ptr %loc, <8 x i8> %v) {
; CHECK-LABEL: define i4 @load_subvector_different_type3
; CHECK-SAME: (ptr [[LOC:%.*]], <8 x i8> [[V:%.*]]) {
; CHECK-NEXT:  entry:
; CHECK-NEXT:    store <8 x i8> [[V]], ptr [[LOC]], align 8
; CHECK-NEXT:    [[TMP0:%.*]] = bitcast <8 x i8> [[V]] to i64
; CHECK-NEXT:    [[TMP1:%.*]] = trunc i64 [[TMP0]] to i16
; CHECK-NEXT:    [[TMP2:%.*]] = trunc i16 [[TMP1]] to i12
; CHECK-NEXT:    [[TMP3:%.*]] = bitcast i12 [[TMP2]] to <3 x i4>
; CHECK-NEXT:    [[R:%.*]] = extractelement <3 x i4> [[TMP3]], i32 1
; CHECK-NEXT:    ret i4 [[R]]
;
  entry:
  store <8 x i8> %v, ptr  %loc
  %gep = getelementptr [3 x i4], ptr %loc, i64 0
  %ref = load <3 x i4>, ptr %gep
  %r = extractelement <3 x i4> %ref, i32 1
  ret i4 %r
}

define i12 @load_subvector_different_type4(ptr %loc, <8 x i8> %v) {
; CHECK-LABEL: define i12 @load_subvector_different_type4
; CHECK-SAME: (ptr [[LOC:%.*]], <8 x i8> [[V:%.*]]) {
; CHECK-NEXT:  entry:
; CHECK-NEXT:    store <8 x i8> [[V]], ptr [[LOC]], align 8
; CHECK-NEXT:    [[TMP0:%.*]] = bitcast <8 x i8> [[V]] to i64
; CHECK-NEXT:    [[TMP1:%.*]] = trunc i64 [[TMP0]] to i24
; CHECK-NEXT:    [[TMP2:%.*]] = bitcast i24 [[TMP1]] to <2 x i12>
; CHECK-NEXT:    [[R:%.*]] = extractelement <2 x i12> [[TMP2]], i32 1
; CHECK-NEXT:    ret i12 [[R]]
;
  entry:
  store <8 x i8> %v, ptr  %loc
  %gep = getelementptr [2 x i12], ptr %loc, i64 0
  %ref = load <2 x i12>, ptr %gep
  %r = extractelement <2 x i12> %ref, i32 1
  ret i12 %r
}

define i6 @load_subvector_different_type5(ptr %loc, <8 x i8> %v) {
; CHECK-LABEL: define i6 @load_subvector_different_type5
; CHECK-SAME: (ptr [[LOC:%.*]], <8 x i8> [[V:%.*]]) {
; CHECK-NEXT:  entry:
; CHECK-NEXT:    store <8 x i8> [[V]], ptr [[LOC]], align 8
; CHECK-NEXT:    [[TMP0:%.*]] = bitcast <8 x i8> [[V]] to i64
; CHECK-NEXT:    [[TMP1:%.*]] = trunc i64 [[TMP0]] to i16
; CHECK-NEXT:    [[TMP2:%.*]] = trunc i16 [[TMP1]] to i12
; CHECK-NEXT:    [[TMP3:%.*]] = bitcast i12 [[TMP2]] to <2 x i6>
; CHECK-NEXT:    [[R:%.*]] = extractelement <2 x i6> [[TMP3]], i32 1
; CHECK-NEXT:    ret i6 [[R]]
;
  entry:
  store <8 x i8> %v, ptr  %loc
  %gep = getelementptr [2 x i6], ptr %loc, i64 0
  %ref = load <2 x i6>, ptr %gep
  %r = extractelement <2 x i6> %ref, i32 1
  ret i6 %r
}

declare void @f(<4 x i32>)
declare void @f_no_mem(<4 x i32>) memory(none)
