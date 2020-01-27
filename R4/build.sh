#!/bin/bash
rm -rf ./llvm-7.0.0
rm -rf ./CPU2006
rm -rf ./build
wget http://releases.llvm.org/7.0.0/clang+llvm-7.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz
tar -xvf clang+llvm-7.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz
mv clang+llvm-7.0.0-x86_64-linux-gnu-ubuntu-16.04 llvm-7.0.0
rm -f clang+llvm-7.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz
tar -xvf CPU2006.tar.xz
tar -xvf build.tar.xz
#rm -rf ./build
#mkdir -p ${R4PATH}/build
#cd ${R4PATH}/build
#cmake ../SVF7.0
#make
cd ${R4PATH}/AsmRewriter
make



