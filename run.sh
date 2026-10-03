#!/bin/sh
# Run the analysis over a C or MLIR file.
#
#   ./run.sh input.c      # compiles, promotes allocas to SSA, then analyses
#   ./run.sh input.mlir   # analyses as-is
#
# A .c input is lowered with clang and mem2reg first.  mem2reg matters: the
# analysis tracks facts per SSA value, so while a variable still lives in an
# alloca every use is a load the analysis knows nothing about.
#
# The plugin is SignAnalysis.dylib on macOS and SignAnalysis.so on Linux and
# WSL2, so probe for it rather than hard-coding a suffix.  Set PLUGIN or
# BUILD_DIR to override.
set -eu

BUILD_DIR="${BUILD_DIR:-build}"

if [ -z "${PLUGIN:-}" ]; then
  for candidate in "$BUILD_DIR"/SignAnalysis.so "$BUILD_DIR"/SignAnalysis.dylib; do
    if [ -f "$candidate" ]; then
      PLUGIN="$candidate"
      break
    fi
  done
fi

if [ -z "${PLUGIN:-}" ]; then
  echo "No plugin found in $BUILD_DIR; build it first (see README.md)." >&2
  exit 1
fi

if [ "$#" -lt 1 ]; then
  echo "usage: $0 input.c | input.mlir" >&2
  exit 2
fi

INPUT="$1"
shift

# Build the MLIR beside the plugin rather than next to the source, so repeated
# runs do not litter the test directory.
case "$INPUT" in
*.c)
  base=$(basename "$INPUT" .c)
  ll="$BUILD_DIR/$base.ll"
  raw="$BUILD_DIR/$base.raw.mlir"
  mlir="$BUILD_DIR/$base.mlir"
  clang -S -emit-llvm "$INPUT" -o "$ll"
  mlir-translate --import-llvm "$ll" -o "$raw"
  mlir-opt --mem2reg "$raw" -o "$mlir"
  ;;
*)
  mlir="$INPUT"
  ;;
esac

# stdout is the unchanged IR and stderr is the annotated listing; send the
# listing to this script's stdout so it can be piped or paged.
mlir-opt --load-pass-plugin="$PLUGIN" \
         --pass-pipeline='builtin.module(sign-analysis)' \
         "$mlir" "$@" 2>&1 1>/dev/null
