chell : main.o chell.o
	gcc -Wall -Werror -o chell main.o chell.o

main.o: main.c 
	gcc -c main.c 

chell.o: src/chell.c 
	gcc -c src/chell.c

clean:
	rm *.o chell