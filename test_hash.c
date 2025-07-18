#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "hash.h"


#define RESIZE_AUTOMATICALLY true

int main(void) {
    HashTable *hash_table = init_hash_table(4);
    if (hash_table == NULL) {
        printf("Failed to initialize hash table!\n");
        exit(EXIT_FAILURE);
    }
    
#ifdef DEBUG
    printf("Tried to initialize hash table\n");
    printf("======== Results ======== \n");
    printf("Table size: %ld\n", hash_table->table_size);
    printf("# of entries: %d\n", hash_table->count_entry);
    printf("load factor (alpha): %.3f\n", hash_table->load_factor);
    printf("Table array address: %p\n", (void *)hash_table->table);
#endif

#ifdef DEBUG
    if (print_table(hash_table) != SUCCESS) 
        exit(EXIT_FAILURE);
#endif

    /*
     * TESTS
     */
#ifdef TESTS
    int index;
    
    if ((index = add_hash_entry("key1", "val1", hash_table)) < 0) {
        fprintf(stderr, "TEST 1 FAILED: Couldn't add key, value pair!\n");
        exit(EXIT_FAILURE);
    }
    printf("TEST 1 PASSED\n");
#ifdef DEBUG
    if (print_table(hash_table) != SUCCESS) 
        exit(EXIT_FAILURE);
#endif

    if ((index = add_hash_entry("key2", "val2", hash_table)) < 0) {
        fprintf(stderr, "TEST 2 FAILED: Couldn't add key, value pair!\n");
        exit(EXIT_FAILURE);
    }
    printf("TEST 2 PASSED\n");
#ifdef DEBUG
    if (print_table(hash_table) != SUCCESS) 
        exit(EXIT_FAILURE);
#endif

    if ((index = add_hash_entry("key333", "val3", hash_table)) < 0) {
        fprintf(stderr, "TEST 3 FAILED: Couldn't add key, value pair!\n");
        exit(EXIT_FAILURE);
    }
    printf("TEST 3 PASSED\n");
#ifdef DEBUG
    if (print_table(hash_table) != SUCCESS) 
        exit(EXIT_FAILURE);
#endif
      
    if ((index = add_hash_entry("key4", "val4", hash_table)) < 0) {
        fprintf(stderr, "TEST 4 FAILED: Couldn't add key, value pair!\n");
        exit(EXIT_FAILURE);
    }
    printf("TEST 4 PASSED\n");

#ifdef DEBUG
    if (print_table(hash_table) != SUCCESS) 
        exit(EXIT_FAILURE);
#endif

    if ((index = add_hash_entry("key4", "val15", hash_table)) < 0) {
        fprintf(stderr, "TEST 5 FAILED: Couldn't add key, value pair!\n");
        exit(EXIT_FAILURE);
    }
    printf("TEST 5 PASSED\n");

#ifdef DEBUG
    if (print_table(hash_table) != SUCCESS) 
        exit(EXIT_FAILURE);
#endif
    
    if (remove_hash_entry("unknown_key", hash_table, RESIZE_AUTOMATICALLY) == SUCCESS) {
        fprintf(stderr, "TEST 6 FAILED: Removed key that doesn't exist!\n");
        exit(EXIT_FAILURE);
    }
    printf("TEST 6 PASSED\n");

    if (remove_hash_entry("key333", hash_table, RESIZE_AUTOMATICALLY) != SUCCESS) {
        fprintf(stderr, "TEST 7 FAILED: Did not remove existing key!\n");
        exit(EXIT_FAILURE);
    }
    printf("TEST 7 PASSED\n");

#ifdef DEBUG
    print_table(hash_table);
#endif

    printf("ALL TESTS PASSED!\n");
#endif // TESTS

    free_table(hash_table);

    exit(EXIT_SUCCESS);
}
