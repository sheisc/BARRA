CC          = gcc
#CFLAGS      = -fno-stack-protector -fno-dwarf2-cfi-asm -fno-asynchronous-unwind-tables 
CFLAGS      = -m32
all: 
	#$(CC) -S $(CFLAGS) server.c
	$(CC) -o server.orig $(CFLAGS) server.c
	$(CC) -o client $(CFLAGS) client.c util.c
	$(CC) -o escape_client $(CFLAGS) escape_client.c util.c
	$(CC) -o got_writer $(CFLAGS) got_writer.c util.c gen_pload.c
	$(CC) -o overflow $(CFLAGS) -DNO_STACK_CANARIES overflow.c util.c
	$(CC) -o aslr_overflow $(CFLAGS) -DNO_STACK_CANARIES aslr_overflow.c util.c
	$(CC) -o aslr_canary_overflow $(CFLAGS) aslr_overflow.c util.c
	$(CC) -o brute_force $(CFLAGS) brute_force.c util.c
	make x86

SERVER=server

x86:
	clang -O0 -fno-builtin -g -m32 -c -emit-llvm $(SERVER).c -o $(SERVER).tmp.bc
	opt -mergereturn $(SERVER).tmp.bc -o $(SERVER).bc
	#wpa -ander $(SERVER).bc	
	clang -m32 -S $(SERVER).bc -o $(SERVER).s
	/home/iron/src/R4/AsmRewriter/instr.SS.x86 $(SERVER).s $(SERVER).instr.SS.s	
	clang -m32 /home/iron/src/R4/AsmRewriter/stack.SS.x86.o $(SERVER).instr.SS.s -o $(SERVER).SS
	rm -f retid.txt	
	clang -O0 -fno-builtin -g -m32 -c -emit-llvm $(SERVER).R4.c -o $(SERVER).tmp.bc
	opt -mergereturn $(SERVER).tmp.bc -o $(SERVER).bc
	wpa -ander $(SERVER).bc	> /dev/null
	clang -m32 -S $(SERVER).bc -o $(SERVER).s
	/home/iron/src/R4/AsmRewriter/instr.R4.x86 $(SERVER).s $(SERVER).instr.R4.s $(SERVER).bc.functions.txt retid.txt
	cat retid.txt
	/home/iron/src/R4/AsmRewriter/gen_addr.x86 1 `cat retid.txt` 4 unsw_addr.s
	clang -m32 -c unsw_addr.s -o unsw_addr.o
	clang -m32 -Wl,--export-dynamic -o $(SERVER).R4 $(SERVER).instr.R4.s /home/iron/src/R4/AsmRewriter/stack.R4.x86.o unsw_addr.o  -lpthread	
	#$(CC) -o lpr $(CFLAGS) lpr.c
	gcc -g -m32 client.R4.c util.c gen_pload.c -o client.R4
	gcc -g -m32 client.SS.c util.c gen_pload.c -o client.SS	
clean:
	rm -f *.o retid.txt server.bc.functions.txt client server escape_client got_writer overflow aslr_overflow \
	aslr_canary_overflow brute_force *.s server.orig server.R4 server.SS *.bc client.R4 client.SS

