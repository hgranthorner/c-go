LINKER_FLAGS = -lSDL2 -lSDL2_ttf -lSDL2_gfx

main: main.o
	gcc build/main.o -o c-go $(LINKER_FLAGS)

main.o: src/structs.h
	gcc -c src/main.c -o build/main.o
