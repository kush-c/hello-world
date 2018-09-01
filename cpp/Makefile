main: main.o brace_expand.o
	g++ main.o brace_expand.o -o brace_expand
test: brace_expand_test.o brace_expand.o
	g++ brace_expand_test.o brace_expand.o -o brace_expand_test
	./brace_expand_test
main.o: main.c++ brace_expand.h
	g++ -I . -c main.c++
brace_expand.o: brace_expand.c++ brace_expand.h
	g++ -I . -c brace_expand.c++
brace_expand_test.o: brace_expand_test.c++ brace_expand.h
		g++ -I . -c brace_expand_test.c++
clean:
	rm -rf *.o
	rm brace_expand
	rm brace_expand_test
