#!/bin/bash
# Interestingness test: the analysis proves some value is zerominus.
# zerominus here is generated when two control paths converge
# and the value from one path is 0 and from the other it is negative, yielding the LUB of the tweo: zerominus. Expected input file: input3.ll 
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

raw=$(mktemp -t reduce1raw)
promoted=$(mktemp -t reduce1m2r)
trap 'rm -f "$raw" "$promoted"' EXIT

mlir-translate --import-llvm "$FILE" -o "$raw" 2>/dev/null || exit 1
mlir-opt --mem2reg "$raw" -o "$promoted" 2>/dev/null || exit 1

mlir-opt --load-pass-plugin="$PLUGIN" \
         --pass-pipeline='builtin.module(sign-analysis)' \
         "$promoted" 2>&1 1>/dev/null |
  grep -q 'is zerominus'
