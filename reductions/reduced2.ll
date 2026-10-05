target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx14.0.0"

define i32 @allocateBtreePage() {
  %1 = mul i32 1, 4
  ret i32 %1
}
