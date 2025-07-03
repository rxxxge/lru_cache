CC=gcc
CFLAGS=-Wall -Wextra -Werror -lm

all: hash

hash: hash.c
	$(CC) $(CFLAGS) hash.c -o hash

test: test.c
	$(CC) $(CFLAGS) test.c -o test

clean:
	rm -rf hash test
