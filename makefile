compiler: main.o
	g++ main.o -o compiler

main.o: main.cpp
	g++ -c main.cpp

run:
	clear && ./compiler

clean:
	rm *.o compiler