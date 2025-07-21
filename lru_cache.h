#include "hash.h"
#include "dll.h"

#define SUCCESS 0
#define FAILURE -1
#define IS_NULL -2

typedef struct LRUCache {
    size_t capacity;
    HashTable *hash_table;
    DLL *dll;
} LRUCache;

typedef struct Pair {
    void *key;
    void *value;
} Pair;

// Temp
void print_list_pair(DLL *dll);

LRUCache *init_lru_cache(size_t capacity);
int get(LRUCache *lru, const char *key);
int put(LRUCache *lru, const char *key, char *value);
void free_lru(LRUCache *lru);
