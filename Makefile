chell : main.o
	gcc -Wall -Werror -o chell main.o

main.o: main.c
	gcc -c main.c