; ModuleID = '/home/indicatelove/prog/llvmpic/llvm/lib/Target/PICMid/Examples/main.c'
source_filename = "/home/indicatelove/prog/llvmpic/llvm/lib/Target/PICMid/Examples/main.c"
target datalayout = "e-m:e-p:8:8-i1:8-i8:8-i64:8-n8:16-S0"
target triple = "picmid"

; Function Attrs: noinline nounwind optnone
define dso_local i8 @foo(i8 noundef %baa) #0 {
entry:
  %baa.addr = alloca i8, align 1
  store i8 %baa, ptr %baa.addr, align 1
  %0 = load i8, ptr %baa.addr, align 1
  %inc = add nsw i8 %0, 1
  store i8 %inc, ptr %baa.addr, align 1
  ret i8 %0
}

; Function Attrs: noinline nounwind optnone
define dso_local void @main() #0 {
entry:
  %i = alloca i8, align 1
  %j = alloca i8, align 1
  %h = alloca i8, align 1
  %k = alloca i8, align 1
  store i8 2, ptr %i, align 1
  store i8 3, ptr %j, align 1
  %0 = load i8, ptr %i, align 1
  %1 = load i8, ptr %j, align 1
  %add = add nsw i8 %0, %1
  store i8 %add, ptr %h, align 1
  %2 = load i8, ptr %i, align 1
  %3 = load i8, ptr %j, align 1
  %mul = mul nsw i8 %2, %3
  store i8 %mul, ptr %h, align 1
  %4 = load i8, ptr %i, align 1
  %5 = load i8, ptr %j, align 1
  %sub = sub nsw i8 %4, %5
  store i8 %sub, ptr %h, align 1
  %6 = load i8, ptr %i, align 1
  %7 = load i8, ptr %j, align 1
  %div = sdiv i8 %6, %7
  store i8 %div, ptr %h, align 1
  %8 = load i8, ptr %i, align 1
  %9 = load i8, ptr %j, align 1
  %or = or i8 %8, %9
  store i8 %or, ptr %h, align 1
  %10 = load i8, ptr %i, align 1
  %11 = load i8, ptr %j, align 1
  %and = and i8 %10, %11
  store i8 %and, ptr %h, align 1
  %12 = load i8, ptr %i, align 1
  %13 = load i8, ptr %j, align 1
  %xor = xor i8 %12, %13
  store i8 %xor, ptr %h, align 1
  %14 = load i8, ptr %i, align 1
  %15 = load i8, ptr %j, align 1
  %shr = ashr i8 %14, %15
  store i8 %shr, ptr %h, align 1
  %16 = load i8, ptr %i, align 1
  %17 = load i8, ptr %j, align 1
  %shl = shl i8 %16, %17
  store i8 %shl, ptr %h, align 1
  %18 = load i8, ptr %i, align 1
  %tobool = icmp ne i8 %18, 0
  br i1 %tobool, label %lor.end, label %lor.rhs

lor.rhs:                                          ; preds = %entry
  %19 = load i8, ptr %j, align 1
  %tobool1 = icmp ne i8 %19, 0
  br label %lor.end

lor.end:                                          ; preds = %lor.rhs, %entry
  %20 = phi i1 [ true, %entry ], [ %tobool1, %lor.rhs ]
  %lor.ext = zext i1 %20 to i8
  store i8 %lor.ext, ptr %h, align 1
  %21 = load i8, ptr %i, align 1
  %tobool2 = icmp ne i8 %21, 0
  br i1 %tobool2, label %land.rhs, label %land.end

land.rhs:                                         ; preds = %lor.end
  %22 = load i8, ptr %j, align 1
  %tobool3 = icmp ne i8 %22, 0
  br label %land.end

land.end:                                         ; preds = %land.rhs, %lor.end
  %23 = phi i1 [ false, %lor.end ], [ %tobool3, %land.rhs ]
  %land.ext = zext i1 %23 to i8
  store i8 %land.ext, ptr %h, align 1
  %24 = load i8, ptr %i, align 1
  %25 = load i8, ptr %j, align 1
  %rem = srem i8 %24, %25
  store i8 %rem, ptr %h, align 1
  store i8 0, ptr %k, align 1
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %land.end
  %26 = load i8, ptr %k, align 1
  %cmp = icmp slt i8 %26, 10
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %27 = load i8, ptr %k, align 1
  %28 = load i8, ptr %i, align 1
  %add4 = add nsw i8 %28, %27
  store i8 %add4, ptr %i, align 1
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %29 = load i8, ptr %k, align 1
  %inc = add nsw i8 %29, 1
  store i8 %inc, ptr %k, align 1
  br label %for.cond, !llvm.loop !3

for.end:                                          ; preds = %for.cond
  %30 = load i8, ptr %i, align 1
  %cmp5 = icmp slt i8 %30, 6
  br i1 %cmp5, label %if.then, label %if.else

if.then:                                          ; preds = %for.end
  store i8 3, ptr %h, align 1
  br label %if.end

if.else:                                          ; preds = %for.end
  store i8 4, ptr %h, align 1
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %31 = load i8, ptr %h, align 1
  %call = call i8 @foo(i8 noundef %31)
  store i8 %call, ptr %h, align 1
  ret void
}

attributes #0 = { noinline nounwind optnone "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{i32 7, !"frame-pointer", i32 2}
!2 = !{!"clang version 17.0.6 (git@github.com:llvm-pic/llvm-pic.git d3395377dbe8bdb1412b1a1412fee02f95db4320)"}
!3 = distinct !{!3, !4}
!4 = !{!"llvm.loop.mustprogress"}
