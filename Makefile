CC = gcc
CFLAGS = -std=c2x
DEPS = card.h deck.h config.h
OBJ = card.o deck.o render.o klondike.o
TESTS_OBJ = tests.o
LIBS = -lncurses

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJ)
	$(CC) $(CFLAGS) $(LIBS) -o csol $^

test: $(OBJ) $(TESTS_OBJ)
	$(CC) $(CFLAGS) $(LIBS) -o csol-tests $^

.PHONY: clean

clean:
	rm *.o
