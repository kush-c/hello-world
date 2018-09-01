all: main.o brace_expand.o
	g++ main.o brace_expand.o -o brace_expand
main.o: main.c++ brace_expand.h
	g++ -I . -c main.c++
brace_expand.o: brace_expand.c++ brace_expand.h
	g++ -I . -c brace_expand.c++
clean:
	rm -rf *.o
	rm target_bin
