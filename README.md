
how to build with test.c:

cd assgn1/sign-analysis
export PATH=~/code/compilers/llvm-project/build/bin:$PATH
cmake --build build
clang -S -emit-llvm test/test.c -o test/test.ll
mlir-translate --import-llvm test/test.ll -o test/test.raw.mlir
mlir-opt --mem2reg test/test.raw.mlir -o test/test.mlir
PLUGIN=build/SignAnalysis.dylib ./run.sh test/test.mlir


how to build with sqlite.c:

cd assgn1/sign-analysis
export PATH=~/code/compilers/llvm-project/build/bin:$PATH
cmake --build build
clang -isysroot $(xcrun --show-sdk-path) -S -emit-llvm sqlite3.c -o test/sqlite.ll
mlir-translate --import-llvm test/sqlite.ll -o test/sqlite.raw.mlir
mlir-opt --mem2reg test/sqlite.raw.mlir -o test/sqlite.mlir
PLUGIN=build/SignAnalysis.dylib ./run.sh test/sqlite.mlir