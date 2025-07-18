#include "lru_cache.h"

LRUCache *init_lru_cache(size_t capacity) {
    LRUCache *lru = (LRUCache *)calloc(1, sizeof(LRUCache));
    if (!lru) {
        fprintf(stderr, "Could not allocate memory for LRUCache struct!\n");
        return NULL;
    }
  
    if (capacity <= 0) {
        fprintf(stderr, "Capacity cannot be less than 1!\n");
        return NULL;
    }
    lru->capacity = capacity;
    lru->size = 0;
    lru->hash_table = init_hash_table(capacity * 2);
    lru->dll = init_linked_list();
    if (!lru->dll || !lru->hash_table)
        return NULL;
    
    return lru;

}

int get(LRUCache *lru, const char *key) {
    if (!lru) {
        fprintf(stderr, "LRU is not valid or is null!\n");
        return IS_NULL;
    }

    if (!key) {
        fprintf(stderr, "The key provided is invalid or NULL!\n");
        return IS_NULL;
    }

    int index = search_entry(key, lru->hash_table);
    if (index < 0) {
        fprintf(stderr, "LRU: Could not find entry in the hash table!\n");
        return FAILURE;
    }

    /*
     * Hash table contains key and value which is a node in linked list
     * that contains our value
     */
    Node *accessed_node = (Node *)lru->hash_table->table[index]->value;

#ifdef DEBUG
    printf("Found value: %s, using key: %s\n", (char *)(accessed_node->data), key);
#endif

    /*
     * Place accessed item at the top of the list
     * as most recently used
     */
    Node *prev_node = accessed_node->prev;
    Node *next_node = accessed_node->next;

    /*
     * Already at the top of the list
     */
    if (!prev_node)
        return SUCCESS;
    /*
     * Connect previous and next nodes
     * of accessed node
     */
    prev_node->next = next_node;
    if (next_node)
        next_node->prev = prev_node;
    else
        lru->dll->tail = prev_node;

    /*
     * Place accessed node at the top
     */
    lru->dll->head->prev = accessed_node;
    accessed_node->prev = NULL;
    accessed_node->next = lru->dll->head;
    lru->dll->head = accessed_node;

    return index;
}

int put(LRUCache *lru, const char *key, char *value) {
    if (!lru) {
        fprintf(stderr, "LRU is not valid or is null!\n");
        return IS_NULL;
    }

    if (!key) {
        fprintf(stderr, "The key provided is invalid or NULL!\n");
        return IS_NULL;
    }

    if (!value) {
        fprintf(stderr, "The value provided is invalid or NULL!\n");
        return IS_NULL;
    }

    if (lru->size == lru->capacity) {
        int index = search_entry(key, lru->hash_table);
        if (index < 0) {
            fprintf(stderr, "LRU: Could not find entry in the hash table!\n");
            return FAILURE;
        }

        free(lru->hash_table->table[index]);
        if (delete_at_end(lru->dll) != SUCCESS)
            return FAILURE;

    }

    if (insert_at_front(lru->dll, (void *)value) != SUCCESS) {
        fprintf(stderr, "LRU: Could not insert entry to the linked list!\n");
        return FAILURE;
    }

#ifdef DEBUG
    printf("DEBUG: DLL HEAD DATA FIELD CONTAINS: %s\n", (char *)lru->dll->head->data);
#endif

    int index = add_hash_entry(key, (void *)lru->dll->head, lru->hash_table);
    if (index < 0) {
        fprintf(stderr, "LRU: Could not add entry to the hash table!\n");
        return FAILURE;
    }

    (lru->size)++;

    return SUCCESS;
}

void free_lru(LRUCache *lru) {
    if (!lru) {
        fprintf(stderr, "LRUCache is not valid or is null!\n");
        fprintf(stderr, "Could not free LRUCache!\n");
        return;
    }

    free_table(lru->hash_table);
    free_dll(lru->dll);
    free(lru);
    lru = NULL;

    return;
}
