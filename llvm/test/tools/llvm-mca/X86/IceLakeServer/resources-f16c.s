# NOTE: Assertions have been autogenerated by utils/update_mca_test_checks.py
# RUN: llvm-mca -mtriple=x86_64-unknown-unknown -mcpu=icelake-server -instruction-tables < %s | FileCheck %s

vcvtph2ps   %xmm0, %xmm2
vcvtph2ps   (%rax), %xmm2

vcvtph2ps   %xmm0, %ymm2
vcvtph2ps   (%rax), %ymm2

vcvtps2ph   $0, %xmm0, %xmm2
vcvtps2ph   $0, %xmm0, (%rax)

vcvtps2ph   $0, %ymm0, %xmm2
vcvtps2ph   $0, %ymm0, (%rax)

# CHECK:      Instruction Info:
# CHECK-NEXT: [1]: #uOps
# CHECK-NEXT: [2]: Latency
# CHECK-NEXT: [3]: RThroughput
# CHECK-NEXT: [4]: MayLoad
# CHECK-NEXT: [5]: MayStore
# CHECK-NEXT: [6]: HasSideEffects (U)

# CHECK:      [1]    [2]    [3]    [4]    [5]    [6]    Instructions:
# CHECK-NEXT:  2      5     1.00                        vcvtph2ps	%xmm0, %xmm2
# CHECK-NEXT:  2      9     0.50    *                   vcvtph2ps	(%rax), %xmm2
# CHECK-NEXT:  2      7     1.00                        vcvtph2ps	%xmm0, %ymm2
# CHECK-NEXT:  2      10    0.50    *                   vcvtph2ps	(%rax), %ymm2
# CHECK-NEXT:  2      5     1.00                        vcvtps2ph	$0, %xmm0, %xmm2
# CHECK-NEXT:  4      6     1.00           *            vcvtps2ph	$0, %xmm0, (%rax)
# CHECK-NEXT:  2      7     1.00                        vcvtps2ph	$0, %ymm0, %xmm2
# CHECK-NEXT:  4      8     1.00           *            vcvtps2ph	$0, %ymm0, (%rax)

# CHECK:      Resources:
# CHECK-NEXT: [0]   - ICXDivider
# CHECK-NEXT: [1]   - ICXFPDivider
# CHECK-NEXT: [2]   - ICXPort0
# CHECK-NEXT: [3]   - ICXPort1
# CHECK-NEXT: [4]   - ICXPort2
# CHECK-NEXT: [5]   - ICXPort3
# CHECK-NEXT: [6]   - ICXPort4
# CHECK-NEXT: [7]   - ICXPort5
# CHECK-NEXT: [8]   - ICXPort6
# CHECK-NEXT: [9]   - ICXPort7
# CHECK-NEXT: [10]  - ICXPort8
# CHECK-NEXT: [11]  - ICXPort9

# CHECK:      Resource pressure per iteration:
# CHECK-NEXT: [0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    [10]   [11]
# CHECK-NEXT:  -      -     4.00   4.00   1.00   1.00   1.00   6.00    -     1.00   1.00   1.00

# CHECK:      Resource pressure by instruction:
# CHECK-NEXT: [0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    [10]   [11]   Instructions:
# CHECK-NEXT:  -      -     0.50   0.50    -      -      -     1.00    -      -      -      -     vcvtph2ps	%xmm0, %xmm2
# CHECK-NEXT:  -      -     0.50   0.50   0.50   0.50    -      -      -      -      -      -     vcvtph2ps	(%rax), %xmm2
# CHECK-NEXT:  -      -     0.50   0.50    -      -      -     1.00    -      -      -      -     vcvtph2ps	%xmm0, %ymm2
# CHECK-NEXT:  -      -     0.50   0.50   0.50   0.50    -      -      -      -      -      -     vcvtph2ps	(%rax), %ymm2
# CHECK-NEXT:  -      -     0.50   0.50    -      -      -     1.00    -      -      -      -     vcvtps2ph	$0, %xmm0, %xmm2
# CHECK-NEXT:  -      -     0.50   0.50    -      -     0.50   1.00    -     0.50   0.50   0.50   vcvtps2ph	$0, %xmm0, (%rax)
# CHECK-NEXT:  -      -     0.50   0.50    -      -      -     1.00    -      -      -      -     vcvtps2ph	$0, %ymm0, %xmm2
# CHECK-NEXT:  -      -     0.50   0.50    -      -     0.50   1.00    -     0.50   0.50   0.50   vcvtps2ph	$0, %ymm0, (%rax)
