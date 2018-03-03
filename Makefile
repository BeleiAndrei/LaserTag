all: tema2

tema2: tema2.o
	                g++ tema2.o -o tema2

build: tema2.o
	                g++ tema2.o -o tema2

tema2.o: tema2.cc
	                g++ -c -O2 tema2.cc 

clean:
	                rm tema2.o tema2


