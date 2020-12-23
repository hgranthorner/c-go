LINKER_FLAGS = -lSDL2 -lSDL2_ttf -lSDL2_gfx

main: main.o structs.o
	gcc build/main.o build/structs.o -o c-go $(LINKER_FLAGS)

main.o: src/structs.h
	gcc -c src/main.c -o build/main.o

structs.o: src/structs.h
	gcc -c src/structs.c -o build/structs.o
