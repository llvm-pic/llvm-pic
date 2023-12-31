; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt -verify-loop-info -passes=irce -S < %s | FileCheck %s
; RUN: opt -verify-loop-info -passes='require<branch-prob>,irce' -S < %s | FileCheck %s

define void @multiple_access_no_preloop(
; CHECK-LABEL: @multiple_access_no_preloop(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    [[LEN_A:%.*]] = load i32, ptr [[A_LEN_PTR:%.*]], align 4, !range [[RNG0:![0-9]+]]
; CHECK-NEXT:    [[LEN_B:%.*]] = load i32, ptr [[B_LEN_PTR:%.*]], align 4, !range [[RNG0]]
; CHECK-NEXT:    [[FIRST_ITR_CHECK:%.*]] = icmp sgt i32 [[N:%.*]], 0
; CHECK-NEXT:    br i1 [[FIRST_ITR_CHECK]], label [[LOOP_PREHEADER:%.*]], label [[EXIT:%.*]]
; CHECK:       loop.preheader:
; CHECK-NEXT:    [[SMIN:%.*]] = call i32 @llvm.smin.i32(i32 [[LEN_B]], i32 [[LEN_A]])
; CHECK-NEXT:    [[SMIN1:%.*]] = call i32 @llvm.smin.i32(i32 [[SMIN]], i32 [[N]])
; CHECK-NEXT:    [[EXIT_MAINLOOP_AT:%.*]] = call i32 @llvm.smax.i32(i32 [[SMIN1]], i32 0)
; CHECK-NEXT:    [[TMP0:%.*]] = icmp slt i32 0, [[EXIT_MAINLOOP_AT]]
; CHECK-NEXT:    br i1 [[TMP0]], label [[LOOP_PREHEADER2:%.*]], label [[MAIN_PSEUDO_EXIT:%.*]]
; CHECK:       loop.preheader2:
; CHECK-NEXT:    br label [[LOOP:%.*]]
; CHECK:       loop:
; CHECK-NEXT:    [[IDX:%.*]] = phi i32 [ [[IDX_NEXT:%.*]], [[IN_BOUNDS_B:%.*]] ], [ 0, [[LOOP_PREHEADER2]] ]
; CHECK-NEXT:    [[IDX_NEXT]] = add nsw i32 [[IDX]], 1
; CHECK-NEXT:    [[ABC_A:%.*]] = icmp slt i32 [[IDX]], [[LEN_A]]
; CHECK-NEXT:    br i1 true, label [[IN_BOUNDS_A:%.*]], label [[OUT_OF_BOUNDS_LOOPEXIT3:%.*]], !prof [[PROF1:![0-9]+]]
; CHECK:       in.bounds.a:
; CHECK-NEXT:    [[ADDR_A:%.*]] = getelementptr i32, ptr [[ARR_A:%.*]], i32 [[IDX]]
; CHECK-NEXT:    store i32 0, ptr [[ADDR_A]], align 4
; CHECK-NEXT:    [[ABC_B:%.*]] = icmp slt i32 [[IDX]], [[LEN_B]]
; CHECK-NEXT:    br i1 true, label [[IN_BOUNDS_B]], label [[OUT_OF_BOUNDS_LOOPEXIT3]], !prof [[PROF1]]
; CHECK:       in.bounds.b:
; CHECK-NEXT:    [[ADDR_B:%.*]] = getelementptr i32, ptr [[ARR_B:%.*]], i32 [[IDX]]
; CHECK-NEXT:    store i32 -1, ptr [[ADDR_B]], align 4
; CHECK-NEXT:    [[NEXT:%.*]] = icmp slt i32 [[IDX_NEXT]], [[N]]
; CHECK-NEXT:    [[TMP1:%.*]] = icmp slt i32 [[IDX_NEXT]], [[EXIT_MAINLOOP_AT]]
; CHECK-NEXT:    br i1 [[TMP1]], label [[LOOP]], label [[MAIN_EXIT_SELECTOR:%.*]]
; CHECK:       main.exit.selector:
; CHECK-NEXT:    [[IDX_NEXT_LCSSA:%.*]] = phi i32 [ [[IDX_NEXT]], [[IN_BOUNDS_B]] ]
; CHECK-NEXT:    [[TMP2:%.*]] = icmp slt i32 [[IDX_NEXT_LCSSA]], [[N]]
; CHECK-NEXT:    br i1 [[TMP2]], label [[MAIN_PSEUDO_EXIT]], label [[EXIT_LOOPEXIT:%.*]]
; CHECK:       main.pseudo.exit:
; CHECK-NEXT:    [[IDX_COPY:%.*]] = phi i32 [ 0, [[LOOP_PREHEADER]] ], [ [[IDX_NEXT_LCSSA]], [[MAIN_EXIT_SELECTOR]] ]
; CHECK-NEXT:    [[INDVAR_END:%.*]] = phi i32 [ 0, [[LOOP_PREHEADER]] ], [ [[IDX_NEXT_LCSSA]], [[MAIN_EXIT_SELECTOR]] ]
; CHECK-NEXT:    br label [[POSTLOOP:%.*]]
; CHECK:       out.of.bounds.loopexit:
; CHECK-NEXT:    br label [[OUT_OF_BOUNDS:%.*]]
; CHECK:       out.of.bounds.loopexit3:
; CHECK-NEXT:    br label [[OUT_OF_BOUNDS]]
; CHECK:       out.of.bounds:
; CHECK-NEXT:    ret void
; CHECK:       exit.loopexit.loopexit:
; CHECK-NEXT:    br label [[EXIT_LOOPEXIT]]
; CHECK:       exit.loopexit:
; CHECK-NEXT:    br label [[EXIT]]
; CHECK:       exit:
; CHECK-NEXT:    ret void
; CHECK:       postloop:
; CHECK-NEXT:    br label [[LOOP_POSTLOOP:%.*]]
; CHECK:       loop.postloop:
; CHECK-NEXT:    [[IDX_POSTLOOP:%.*]] = phi i32 [ [[IDX_NEXT_POSTLOOP:%.*]], [[IN_BOUNDS_B_POSTLOOP:%.*]] ], [ [[IDX_COPY]], [[POSTLOOP]] ]
; CHECK-NEXT:    [[IDX_NEXT_POSTLOOP]] = add i32 [[IDX_POSTLOOP]], 1
; CHECK-NEXT:    [[ABC_A_POSTLOOP:%.*]] = icmp slt i32 [[IDX_POSTLOOP]], [[LEN_A]]
; CHECK-NEXT:    br i1 [[ABC_A_POSTLOOP]], label [[IN_BOUNDS_A_POSTLOOP:%.*]], label [[OUT_OF_BOUNDS_LOOPEXIT:%.*]], !prof [[PROF1]]
; CHECK:       in.bounds.a.postloop:
; CHECK-NEXT:    [[ADDR_A_POSTLOOP:%.*]] = getelementptr i32, ptr [[ARR_A]], i32 [[IDX_POSTLOOP]]
; CHECK-NEXT:    store i32 0, ptr [[ADDR_A_POSTLOOP]], align 4
; CHECK-NEXT:    [[ABC_B_POSTLOOP:%.*]] = icmp slt i32 [[IDX_POSTLOOP]], [[LEN_B]]
; CHECK-NEXT:    br i1 [[ABC_B_POSTLOOP]], label [[IN_BOUNDS_B_POSTLOOP]], label [[OUT_OF_BOUNDS_LOOPEXIT]], !prof [[PROF1]]
; CHECK:       in.bounds.b.postloop:
; CHECK-NEXT:    [[ADDR_B_POSTLOOP:%.*]] = getelementptr i32, ptr [[ARR_B]], i32 [[IDX_POSTLOOP]]
; CHECK-NEXT:    store i32 -1, ptr [[ADDR_B_POSTLOOP]], align 4
; CHECK-NEXT:    [[NEXT_POSTLOOP:%.*]] = icmp slt i32 [[IDX_NEXT_POSTLOOP]], [[N]]
; CHECK-NEXT:    br i1 [[NEXT_POSTLOOP]], label [[LOOP_POSTLOOP]], label [[EXIT_LOOPEXIT_LOOPEXIT:%.*]], !llvm.loop [[LOOP2:![0-9]+]], !irce.loop.clone [[META7:![0-9]+]]
;
  ptr %arr_a, ptr %a_len_ptr, ptr %arr_b, ptr %b_len_ptr, i32 %n) {

  entry:
  %len.a = load i32, ptr %a_len_ptr, !range !0
  %len.b = load i32, ptr %b_len_ptr, !range !0
  %first.itr.check = icmp sgt i32 %n, 0
  br i1 %first.itr.check, label %loop, label %exit

  loop:
  %idx = phi i32 [ 0, %entry ] , [ %idx.next, %in.bounds.b ]
  %idx.next = add i32 %idx, 1
  %abc.a = icmp slt i32 %idx, %len.a
  br i1 %abc.a, label %in.bounds.a, label %out.of.bounds, !prof !1

  in.bounds.a:
  %addr.a = getelementptr i32, ptr %arr_a, i32 %idx
  store i32 0, ptr %addr.a
  %abc.b = icmp slt i32 %idx, %len.b
  br i1 %abc.b, label %in.bounds.b, label %out.of.bounds, !prof !1

  in.bounds.b:
  %addr.b = getelementptr i32, ptr %arr_b, i32 %idx
  store i32 -1, ptr %addr.b
  %next = icmp slt i32 %idx.next, %n
  br i1 %next, label %loop, label %exit

  out.of.bounds:
  ret void

  exit:
  ret void
}

!0 = !{i32 0, i32 2147483647}
!1 = !{!"branch_weights", i32 128, i32 4}
