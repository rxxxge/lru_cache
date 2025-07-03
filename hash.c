#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/*
 * 32 bit FNV-0 hash type
 */
typedef u_int32_t Fnv32_t;

/*
 * 32 bit magic FNV-1a prime
 */
#define FNV_32_PRIME ((Fnv32_t)0x01000193)

/*
 * Initial hash value for 32 bit FNV function
 */
#define FNV1_32_INIT ((Fnv32_t)0x811c9dc5)
#define FNV1_32A_INIT FNV1_32_INIT

/*
 * Temporary
 */
#define ARR_SIZE 17

/*
 * Define for debug purposes
 */
#define DEBUG

/*
 * Function return codes
 */
#define SUCCESS 0
#define IS_NULL -1
#define ARRAY_IS_NULL -2
#define TABLE_IS_NULL -3
#define ERR_ARRAY_IS_FULL -4
#define KEY_NOT_FOUND -5 

/*
 * Maximum load factor alpha
 */
#define ALPHA_MAX 0.72

/*
 * Type of element in the array
 */
typedef struct HashEntry {
    const char *key;
    char *value;
} HashEntry;

typedef struct HashTable {
    unsigned int table_size;
    unsigned int count_entry;
    float load_factor;
    HashEntry **table;

    /*
     * Function pointer to update load factor
     */
    void (*update_lf)(struct HashTable *);
} HashTable;

/*
 * Hash function
 */
