CC=gcc
CFLAGS=-D DEBUG -D TESTS -Wall -Wextra -Werror -pedantic -lm

all: test_hash

test_hash: hash.c test_hash.c
	$(CC) $(CFLAGS) hash.c test_hash.c -g -o test_hash

clean:
	rm -rf test_hash 
