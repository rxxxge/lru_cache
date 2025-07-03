#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

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
#define ARRAY_IS_NULL -1
#define ERR_ARRAY_IS_FULL -2
#define KEY_NOT_FOUND -3

/*
 * Type of element in the array
 */
typedef struct HashEntry {
    const char *key;
    char *value;
} HashEntry;

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

int search(char *key, HashEntry *arr[]) {
    if (arr == NULL) {
        printf("Array is not valid!\n");
        return ARRAY_IS_NULL;
    }

    Fnv32_t hval = fnv_32a_str(key, FNV1_32A_INIT);
    int index = (hval % ARR_SIZE + ARR_SIZE) % ARR_SIZE;

#ifdef DEBUG
    printf("Hash value from %s is %d\n", key, hval);
    printf("Computed index: %d\n", index);
#endif

    if (arr[index] != NULL) {
        return index;
    } 
        
    return KEY_NOT_FOUND;
}

int print_array(HashEntry *arr[]) {
    if (arr == NULL) {
        printf("Array is not valid!\n");
        return ARRAY_IS_NULL;
    }

    printf("[Index] --- (key, value)\n");
    for (int i = 0; i < ARR_SIZE; i++) {
        if (arr[i] != NULL) {
            printf("[%d] --- (%s, %s)\n |\n", i, arr[i]->key, arr[i]->value);
        } else {
            printf("[%d]\n |\n", i);
        }
    }

    return SUCCESS;
}

/*
 * Handle collision by linear probing
 */
int handle_collision(const char *key, char *value, HashEntry* arr[], int index) {
    if (arr == NULL) {
        printf("Array is not valid!\n");
        return ARRAY_IS_NULL;
    }

    int original_index = index;
    do {
        index = (index + 1) % ARR_SIZE;
        if (arr[index] == NULL) {
            HashEntry *entry = malloc(sizeof(HashEntry));
            entry->key = key;
            entry->value = value;
            arr[index] = entry;
            return SUCCESS;
        } 
    } while (index != original_index);
    
    return ERR_ARRAY_IS_FULL;
}

/*
 * Free array at the end of program?
 * Useless?
 */
int free_array(HashEntry *arr[]) {
    if (arr == NULL) {
        printf("Array is not valid!\n");
        return ARRAY_IS_NULL;
    }

    for (int i = 0; i < ARR_SIZE; i++) {
        if (arr[i] != NULL) {
            free(arr[i]);
            arr[i] = NULL;
        }
    }

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
    HashEntry *arr[ARR_SIZE] = {0}; 

    Fnv32_t hval;
    int index;

    printf("Array size: %d\n", ARR_SIZE);

    for (int i = 0; i < 37; i += 2) {
        hval = fnv_32a_str(strings[i], FNV1_32A_INIT); 
        index = (hval % ARR_SIZE + ARR_SIZE) % ARR_SIZE;

        if (arr[index] == NULL) {
            HashEntry *entry = malloc(sizeof(HashEntry));
            if (!entry) {
                printf("Could not allocate hash entry!\n");
                exit(EXIT_FAILURE);
            }

            entry->key = strings[i];
            entry->value = strings[i + 1];
            arr[index] = entry;
            printf("Should append: (%s, %s)\n", arr[index]->key, arr[index]->value);
        }
        else {
            /*
             * Handle collisions by linear probing
             */
            int result = handle_collision(strings[i], strings[i + 1], arr, index);
            if (result == ERR_ARRAY_IS_FULL) {
                /*
                 * TODO: Resize the array
                 */
                printf("NEED TO RESIZE!\n");
            } else if (result == ARRAY_IS_NULL) {
                exit(EXIT_FAILURE);
            }

        } 

#ifdef DEBUG
        printf("hash value from '%s': %d, init hval: %d\n", strings[i], hval, FNV1_32A_INIT);
#endif
    }

#ifdef DEBUG
    if (print_array(arr) != SUCCESS) 
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

        index = search(key, arr);
        if (index == ARRAY_IS_NULL) 
            exit(EXIT_FAILURE);

        if (index == KEY_NOT_FOUND) {
            printf("Key does not exist!\n");
        } else {
            printf("key: %s. value: %s\n", arr[index]->key, arr[index]->value);
        }
    }

    if (free_array(arr) != SUCCESS)
        exit(EXIT_FAILURE);

    exit(EXIT_SUCCESS);
}
