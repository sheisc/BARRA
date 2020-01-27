CC=clang

all: 
	${CC} -g -D_GNU_SOURCE -fPIC -shared hook.c -o hook.so -ldl 
	${CC} -c -emit-llvm main.c -o main.bc 
	${CC} -c -emit-llvm randomize.c -o randomize.bc 	
	${CC} -Wl,--export-dynamic main.bc randomize.bc -o main
clean:
	rm -f *.o *.so main *.bc


