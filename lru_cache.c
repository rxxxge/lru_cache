#include "lru_cache.h"

void print_list_pair(DLL *dll) {
    if (!dll) {
        fprintf(stderr, "Doubly linked list is not valid or is null!\n");
        return;
    }
    
    Node *current = dll->head;
    
    printf("HEAD ");
    while (current) {
        if (current == dll->tail) {
            printf("(%s) ", (char *)((Pair *)current->data)->key);
            current = current->next;
            continue;
        }
        printf("(%s) <--> ",  (char *)((Pair *)current->data)->key);
        current = current->next;
    }

    printf("TAIL\n");
}

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
    printf("Found value: %s, using key: %s\n", (char *)((Pair *)accessed_node->data)->value, key);
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

    if (lru->hash_table->count_entry == lru->capacity) {
        const char *tail_key = (char *)((Pair *)lru->dll->tail->data)->key;

        printf("TAIL_KEY: %s\n", tail_key);
        // Find Least Recently Used entry 
        int index = search_entry(tail_key, lru->hash_table);
        if (index < 0) {
            fprintf(stderr, "LRU: Could not find entry in the hash table!\n");
            return FAILURE;
        }

        /* Free the hash table from the LRU entry and remove tail node 
         * from the end of the linked list
         */
        free(lru->hash_table->table[index]);
        lru->hash_table->table[index] = NULL;

        free(lru->dll->tail->data);
        lru->dll->tail->data = NULL;
        if (delete_at_end(lru->dll) != SUCCESS)
            return FAILURE;
    }
    
    Pair *pair = calloc(1, sizeof(Pair));
    if (!pair) {
        fprintf(stderr, "Could not allocate Pair struct!\n");
        return IS_NULL;
    }

    pair->key = (void *)key;
    pair->value = (void *)value;

    if (insert_at_front(lru->dll, (void *)pair) != SUCCESS) {
        fprintf(stderr, "LRU: Could not insert entry to the linked list!\n");
        return FAILURE;
    }

#ifdef DEBUG
    printf("DEBUG: DLL HEAD DATA FIELD CONTAINS: %s\n", (char *)((Pair *)lru->dll->head->data)->key);
#endif

    bool auto_resize = false; // Do not auto resize the hash table
    int index = add_hash_entry(key, (void *)lru->dll->head, lru->hash_table, auto_resize);
    if (index < 0) {
        fprintf(stderr, "LRU: Could not add entry to the hash table!\n");
        return FAILURE;
    }

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
