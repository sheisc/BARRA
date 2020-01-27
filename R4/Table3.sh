#!/bin/bash
###############################################################
#
#	table3.sh 
#
###############################################################

DIRS="C C++"
CPUPATH="./CPU2006"
fmtStr="%8s %8s %8s %12s %8s   %s\n"
printf "As SPEC CPU2006 is commercial, two columns (#LOC and Overheads) are omitted.\n"
printf "%s\n" "-----------------------------------------------------------------"
printf "${fmtStr}" "#Text" "#Funs" "#Calls" "ShadowStack" "BarRA" "Benchmark"
printf "%s\n" "-----------------------------------------------------------------"
for d in ${DIRS}
do
	files=`ls ${CPUPATH}/cpu2006.R4/backup/${d}/exe`
	for f in ${files}
	do 		
		textSize=`size ${CPUPATH}/cpu2006.R4/backup/${d}/exe/${f} | sed '1d' | awk '{printf $1" "}' | tr -d 'n'`
		funcCnt=`cat  ${CPUPATH}/cpu2006.R4/backup/${d}/asm/${f}.instr.s | grep cfi_startproc | wc -l`
		callCnt=`cat ${CPUPATH}/cpu2006.R4/backup/${d}/asm/${f}.instr.s | grep $'\t'callq$'\t' | wc -l`
		origSize=`ls -l ${CPUPATH}/cpu2006.R4/backup/${d}/exe/${f} | awk '{print $5}'`
		R4Size=`ls -l ${CPUPATH}/cpu2006.R4/backup/${d}/instr/${f} | awk '{print $5}'`
		SSSize=`ls -l ${CPUPATH}/cpu2006.SS/backup/${d}/instr/${f} | awk '{print $5}'`
		deltaSS=`echo "scale=3; (${SSSize}-${origSize})*100/${origSize}" | bc`	
		perSS=`echo "${deltaSS}+0.005" | awk '{printf ("%.2f\n",$1)}'`	
		deltaR4=`echo "scale=3; (${R4Size}-${origSize})*100/${origSize}" | bc`
		perR4=`echo "${deltaR4}+0.005" | awk '{printf ("%.2f\n",$1)}'`
		shortName=`echo "${f}" | sed 's/_base\.amd64-m64-softbound-nn//'`		
				
		printf "${fmtStr}" "${textSize}" "${funcCnt}" "${callCnt}" "${perSS}%" "${perR4}%" "${shortName}"
	done
done










