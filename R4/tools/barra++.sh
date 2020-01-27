#!/bin/bash



APP=$1
LIBS=$2


extract-bc ./${APP} -o ./${APP}.bc
llvm-dis ${APP}.bc -o ${APP}.ll
wpa -ander -svfmain ${APP}.bc > /dev/null
clang++ -O2 -c -S ${APP}.wpa.bc -o ${APP}.s	
rm -f retid.txt
${R4PATH}/AsmRewriter/instr2 ${APP}.s ${APP}.instr.s ${APP}.bc.functions.txt retid.txt
clang++ -c ${APP}.instr.s -o ${APP}.o
${R4PATH}/AsmRewriter/gen_addr 1 `cat retid.txt` 8 unsw_addr.s
clang++ -c unsw_addr.s -o unsw_addr.o
clang++ -Wl,--export-dynamic -o ${APP}.R4 ${APP}.o ${R4PATH}/AsmRewriter/stack.o unsw_addr.o  ${LIBS} 2> /dev/null
rm -f ${APP}.s ${APP}.bc.functions.txt retid.txt unsw_addr.o ${APP}.ll ${APP}.o ${APP}.wpa.bc ${APP}.bc
strip ${APP}.R4
echo "${APP}.R4 generated."
 
