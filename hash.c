/*
 * hash.c
 * Hash table function implementations
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#include "hash.h"

/*
 * Hash function
 */
Fnv32_t fnv_32a_str(const char *str, Fnv32_t hval) {
    unsigned char *s = (unsigned char *)str;

    while (*s) {
        /* xor the bottom with the current octet */
        hval ^= (Fnv32_t)*s++;
        /* multiply by the 32 bit FNV magic prime mod 2^32 */
        hval *= FNV_32_PRIME;
    }

    return hval;
}

/*
 * UTILITY
 * Get index using key and hash function
 */
int get_index(const char *key, size_t table_size) {
    if (key == NULL) {
        fprintf(stderr, "The key provided is invalid or NULL!\n");
        return IS_NULL;
    }

    Fnv32_t hval = fnv_32a_str(key, FNV1_32A_INIT);
    int index = (hval % table_size + table_size) % table_size;
#ifdef DEBUG
    printf("hash value from '%s': %d, init hval: %d\n", key, hval, FNV1_32A_INIT);
#endif

    return index; 
}

/*
 * UTILITY
 * Update the load factor
 */
int update_load_factor(HashTable *table) {
    if (table == NULL) {
        fprintf(stderr, "Table is not valid!\n");
        return IS_NULL;
    }

    table->load_factor = (float)table->count_entry / (float)table->table_size;
    return SUCCESS;
}

/*
 * UTILITY
 * Mostly for debugging purposes
 */
int print_table(HashTable *table) {
    if (table == NULL) {
        printf("Table is not valid!\n");
        return IS_NULL;
    }

    printf("\n[Index] --- (key, value)\n");
    for (unsigned int i = 0; i < table->table_size; i++) {
        if (table->table[i] != NULL) {
            printf("[%d] --- (%s, %s)\n |\n", i, table->table[i]->key, (char *)table->table[i]->value);
        } else {
            printf("[%d]\n |\n", i);
        }
    }

    return SUCCESS;
}

/*
 * UTILITY
 * Resize the table based on load factor
 */
int resize_on_lf(HashTable *table, bool size_up) {
    if (table == NULL) {
        printf("Table is not valid!\n");
        return IS_NULL;
    }
   
    if (size_up) {
        /*
         * If load factor approaches ALPHA_MAX
         * call resize_table and size up
         */
        if (fabs(ALPHA_MAX - table->load_factor) < 0.1f || (table->load_factor - ALPHA_MAX) >= 0.0f) {
            if (resize_table(table, RESIZE_UP) != SUCCESS) {
                return FAILURE;
            }
        } 
        
    } else {
        /*
         * If load factor approaches ALPHA_MIN
         * call resize_table and size down
         */
        if (fabs(ALPHA_MIN - table->load_factor) < 0.1f || (ALPHA_MIN - table->load_factor) >= 0.0f) {
            if (resize_table(table, size_up) != SUCCESS)
                return FAILURE;
        }
    }

    return SUCCESS;
}

/*
 * Resize the table specifying boolean 
 * "size_up" parameter to increase/decrease size
 * of the table accordingly
 */
int resize_table(HashTable *table, bool size_up) {
    if (table == NULL) {
        printf("Table is not valid!\n");
        return IS_NULL;
    }

    if (size_up) {
        size_t new_size = table->table_size * 2;
        HashEntry **new_table = (HashEntry **)calloc(new_size, sizeof(HashEntry *));
        if (new_table == NULL) {
            printf("Could not allocate new table array!\n");
            return IS_NULL; 
        }

        /*
         * Copy every element from old table array into new one
         */
        for (size_t i = 0; i < table->table_size; i++) {
            if (table->table[i])
                new_table[i] = table->table[i];
        }

        /*
         * Free old table array
         */
        free(table->table);

        /*
         * Assign a new table array
         */
        table->table_size = new_size;
        table->table = new_table;
    } else {
        size_t new_size = table->table_size == 4 ? table->table_size : table->table_size / 2;
        HashEntry **new_table = (HashEntry **)calloc(new_size, sizeof(HashEntry *));
        if (new_table == NULL) {
            printf("Could not allocate new table array!\n");
            return IS_NULL; 
        }

        for (size_t i = 0; i < table->table_size; i++) {
            if (table->table[i]) {
                if (i < new_size) {
                    new_table[i] = table->table[i];
                } else {
                    int index = get_index(table->table[i]->key, new_size);
                    if (index < 0)
                        return FAILURE;
                    new_table[index] = table->table[i];
                } 
            }
        }

        free(table->table);

        table->table_size = new_size;
        table->table = new_table;
    }
    
    return SUCCESS;
}

/*
 * Initialize hash table
 */
HashTable *init_hash_table(size_t table_size) {
    HashTable *hash_table = (HashTable *)calloc(1, sizeof(HashTable));
    if (hash_table == NULL) {
        printf("Could not allocate memory for hash table!\n");
        return NULL;
    }

    hash_table->table_size = table_size;
    hash_table->count_entry = 0;
    hash_table->load_factor = (float)hash_table->count_entry / (float)hash_table->table_size;

    hash_table->table = (HashEntry **)calloc(table_size, sizeof(HashEntry *));
    if (hash_table->table == NULL) {
        printf("Could not allocate hash table array!\n");
        return NULL;
    }

    hash_table->update_lf = update_load_factor;
    
    return hash_table;
}

