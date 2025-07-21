#include "lru_cache.h"

int main(void) {
    LRUCache *lru = init_lru_cache(4);
    printf("\nLRU INFO: \n");
    printf("Capacity: %ld\n", lru->capacity);
    printf("Hash Table: ");
    print_table(lru->hash_table);
    printf("DLL: ");
    print_list_pair(lru->dll);

    if (put(lru, "key1", "value1") != SUCCESS)
        exit(EXIT_FAILURE);
    printf("\nLRU INFO: \n");
    printf("Capacity: %ld\n", lru->capacity);
    printf("Hash Table: ");
    print_table(lru->hash_table);
    printf("DLL: ");
    print_list_pair(lru->dll);

    if (put(lru, "key2", "value2") != SUCCESS)
        exit(EXIT_FAILURE);
    printf("\nLRU INFO: \n");
    printf("Capacity: %ld\n", lru->capacity);
    printf("Hash Table: ");
    print_table(lru->hash_table);
    printf("DLL: ");
    print_list_pair(lru->dll);

    if (put(lru, "key3", "value3") != SUCCESS)
        exit(EXIT_FAILURE);
    printf("\nLRU INFO: \n");
    printf("Capacity: %ld\n", lru->capacity);
    printf("Hash Table: ");
    print_table(lru->hash_table);
    printf("DLL: ");
    print_list_pair(lru->dll);

    if (put(lru, "key4", "value4") != SUCCESS)
        exit(EXIT_FAILURE);
    printf("\nLRU INFO: \n");
    printf("Capacity: %ld\n", lru->capacity);
    printf("Hash Table: ");
    print_table(lru->hash_table);
    printf("DLL: ");
    print_list_pair(lru->dll);
    
    if (get(lru, "key1") < 0)
        exit(EXIT_FAILURE);
    printf("\nLRU INFO: \n");
    printf("Capacity: %ld\n", lru->capacity);
    printf("Hash Table: ");
    print_table(lru->hash_table);
    printf("DLL: ");
    print_list_pair(lru->dll);

    if (get(lru, "key3") < 0)
        exit(EXIT_FAILURE);
    printf("\nLRU INFO: \n");
    printf("Capacity: %ld\n", lru->capacity);
    printf("Hash Table: ");
    print_table(lru->hash_table);
    printf("DLL: ");
    print_list_pair(lru->dll);

    if (put(lru, "key5", "value5") != SUCCESS)
        exit(EXIT_FAILURE);
    printf("\nLRU INFO: \n");
    printf("Capacity: %ld\n", lru->capacity);
    printf("Hash Table: ");
    print_table(lru->hash_table);
    printf("DLL: ");
    print_list_pair(lru->dll);

    if (get(lru, "key4") < 0)
        exit(EXIT_FAILURE);
    printf("\nLRU INFO: \n");
    printf("Capacity: %ld\n", lru->capacity);
    printf("Hash Table: ");
    print_table(lru->hash_table);
    printf("DLL: ");
    print_list_pair(lru->dll);

    free_lru(lru);

    exit(EXIT_SUCCESS);
}
