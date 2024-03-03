; ModuleID = '/home/indicatelove/prog/llvmpic/llvm/lib/Target/PICMid/Examples/main.c'
source_filename = "/home/indicatelove/prog/llvmpic/llvm/lib/Target/PICMid/Examples/main.c"
target datalayout = "E-p:8:8-i:8:8"
target triple = "picmid"

; Function Attrs: noinline nounwind optnone
define dso_local i8 @main() #0 {
entry:
  %retval = alloca i8, align 1
  store i8 0, ptr %retval, align 1
  ret i8 0
}

attributes #0 = { noinline nounwind optnone "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{i32 7, !"frame-pointer", i32 2}
!2 = !{!"clang version 17.0.6 (git@github.com:llvm-pic/llvm-pic.git c8bcc6fc4e3a90631fc0102f6bcbbcfe56180d59)"}
