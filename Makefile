chell : main.o chell.o
	gcc -g -Wall -Werror -o chell main.o chell.o

main.o: main.c 
	gcc -g -Wall -Werror -c main.c 

chell.o: src/chell.c 
	gcc -g -Wall -Werror -c src/chell.c

clean:
	rm *.o chell