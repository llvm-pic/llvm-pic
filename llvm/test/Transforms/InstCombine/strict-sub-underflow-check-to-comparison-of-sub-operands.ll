; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt < %s -passes=instcombine -S | FileCheck %s

declare void @llvm.assume(i1)
declare i8 @gen8()
declare void @use8(i8)

define i1 @t0(i8 %base, i8 %offset) {
; CHECK-LABEL: @t0(
; CHECK-NEXT:    [[CMP:%.*]] = icmp slt i8 [[OFFSET:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[CMP]])
; CHECK-NEXT:    [[ADJUSTED:%.*]] = sub i8 [[BASE:%.*]], [[OFFSET]]
; CHECK-NEXT:    call void @use8(i8 [[ADJUSTED]])
; CHECK-NEXT:    [[RES:%.*]] = icmp ule i8 [[OFFSET]], [[BASE]]
; CHECK-NEXT:    ret i1 [[RES]]
;
  %cmp = icmp slt i8 %offset, 0
  call void @llvm.assume(i1 %cmp)

  %adjusted = sub i8 %base, %offset
  call void @use8(i8 %adjusted)
  %res = icmp ult i8 %adjusted, %base
  ret i1 %res
}
define i1 @t1(i8 %base, i8 %offset) {
; CHECK-LABEL: @t1(
; CHECK-NEXT:    [[CMP:%.*]] = icmp slt i8 [[OFFSET:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[CMP]])
; CHECK-NEXT:    [[ADJUSTED:%.*]] = sub i8 [[BASE:%.*]], [[OFFSET]]
; CHECK-NEXT:    call void @use8(i8 [[ADJUSTED]])
; CHECK-NEXT:    [[RES:%.*]] = icmp ugt i8 [[OFFSET]], [[BASE]]
; CHECK-NEXT:    ret i1 [[RES]]
;
  %cmp = icmp slt i8 %offset, 0
  call void @llvm.assume(i1 %cmp)

  %adjusted = sub i8 %base, %offset
  call void @use8(i8 %adjusted)
  %res = icmp uge i8 %adjusted, %base
  ret i1 %res
}
define i1 @t2(i8 %offset) {
; CHECK-LABEL: @t2(
; CHECK-NEXT:    [[CMP:%.*]] = icmp slt i8 [[OFFSET:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[CMP]])
; CHECK-NEXT:    [[BASE:%.*]] = call i8 @gen8()
; CHECK-NEXT:    [[ADJUSTED:%.*]] = sub i8 [[BASE]], [[OFFSET]]
; CHECK-NEXT:    call void @use8(i8 [[ADJUSTED]])
; CHECK-NEXT:    [[RES:%.*]] = icmp uge i8 [[BASE]], [[OFFSET]]
; CHECK-NEXT:    ret i1 [[RES]]
;
  %cmp = icmp slt i8 %offset, 0
  call void @llvm.assume(i1 %cmp)

  %base = call i8 @gen8()
  %adjusted = sub i8 %base, %offset
  call void @use8(i8 %adjusted)
  %res = icmp ugt i8 %base, %adjusted
  ret i1 %res
}
define i1 @t3(i8 %offset) {
; CHECK-LABEL: @t3(
; CHECK-NEXT:    [[CMP:%.*]] = icmp slt i8 [[OFFSET:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[CMP]])
; CHECK-NEXT:    [[BASE:%.*]] = call i8 @gen8()
; CHECK-NEXT:    [[ADJUSTED:%.*]] = sub i8 [[BASE]], [[OFFSET]]
; CHECK-NEXT:    call void @use8(i8 [[ADJUSTED]])
; CHECK-NEXT:    [[RES:%.*]] = icmp ult i8 [[BASE]], [[OFFSET]]
; CHECK-NEXT:    ret i1 [[RES]]
;
  %cmp = icmp slt i8 %offset, 0
  call void @llvm.assume(i1 %cmp)

  %base = call i8 @gen8()
  %adjusted = sub i8 %base, %offset
  call void @use8(i8 %adjusted)
  %res = icmp ule i8 %base, %adjusted
  ret i1 %res
}

; Here we don't know that offset is non-zero. Can't fold.
define i1 @n4_maybezero(i8 %base, i8 %offset) {
; CHECK-LABEL: @n4_maybezero(
; CHECK-NEXT:    [[ADJUSTED:%.*]] = sub i8 [[BASE:%.*]], [[OFFSET:%.*]]
; CHECK-NEXT:    call void @use8(i8 [[ADJUSTED]])
; CHECK-NEXT:    [[RES:%.*]] = icmp ult i8 [[ADJUSTED]], [[BASE]]
; CHECK-NEXT:    ret i1 [[RES]]
;
  %adjusted = sub i8 %base, %offset
  call void @use8(i8 %adjusted)
  %res = icmp ult i8 %adjusted, %base
  ret i1 %res
}
; We need to know that about %offset, %base won't do. Can't fold.
define i1 @n5_wrongnonzero(i8 %base, i8 %offset) {
; CHECK-LABEL: @n5_wrongnonzero(
; CHECK-NEXT:    [[CMP:%.*]] = icmp sgt i8 [[BASE:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[CMP]])
; CHECK-NEXT:    [[ADJUSTED:%.*]] = sub i8 [[BASE]], [[OFFSET:%.*]]
; CHECK-NEXT:    call void @use8(i8 [[ADJUSTED]])
; CHECK-NEXT:    [[RES:%.*]] = icmp ult i8 [[ADJUSTED]], [[BASE]]
; CHECK-NEXT:    ret i1 [[RES]]
;
  %cmp = icmp sgt i8 %base, 0
  call void @llvm.assume(i1 %cmp)

  %adjusted = sub i8 %base, %offset
  call void @use8(i8 %adjusted)
  %res = icmp ult i8 %adjusted, %base
  ret i1 %res
}
