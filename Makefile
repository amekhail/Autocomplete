prog: test.o autocomplete.o
	gcc -Wall -std=c11 test.o autocomplete.o -o prog
test.o: test.c
	gcc -Wall -c -std=c11 test.c -o test.o
autocomplete.o: autocomplete.c
	gcc -Wall -c -std=c11 autocomplete.c -o autocomplete.o
debug: test.o autocomplete.o
	gcc -Wall -g -std=c11 test.o autocomplete.o -o prog