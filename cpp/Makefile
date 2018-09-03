all: main.o brace_expand.o brace_expand_test.o
	g++ main.o brace_expand.o -o brace_expand
	g++ brace_expand_test.o brace_expand.o -o brace_expand_test
main: main.o brace_expand.o
	g++ main.o brace_expand.o -o brace_expand
test: brace_expand_test.o brace_expand.o
	g++ brace_expand_test.o brace_expand.o -o brace_expand_test
	./brace_expand_test
main.o: main.cpp brace_expand.h
	g++ -I . -c main.cpp
brace_expand.o: brace_expand.cpp brace_expand.h
	g++ -I . -c brace_expand.cpp
brace_expand_test.o: brace_expand_test.cpp brace_expand.h
	g++ -I . -c brace_expand_test.cpp
clean:
	rm -rf *.o
	rm brace_expand
	rm brace_expand_test