Fnv32_t fnv_32a_str(char *str, Fnv32_t hval) {
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
 * Update the load factor
 */

int resize_table(HashTable *table, bool size_up) {
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
        for (unsigned int i = 0; i < table->table_size; i++) {
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
        table->table_size *= 2;
        table->table = new_table;
    } else {
        /*
         * TODO: Size down
         */
        printf("Unimplemented!\n");
    }
    
    return SUCCESS;
}

void update_load_factor(HashTable *table) {
    table->load_factor = (float)table->count_entry / (float)table->table_size;


    /*
     * Resize array by 2x its size if load factor close to 0.72 or higher
     */
    if (fabs(ALPHA_MAX - table->load_factor) < 0.1f || (table->load_factor - ALPHA_MAX) >= 0.0f) {
        /*
         * Resize up the array
         */ 
        resize_table(table, true);
    }

    /* 
     * TODO:
     * Resize table down if load factor is less than 0.2
     */
    
}

/*
 * Initialize hash table
 */
HashTable *init_hash_table(unsigned int table_size) {
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

int search(char *key, HashTable *table) {
    if (table == NULL) {
        printf("Table is not valid!\n");
        return TABLE_IS_NULL;
    }

    Fnv32_t hval = fnv_32a_str(key, FNV1_32A_INIT);
    int index = (hval % table->table_size + table->table_size) % table->table_size;

#ifdef DEBUG
    printf("Hash value from %s is %d\n", key, hval);
    printf("Computed index: %d\n", index);
#endif

    if (table->table[index] != NULL) {
        return index;
    } 
        
    return KEY_NOT_FOUND;
}

int print_array(HashTable *table) {
    if (table == NULL) {
        printf("Table is not valid!\n");
        return TABLE_IS_NULL;
    }

    printf("[Index] --- (key, value)\n");
    for (unsigned int i = 0; i < table->table_size; i++) {
        if (table->table[i] != NULL) {
            printf("[%d] --- (%s, %s)\n |\n", i, table->table[i]->key, table->table[i]->value);
        } else {
            printf("[%d]\n |\n", i);
        }
    }

    return SUCCESS;
}


/*
 * Handle collision by linear probing
 */
int handle_collision(const char *key, char *value, HashTable *table, int index) {
    if (table == NULL) {
        printf("Table is not valid!\n");
        return TABLE_IS_NULL;
    }

    /*
     * TEMPORARY
     */
    if (table->table_size == table->count_entry)
        return SUCCESS;

    while (table->table[index] != NULL) {
        index = (index + 1) % table->table_size;
    }

    HashEntry *entry = malloc(sizeof(HashEntry));
    entry->key = key;
    entry->value = value;
    table->table[index] = entry;
    table->count_entry++;

    table->update_lf(table);
    
    return SUCCESS;
}

/*
 * Free array at the end of program?
 */
int free_table(HashTable *table) {
    if (table == NULL) {
        printf("Table is not valid!\n");
        return ARRAY_IS_NULL;
    }

    for (unsigned int i = 0; i < table->table_size; i++) {
        free(table->table[i]);
    }

    free(table->table);
    free(table);

    return SUCCESS;
}


int main(void) {
    char *strings[38] = {
        "a",
        "b",
        "d",
        "e",
        "f",
        "g",
        "h",
        "i",
        "j",
        "k",
        "l",
        "m",
        "n",
        "o",
        "p",
        "Q",
        "q",
        "r",
        "s",
        "t",
        "u",
        "v",
        "w",
        "x",
        "y",
        "z",
        "A",
        "B",
        "C",
        "D",
        "E",
        "F",
        "G",
        "H",
        "I",
        "J",
        "K",
        "L"
    };
//    HashEntry *arr[ARR_SIZE] = {0}; 

    HashTable *hash_table = init_hash_table(8);
    if (hash_table == NULL) {
        printf("Failed to initialize hash table!\n");
        exit(EXIT_FAILURE);
    }
    
#ifdef DEBUG
    printf("Tried to initialize hash table\n");
    printf("======== Results ======== \n");
    printf("Table size: %d\n", hash_table->table_size);
    printf("# of entries: %d\n", hash_table->count_entry);
    printf("load factor (alpha): %.3f\n", hash_table->load_factor);
    printf("Table array address: %p\n", hash_table->table);
#endif

    Fnv32_t hval;
    int index;

    for (int i = 0; i < 37; i += 2) {
        hval = fnv_32a_str(strings[i], FNV1_32A_INIT); 
        index = (hval % hash_table->table_size + hash_table->table_size) % hash_table->table_size;

        if (hash_table->table[index] == NULL) {
            HashEntry *entry = malloc(sizeof(HashEntry));
            if (!entry) {
                printf("Could not allocate hash entry!\n");
                exit(EXIT_FAILURE);
            }

            entry->key = strings[i];
            entry->value = strings[i + 1];
            hash_table->table[index] = entry;
            hash_table->count_entry++;

            printf("Load factor: %.7f\n", hash_table->load_factor);
            hash_table->update_lf(hash_table); 

            printf("Should append: (%s, %s)\n", hash_table->table[index]->key, hash_table->table[index]->value);
        }
        else {
#ifdef DEBUG
            printf("Load factor: %.7f\n", hash_table->load_factor);
#endif
            /*
             * Handle collisions by linear probing
             */
            if (handle_collision(strings[i], strings[i + 1], hash_table, index) != SUCCESS) {
                exit(EXIT_FAILURE);
            }

        } 

#ifdef DEBUG
        printf("hash value from '%s': %d, init hval: %d\n", strings[i], hval, FNV1_32A_INIT);
#endif
    }

#ifdef DEBUG
    if (print_array(hash_table) != SUCCESS) 
        exit(EXIT_FAILURE);
#endif

    /*
     * Do a search in the array by key 
     */
    char key[256];
    while (scanf("%s", key) != EOF) {

#ifdef DEBUG
        printf("READ KEY: %s\n", key);
#endif

        index = search(key, hash_table);
        if (index == ARRAY_IS_NULL) 
            exit(EXIT_FAILURE);

        if (index == KEY_NOT_FOUND) {
            printf("Key does not exist!\n");
        } else {
            printf("key: %s. value: %s\n", hash_table->table[index]->key, hash_table->table[index]->value);
        }
    }
    
    /*
     * TODO: REDO
     */
    if (free_table(hash_table) != SUCCESS)
        exit(EXIT_FAILURE);

    exit(EXIT_SUCCESS);
}
