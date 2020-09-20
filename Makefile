run: main clean

main: arvore.o barreira.o main.o
	gcc -std=c99 -Wall -pthread -g main.o arvore.o barreira.o -o main
	./main

arvore.o: arvore.h arvore.c
	gcc -std=c99 -c -Wall -pthread -g arvore.c

main.o: arvore.o main.c
	gcc -c -Wall -pthread -g main.c

barreira.o: barreira.h barreira.c
	gcc -std=c99 -c -Wall -pthread -g barreira.c

clean: 
	rm -rf *.o