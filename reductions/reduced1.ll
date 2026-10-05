target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx14.0.0"

define i32 @proxyBreakConchLock() {
  br i1 false, label %2, label %1

1:                                                ; preds = %0
  br label %2

2:                                                ; preds = %1, %0
  %3 = phi i32 [ 0, %1 ], [ -1, %0 ]
  ret i32 %3
}
