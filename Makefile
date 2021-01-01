LINKER_FLAGS=-lSDL2 -lSDL2_ttf -lSDL2_gfx
CFLAGS=-Wall -Wextra -Wswitch-enum -pedantic -Wfloat-equal -Wshadow -Wstrict-prototypes

main: main.o render.o structs.o
	gcc $(CFLAGS) build/main.o build/render.o build/structs.o -o c-go $(LINKER_FLAGS)

main.o: src/structs.h
	gcc $(CFLAGS) -c src/main.c -o build/main.o

render.o: src/structs.h
	gcc $(CFLAGS) -c src/render.c -o build/render.o

structs.o: src/structs.h
	gcc $(CFLAGS) -c src/structs.c -o build/structs.o
