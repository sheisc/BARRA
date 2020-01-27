CFLAGS     ?= -O0 
CFLAGS     += -Wall -D_FORTIFY_SOURCE=2 -g 
all:
	make x64
	make x86

x64: instr.cpp instr2.cpp instr3.cpp stack.c fork.cpp randomize.h timing.cpp instr.SS.cpp stack.SS.c gadgets.cpp
	g++ $(CFLAGS) instr.cpp -o instr
	g++ $(CFLAGS) instr2.cpp -o instr2
	gcc -c stack.c -o stack.o
	g++ $(CFLAGS) instr3.cpp -o instr3
	g++ -g -D_GNU_SOURCE -fPIC -shared fork.cpp -o fork.so -ldl	
	g++ gen_addr.cpp -o gen_addr
	g++ timing.cpp -o timing	
	g++ instr.SS.cpp -o instr.SS
	gcc -c stack.SS.c -o stack.SS.o
	g++ -std=c++11 gadgets.cpp -o gadgets

x86: instr.SS.x86.cpp stack.SS.x86.c stack.R4.x86.c instr.R4.x86.cpp gen_addr.x86.cpp fork.x86.cpp randomize.h
	gcc -m32 -c stack.SS.x86.c -o stack.SS.x86.o
	gcc -m32 -c stack.R4.x86.c -o stack.R4.x86.o	
	g++ -m32 instr.SS.x86.cpp -o instr.SS.x86
	g++ -m32 instr.R4.x86.cpp -o instr.R4.x86
	g++ -m32 gen_addr.x86.cpp -o gen_addr.x86
	#g++ -m32 -g -D_GNU_SOURCE -fPIC -shared fork.x86.cpp -o fork.x86.so -ldl

clean_x64:
	rm -rf instr instr2 stack.o instr3 fork.so gen_addr timing *.o *.so instr.SS gadgets

clean_x86:
	rm -rf 	stack.SS.x86.o stack.SS.x86.o instr.SS.x86 instr.R4.x86 gen_addr.x86 fork.x86.so *.o *.so

clean:
	make clean_x86
	make clean_x64


