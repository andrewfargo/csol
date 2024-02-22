CC = gcc
CFLAGS = -std=c2x
DEPS = card.h deck.h
OBJ = card.o deck.o
TESTS_OBJ = tests.o
LIBS = -lncurses

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(OBJ) $(TESTS_OBJ)
	$(CC) $(CFLAGS) $(LIBS) -o csol-tests $^

.PHONY: clean

clean:
	rm *.o
