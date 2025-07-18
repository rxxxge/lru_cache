/*
 * dll.c
 * Implementation of doubly linked list methods
 */
#include "dll.h"

DLL *init_linked_list(void) {
    DLL *dll = (DLL *)calloc(1, sizeof(DLL)); 
    if (!dll) {
        fprintf(stderr, "Could not allocate doubly linked list!\n");
        return NULL;
    }
    dll->list_size = 0;
    dll->head = NULL;
    dll->tail = NULL;

    return dll;
}

int insert_at_front(DLL *dll, void *data) {
    if (!dll) {
        fprintf(stderr, "Doubly linked list is not valid or is null!\n");
        return IS_NULL;
    }

    Node *new_node = (Node *)calloc(1, sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Could not allocate a node!\n");
        return IS_NULL;
    }

    new_node->prev = NULL;
    
    if (dll->list_size > 0) {
        new_node->next = dll->head;
        dll->head->prev = new_node; 
    } else {
        new_node->next = NULL;
        dll->tail = new_node;
    }

    dll->head = new_node;

    /*
     * "data" can be NULL
     */
    new_node->data = data;
    (dll->list_size)++;

    return SUCCESS;
}

int insert_at_end(DLL *dll, void *data) {
    if (!dll) {
        fprintf(stderr, "Doubly linked list is not valid or is null!\n");
        return IS_NULL;
    }

    Node *new_node = (Node *)calloc(1, sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Could not allocate a node!\n");
        return IS_NULL;
    }

    new_node->next = NULL;
    
    if (dll->list_size > 0) {
        new_node->prev = dll->tail;
        dll->tail->next = new_node; 
    } else {
        new_node->prev = NULL;
        dll->head = new_node;
    }

    dll->tail = new_node;

    /*
     * "data" can be NULL
     */
    new_node->data = data;
    (dll->list_size)++;

    return SUCCESS;
}

int delete_at_front(DLL *dll) {
    if (!dll) {
        fprintf(stderr, "Doubly linked list is not valid or is null!\n");
        return IS_NULL;
    }

    if (dll->list_size == 0) {
        printf("List is empty. Nothing to delete!\n");
        return SUCCESS;
    }

    Node *old_head = dll->head;
    Node *new_head = dll->head->next;

    dll->head = new_head;
    dll->head->prev = NULL;

    (dll->list_size)--;
    free(old_head);
    old_head = NULL;

    return SUCCESS;
}

int delete_at_end(DLL *dll) {
    if (!dll) {
        fprintf(stderr, "Doubly linked list is not valid or is null!\n");
        return IS_NULL;
    }

    if (dll->list_size == 0) {
        printf("List is empty. Nothing to delete!\n");
        return SUCCESS;
    }

    Node *old_tail = dll->tail;
    Node *new_tail = dll->tail->prev;

    dll->tail = new_tail;
    dll->tail->next = NULL;

    (dll->list_size)--;
    free(old_tail);
    old_tail = NULL;

    return SUCCESS;
}

void print_list(DLL *dll) {
    if (!dll) {
        fprintf(stderr, "Doubly linked list is not valid or is null!\n");
        return;
    }
    
    Node *current = dll->head;
    
    printf("HEAD ");
    while (current) {
        if (current == dll->tail) {
            printf("(%s) ", (char *)(current->data));
            current = current->next;
            continue;
        }
        printf("(%s) <--> ", (char *)(current->data));
        current = current->next;
    }

    printf("TAIL\n");
}

void free_dll(DLL *dll) {
    if (!dll) {
        fprintf(stderr, "Doubly linked list is not valid or is null!\n");
        fprintf(stderr, "Could not free Doubly linked list!\n");
        return;
    }

    /*
     * Start with HEAD node 
     * While current is not NULL or current == tail->next
     * Save next node and free current one
     */
    Node *current = dll->head;
    while (current) {
        Node* next_node = current->next;
        free(current);
        current = next_node;
    }    

    free(dll);
    dll = NULL;
    
    return;
}
