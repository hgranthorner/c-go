LIB=-I include -L lib -l SDL2-2.0.0

main: main.o
	gcc build/main.o -o c-go $(LIB)

main.o:
	gcc -c src/main.c -o build/main.o -I include -L lib -l SDL2-2.0.0
