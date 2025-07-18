#include "dll.h"

int main(void) {
    DLL *dll = init_linked_list(); 
    if (!dll) {
        fprintf(stderr, "Doubly linked list is not valid or is null!\n");
        exit(EXIT_FAILURE);
    }

    printf("DLL size: %ld\n", dll->list_size);

#ifdef DEBUG
    print_list(dll);
#endif

    int data = 1;
    if (insert_at_front(dll, &data) != SUCCESS)
        exit(EXIT_FAILURE);

    printf("DLL size: %ld\n", dll->list_size);
    printf("DLL head: %p\n", (void *)dll->head);
    printf("DLL head->next: %p\n", (void *)dll->head->next);
    printf("DLL tail: %p\n", (void *)dll->tail);
    printf("DLL tail->prev: %p\n", (void *)dll->tail->prev);

    printf("Head node data: %d\n", *(int *)(dll->head->data));

#ifdef DEBUG
    print_list(dll);
#endif
    
    int data2 = 2;
    if (insert_at_front(dll, &data2) != SUCCESS)
        exit(EXIT_FAILURE);

    printf("DLL size: %ld\n", dll->list_size);
    printf("DLL head: %p\n", (void *)dll->head);
    printf("DLL head->next: %p\n", (void *)dll->head->next);
    printf("DLL tail: %p\n", (void *)dll->tail);
    printf("DLL tail->prev: %p\n", (void *)dll->tail->prev);

    printf("Head node data: %d\n", *(int *)(dll->head->data));

#ifdef DEBUG
    print_list(dll);
#endif

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

#ifdef DEBUG
    print_list(dll);
#endif

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

#ifdef DEBUG
    print_list(dll);
#endif

    if (delete_at_front(dll) != SUCCESS)
        exit(EXIT_FAILURE);

    printf("DLL size: %ld\n", dll->list_size);
    printf("DLL head: %p\n", (void *)dll->head);
    printf("DLL head->next: %p\n", (void *)dll->head->next);
    printf("DLL tail: %p\n", (void *)dll->tail);
    printf("DLL tail->prev: %p\n", (void *)dll->tail->prev);

    printf("Head node data: %d\n", *(int *)(dll->head->data));
    printf("Tail node data: %d\n", *(int *)(dll->tail->data));
    
#ifdef DEBUG
    print_list(dll);
#endif

    if (delete_at_end(dll) != SUCCESS)
        exit(EXIT_FAILURE);

    printf("DLL size: %ld\n", dll->list_size);
    printf("DLL head: %p\n", (void *)dll->head);
    printf("DLL head->next: %p\n", (void *)dll->head->next);
    printf("DLL tail: %p\n", (void *)dll->tail);
    printf("DLL tail->prev: %p\n", (void *)dll->tail->prev);

    printf("Head node data: %d\n", *(int *)(dll->head->data));
    printf("Tail node data: %d\n", *(int *)(dll->tail->data));
    
#ifdef DEBUG
    print_list(dll);
#endif

    free_dll(dll);

    exit(EXIT_SUCCESS);
}

