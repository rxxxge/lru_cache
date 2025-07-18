CC=gcc
CFLAGS=-D DEBUG -D TESTS -Wall -Wextra -Werror -pedantic -lm
TARGET=test_lru

# Default
VALGRIND_TARGET=$(TARGET)

all: $(TARGET) 

test_lru: test_lru.c lru_cache.c hash.c dll.c
	$(CC) $(CFLAGS) test_lru.c lru_cache.c hash.c dll.c -g -o test_lru

test_dll: test_dll.c dll.c 
	$(CC) $(CFLAGS) dll.c test_dll.c -g -o test_dll

test_hash: hash.c test_hash.c
	$(CC) $(CFLAGS) hash.c test_hash.c -g -o test_hash

valgrind: $(VALGRIND_TARGET)
	valgrind -s --leak-check=full --show-leak-kinds=all ./$(VALGRIND_TARGET)

clean:
	rm -rf test_lru test_hash test_dll 
