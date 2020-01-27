all:
	clang -o main main.c tls.c -lpthread
	#strace ./tls 2> strace.txt
	clang -c -S tls.c -o tls.s -fno-asynchronous-unwind-tables
clean:
	rm -f tls *.o main


