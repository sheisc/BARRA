#!/bin/bash
###############################################################
#
#	table1.sh 
#		The time needed might be different in different
#		machines.
#		
###############################################################
printf "The time needed might be different on different machines.\n"
printf "It might be also affected by process scheduling.\n"
printf "But it is clear that it is proportional to the depth of the call stack.\n"
printf "%s\n" "----------------------------------------------------------"
printf "%s\t%s\t\n" "Depth" "Time(us)"
./AsmRewriter/timing

