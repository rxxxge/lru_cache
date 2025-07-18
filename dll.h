#ifndef _DLL_H_
#define _DLL_H_

#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define FAILURE -1
#define IS_NULL -2

/*
 * Node type
 * data field must be casted to appropriate type by the user
 */
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

DLL *init_linked_list(void);
int insert_at_front(DLL *dll, void *data);
int insert_at_end(DLL *dll, void *data);
int delete_at_front(DLL *dll);
int delete_at_end(DLL *dll);
void print_list(DLL *dll);
void free_dll(DLL *dll);

#endif
