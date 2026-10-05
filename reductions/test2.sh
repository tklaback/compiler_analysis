#!/bin/bash
# Interestingness test: the analysis proves a multiply is strictly positive.
set -u

if [ "$#" -lt 1 ]; then
  echo "usage: $0 <file.ll>" >&2
  exit 1
fi

FILE="$1"

if [ ! -f "$FILE" ]; then
  echo "error: '$FILE' not found" >&2
  exit 1
fi

HERE=$(cd "$(dirname "$0")" && pwd)
ROOT=$(dirname "$HERE")
PLUGIN="$ROOT/build/SignAnalysis.dylib"
[ -f "$PLUGIN" ] || PLUGIN="$ROOT/build/SignAnalysis.so"

LLVM_BIN="${LLVM_BIN:-$HOME/code/compilers/llvm-project/build/bin}"
if [ -x "$LLVM_BIN/mlir-opt" ]; then
  PATH="$LLVM_BIN:$PATH"
  export PATH
fi

raw=$(mktemp -t reduce2raw)
promoted=$(mktemp -t reduce2m2r)
trap 'rm -f "$raw" "$promoted"' EXIT

mlir-translate --import-llvm "$FILE" -o "$raw" 2>/dev/null || exit 1
mlir-opt --mem2reg "$raw" -o "$promoted" 2>/dev/null || exit 1

mlir-opt --load-pass-plugin="$PLUGIN" \
         --pass-pipeline='builtin.module(sign-analysis)' \
         "$promoted" 2>&1 1>/dev/null |
  grep -q 'llvm\.mul.*is plus'

