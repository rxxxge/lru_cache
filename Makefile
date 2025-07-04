CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -lm

all: hash

hash: hash.c
	$(CC) $(CFLAGS) hash.c -g -o hash

test: test.c
	$(CC) $(CFLAGS) test.c -o test

clean:
	rm -rf hash test
