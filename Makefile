CC=gcc
CFLAGS=-Wall -Wextra -Werror

hash: hash.c
	$(CC) $(CFLAGS) hash.c -o hash

clean:
	rm -rf hash
