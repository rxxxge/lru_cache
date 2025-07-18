#ifndef _HASH_H_
#define _HASH_H_

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>

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
 * Function return codes
 */
#define SUCCESS 0
#define FAILURE -1
#define IS_NULL -2

#define RESIZE_UP true
#define RESIZE_DOWN false

/*
 * Maximum/minimum load factor alpha
 */
#define ALPHA_MAX 0.72
#define ALPHA_MIN 0.18

/*
 * Type of element in the array
 */
typedef struct HashEntry {
    const char *key;
    void *value;
} HashEntry;

typedef struct HashTable {
    size_t table_size;
    unsigned int count_entry;
    float load_factor;
    HashEntry **table;

    /*
     * Function pointer to update load factor
     */
    int (*update_lf)(struct HashTable *);
} HashTable;

/*
 * Hash function
 */
Fnv32_t fnv_32a_str(const char *str, Fnv32_t hval);

/*
 * UTILITY
 * Get index using key and hash function
 */
int get_index(const char *key, size_t table_size);

/*
 * UTILITY
 * Update the load factor
 */
int update_load_factor(HashTable *table);

/*
 * UTILITY
 * Utility function to print the whole table
 */
int print_table(HashTable *table);

/*
 * UTILITY
 * Resize the table based on load factor
 */
int resize_on_lf(HashTable *table, bool size_up);

/*
 * Resize the table specifying boolean 
 * "size_up" parameter to increase/decrease size
 * of the table accordingly
 */
int resize_table(HashTable *table, bool size_up);

/*
 * Initialize hash table
 */
HashTable *init_hash_table(size_t table_size);

/*
 * Search entry by the key.
 * Returns index in the hash table on success
 */
int search_entry(const char *key, HashTable *table);

/*
 * Handle collision by linear probing
 */
int handle_collision(const char *key, void *value, HashTable *table, int index, bool auto_resize);

/*
 * Creates entry (key, value) pair to the table array at computed index
 */
int create_hash_entry(const char *key, void *value, HashTable *table, int index, bool auto_resize);

/*
 * Wrapper function around create_hash_entry to add entry to the table array
 * Returns index on success
 * Resizes automatically
 */
int add_hash_entry(const char *key, void *value, HashTable *table);

/*
 * Removes pair by its key (if it exists)
 */
int remove_hash_entry(const char *key, HashTable *table, bool auto_resize);

/*
 * Free the hash table at the end
 */
void free_table(HashTable *table);

#endif // _HASH_H_
