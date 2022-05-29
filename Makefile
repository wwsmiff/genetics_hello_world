
main: main.o
	g++ main.o -o main

main.o: main.cpp utils.h
	g++ -std=c++17 -Wall -c main.cpp

.PHONY: clean
clean:
	rm -f *.o main output

.PHONY: run
run:
	./main > output