int search_entry(const char *key, HashTable *table) {
    if (table == NULL) {
        fprintf(stderr, "Table is not valid!\n");
        return IS_NULL;
    }

    if (key == NULL) {
        fprintf(stderr, "The key provided is invalid or NULL!\n");
        return IS_NULL;
    }

    int index = get_index(key, table->table_size);
    if (index < 0)
        return FAILURE;
    int original_index = index;

    do {
        index = (index + 1) % table->table_size;
        if (!table->table[index]) 
            continue;
        if (strcmp(table->table[index]->key, key) == 0) {
#ifdef DEBUG
            printf("Computed index: %d\n", index);
#endif
            return index;
        } 
    } while (index != original_index);
       
    printf("Key does not exist!\n");
    return FAILURE;
}

/*
 * Handle collision by linear probing
 */
int handle_collision(const char *key, void *value, HashTable *table, int index, bool auto_resize) {
    if (table == NULL) {
        printf("Table is not valid!\n");
        return IS_NULL;
    }

    if (key == NULL) {
        fprintf(stderr, "The key provided is invalid or NULL!\n");
        return IS_NULL;
    }

    if (value == NULL) {
        fprintf(stderr, "The value provided is invalid or NULL!\n");
        return IS_NULL;
    }

    while (table->table[index] != NULL) {
        index = (index + 1) % table->table_size;
    }

    if (create_hash_entry(key, value, table, index, auto_resize) != SUCCESS) 
        return FAILURE;
    
    return index;
}

/*
 * Adds entry (key, value) pair to the table array at computed index
 */
int create_hash_entry(const char *key, void *value, HashTable *table, int index, bool auto_resize) {
    if (table == NULL) {
        fprintf(stderr, "Table is not valid!\n");
        return IS_NULL;
    }

    if (key == NULL) {
        fprintf(stderr, "The key provided is invalid or NULL!\n");
        return IS_NULL;
    }

    if (value == NULL) {
        fprintf(stderr, "The value provided is invalid or NULL!\n");
        return IS_NULL;
    }

    HashEntry *entry = (HashEntry *)calloc(1, sizeof(HashEntry));
    if (!entry) {
        printf("Could not allocate hash entry!\n");
        return IS_NULL;
    }

    entry->key = key;
    entry->value = value; 
    table->table[index] = entry;
    table->count_entry++;
#ifdef HASH_DEBUG
    printf("Load factor: %.7f\n", table->load_factor);
    printf("RECEIVED KEY: %s, VALUE: %p in create_hash_entry at index: %d\n", entry->key, entry->value, index);
#endif
    if (table->update_lf(table) != SUCCESS)
        return FAILURE;

    /*
     * Handle resizing automatically, if specified
     */
    if (auto_resize) {
        if (resize_on_lf(table, RESIZE_UP) != SUCCESS)
            return FAILURE;
    }

    return SUCCESS;
}

/*
 * Wrapper function around create_hash_entry to add entry to the table array
 * Returns index on success
 */
int add_hash_entry(const char *key, void *value, HashTable *table) {
    if (table == NULL) {
        printf("Table is not valid!\n");
        return IS_NULL;
    }

    if (key == NULL) {
        fprintf(stderr, "The key provided is invalid or NULL!\n");
        return IS_NULL;
    }

    if (value == NULL) {
        fprintf(stderr, "The value provided is invalid or NULL!\n");
        return IS_NULL;
    }

    bool auto_resize = true;
    int index = get_index(key, table->table_size);
    if (index < 0) {
        return FAILURE;
    }
    /*
     * If entry is empty we can fill it with
     * new (key, value) pair, otherwise look for next
     * empty slot by linear probing
     */
    if (table->table[index] == NULL) {
        if (create_hash_entry(key, value, table, index, auto_resize) != SUCCESS)
            return FAILURE;
    } else {
#ifdef DEBUG
        printf("Load factor: %.7f\n", table->load_factor);
#endif
        /*
         * If key is the same, it is going to change only the value
         */
#ifdef DEBUG
        printf("COMPARING STRINGS\n");
#endif

        int search_index;
        if ((search_index = search_entry(key, table)) >= 0) { 
#ifdef DEBUG
            printf("KEYS ARE THE SAME. REPLACING\n");
#endif
            if (create_hash_entry(key, value, table, search_index, auto_resize) != SUCCESS) {
                return FAILURE;
            }

            return index;
        } 
        /*
         * Handle collisions by linear probing
         */
        index = handle_collision(key, value, table, index, auto_resize);
        if (index < 0)
            return FAILURE;
    }

    return index;
}


/*
 * Removes pair by its key (if it exists)
 * and resizes the table (if specified) based on load factor
 */
int remove_hash_entry(const char *key, HashTable *table, bool auto_resize) {
    if (table == NULL) {
        printf("Table is not valid!\n");
        return IS_NULL;
    }

    if (key == NULL) {
        fprintf(stderr, "The key provided is invalid or NULL!\n");
        return IS_NULL;
    }

    int index = search_entry(key, table);

    if (index < 0) {
        return FAILURE;
    } else {
        free(table->table[index]);
        table->table[index] = NULL;
        table->count_entry--;

        if (table->update_lf(table) != SUCCESS)
            return FAILURE;
    }
    
    /*
     * Handle resizing automatically, if specified
     */
    if (auto_resize) {
        if (resize_on_lf(table, RESIZE_DOWN) != SUCCESS)
            return FAILURE;
    }

    return SUCCESS;
}

/*
 * Free the hash table at the end of program
 */
void free_table(HashTable *table) {
    if (table == NULL) {
        printf("Table is not valid!\n");
        return;
    }

    for (unsigned int i = 0; i < table->table_size; i++) {
        free(table->table[i]);
    }

    free(table->table);
    free(table);
    
    return;
}
