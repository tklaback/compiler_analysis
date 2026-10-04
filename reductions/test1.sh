#!/bin/bash
# Interestingness test: the analysis proves some value is zerominus.
#
# llvm-reduce hands this script a candidate .ll and keeps the reduction only
# when the script exits 0.
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

# llvm-reduce may run this from any directory, so resolve everything up front.
HERE=$(cd "$(dirname "$0")" && pwd)
ROOT=$(dirname "$HERE")
PLUGIN="$ROOT/build/SignAnalysis.dylib"
[ -f "$PLUGIN" ] || PLUGIN="$ROOT/build/SignAnalysis.so"

LLVM_BIN="${LLVM_BIN:-$HOME/code/compilers/llvm-project/build/bin}"
if [ -x "$LLVM_BIN/mlir-opt" ]; then
  PATH="$LLVM_BIN:$PATH"
  export PATH
fi

# Unique temps: llvm-reduce runs many candidates, possibly in parallel.
raw=$(mktemp -t reduce1raw)
promoted=$(mktemp -t reduce1m2r)
trap 'rm -f "$raw" "$promoted"' EXIT

# A reduced candidate is often invalid IR; that just means "not interesting".
mlir-translate --import-llvm "$FILE" -o "$raw" 2>/dev/null || exit 1
mlir-opt --mem2reg "$raw" -o "$promoted" 2>/dev/null || exit 1

# The annotated listing goes to stderr, the unchanged IR to stdout.
mlir-opt --load-pass-plugin="$PLUGIN" \
         --pass-pipeline='builtin.module(sign-analysis)' \
         "$promoted" 2>&1 1>/dev/null |
  grep -q 'is zerominus'
