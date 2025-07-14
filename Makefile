CC=gcc
CFLAGS=-D DEBUG -D TESTS -Wall -Wextra -Werror -pedantic -lm
TARGET=test_dll

# Default
VALGRIND_TARGET=$(TARGET)

all: $(TARGET) 

test_dll: dll.c 
	$(CC) $(CFLAGS) dll.c -g -o test_dll

test_hash: hash.c test_hash.c
	$(CC) $(CFLAGS) hash.c test_hash.c -g -o test_hash

valgrind: $(VALGRIND_TARGET)
	valgrind -s --leak-check=full --show-leak-kinds=all ./$(VALGRIND_TARGET)

clean:
	rm -rf test_hash test_dll 
