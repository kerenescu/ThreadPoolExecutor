build:
	gcc -g -Wall -Wextra -o tema2 main.c lista-generica.c coada.c stiva.c

run:
	./tema2

clean:
	rm -rf tema2