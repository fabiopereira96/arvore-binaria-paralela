run: main clean

main: arvore.o main.o
	gcc -std=c99 -Wall -pthread -g main.o arvore.o -o main
	./main

arvore.o: arvore.h arvore.c
	gcc -std=c99 -c -Wall -pthread -g arvore.c

main.o: arvore.o main.c
	gcc -c -Wall -pthread -g main.c

clean: 
	rm -rf *.o