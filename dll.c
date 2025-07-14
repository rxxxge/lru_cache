#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define FAILURE -1
#define IS_NULL -2

typedef struct Node {
    void *data;
    struct Node *next;
    struct Node *prev;
} Node;

/*
 * Doubly Linked List
 */
typedef struct DLL {
   size_t list_size;
   Node *head;
   Node *tail;
} DLL;

DLL *init_linked_list(void) {
    DLL *dll = calloc(1, sizeof(DLL)); 
    if (!dll) {
        fprintf(stderr, "Could not allocate doubly linked list!\n");
        return NULL;
    }
    dll->list_size = 0;
    
    /*
     * Allocate memory for tail
     */

    //head->data = NULL;
    //head->next = tail;
    //head->prev = NULL;

    //tail->data = NULL;
    //tail->next = NULL;
    //tail->prev = head;
#ifdef DEBUG
    //printf("ADDRESS OF HEAD: %p\n", (void *)head);
    //printf("ADDRESS OF TAIL: %p\n", (void *)tail);

    //printf("ADDRESS OF HEAD->NEXT: %p\n", (void *)head->next);
    //printf("ADDRESS OF TAIL->PREV: %p\n", (void *)tail->prev);
#endif

    dll->head = NULL;
    dll->tail = NULL;

    return dll;
}

int insert_at_front(DLL *dll, void *data) {
    if (!dll) {
        fprintf(stderr, "Doubly linked list is not valid or is null!\n");
        return IS_NULL;
    }

    Node *new_node = calloc(1, sizeof(Node));
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

    Node *new_node = calloc(1, sizeof(Node));
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

int main(void) {
    DLL *dll = init_linked_list(); 
    if (!dll) {
        fprintf(stderr, "Doubly linked list is not valid or is null!\n");
        exit(EXIT_FAILURE);
    }

    printf("DLL size: %ld\n", dll->list_size);

    int data = 1;
    if (insert_at_front(dll, &data) != SUCCESS)
        exit(EXIT_FAILURE);

    printf("DLL size: %ld\n", dll->list_size);
    printf("DLL head: %p\n", (void *)dll->head);
    printf("DLL head->next: %p\n", (void *)dll->head->next);
    printf("DLL tail: %p\n", (void *)dll->tail);
    printf("DLL tail->prev: %p\n", (void *)dll->tail->prev);

    printf("Head node data: %d\n", *(int *)(dll->head->data));
    
    int data2 = 2;
    if (insert_at_front(dll, &data2) != SUCCESS)
        exit(EXIT_FAILURE);

    printf("DLL size: %ld\n", dll->list_size);
    printf("DLL head: %p\n", (void *)dll->head);
    printf("DLL head->next: %p\n", (void *)dll->head->next);
    printf("DLL tail: %p\n", (void *)dll->tail);
    printf("DLL tail->prev: %p\n", (void *)dll->tail->prev);

    printf("Head node data: %d\n", *(int *)(dll->head->data));

    int data3 = 3;
    if (insert_at_front(dll, &data3) != SUCCESS)
        exit(EXIT_FAILURE);

    printf("DLL size: %ld\n", dll->list_size);
    printf("DLL head: %p\n", (void *)dll->head);
    printf("DLL head->next: %p\n", (void *)dll->head->next);
    printf("DLL tail: %p\n", (void *)dll->tail);
    printf("DLL tail->prev: %p\n", (void *)dll->tail->prev);

    printf("Head node data: %d\n", *(int *)(dll->head->data));
    printf("Tail node data: %d\n", *(int *)(dll->tail->data));

    int data4 = 4;
    if (insert_at_end(dll, &data4) != SUCCESS)
        exit(EXIT_FAILURE);

    printf("DLL size: %ld\n", dll->list_size);
    printf("DLL head: %p\n", (void *)dll->head);
    printf("DLL head->next: %p\n", (void *)dll->head->next);
    printf("DLL tail: %p\n", (void *)dll->tail);
    printf("DLL tail->prev: %p\n", (void *)dll->tail->prev);

    printf("Head node data: %d\n", *(int *)(dll->head->data));
    printf("Tail node data: %d\n", *(int *)(dll->tail->data));
    exit(EXIT_SUCCESS);
}
