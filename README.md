# LRU Cache Implementation in C

A high-performance Least Recently Used (LRU) cache implementation in pure C using doubly linked lists and hash tables for O(1) operations.

## Features

- **O(1) Time Complexity**: Get and put operations
- **Custom Hash Table**: Implemented with collision handling using linear probing
- **Doubly Linked List**: Efficient insertion/deletion at both ends
- **Configurable Capacity**: Set maximum cache size

## Structure

```
src/
├── lru_cache.c         # Main LRU cache implementation
├── lru_cache.h         # Header file with API definitions
├── hash.c              # Hash table implementation
├── hash.h              # Hash table header
├── dll.c               # Doubly linked list implementation
├── dll.h               # Doubly linked list header
├── test_lru.c          # Example usage of lru_cache
├── test_hash.c         # Tests for hash table and some usage examples
└── test_dll.c          # Example usage of Linked list 
```

## API Reference

### Core Functions

```c
// Create a new LRU cache with specified capacity
LRUCache *init_lru_cache(size_t capacity);

// Get value by key (moves to front)
int get(LRUCache *lru, const char *key);

// Put key-value pair (evicts LRU if full)
int put(LRUCache *lru, const char *key, char *value);

// Destroy cache and free memory
void free_lru(LRUCache *lru);
```

## Usage Example

```c
#include "lru_cache.h"

int main() {
    // Create cache with capacity of 4
    LRUCache *lru = init_lru_cache(4);

    // Add some key-value pairs
    put(lru, "key1", "value1");
    put(lru, "key2", "value2");
    put(lru, "key3", "value3");
    put(lru, "key4", "value4");

    // Get a value
    int index = get(lru, "key1");
    if (index >= 0) {
        void *value = lru->hash_table->table[index];
        printf("value from key1: %s\n", (char *)value);
    }
    
    // Adding another item will remove it from the cache as least recently used 
    put(lru, "key5", "value5");  // Evicts key 2
    
    // Clean up
    free_lru(lru);

    return 0;
}
```

## Building

### Prerequisites
- GCC
- Make

### Using Makefile

```bash
make test_lru
make test_hash
make test_dll

make clean
```
