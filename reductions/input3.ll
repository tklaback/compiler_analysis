target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx14.0.0"

; The two operands of the sdiv are the same value, so the analysis concludes
; the quotient is one.  Everything else here is padding for llvm-reduce to
; strip away.
define i32 @selfdiv() {
  %x = add i32 20, 0
  %d = sdiv i32 %x, %x
  ret i32 %d
}

define i32 @padding(i32 %a, i32 %b) {
  %p = mul i32 %a, %b
  %q = sub i32 %a, %b
  %r = add i32 %p, %q
  ret i32 %r
}
