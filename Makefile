CC := gcc
CFLAGS := -Wall -Wextra -std=c99
LDFLAGS := -lSDL2 -lSDL2_ttf

game: game.o engine.o
	$(CC) $(CFLAGS) -o game game.o engine.o $(LDFLAGS)

game.o: game.c engine.h
	$(CC) $(CFLAGS) -c game.c

engine.o: engine.c engine.h
	$(CC) $(CFLAGS) -c engine.c

.PHONY: clean

clean:
	rm -f game *.o
