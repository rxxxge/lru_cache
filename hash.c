#include <stdio.h>
#include <sys/types.h>

/*
 * 32 bit FNV-0 hash type
 */
typedef u_int32_t Fnv32_t;

/*
 * 32 bit magic FNV-1a prime
 */
#define FNV_32_PRIME ((Fnv32_t)0x01000193)
#define FNV1_32_INIT ((Fnv32_t)0x811c9dc5)
#define FNV1_32A_INIT FNV1_32_INIT

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

int main(void) {
    char *strings[11] = {
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
    };
    int arr_size = 17;

    Fnv32_t hval;
    int index;

    printf("Array size: %d\n", arr_size);
    for (int i = 0; i < 11; i++) {
        hval = fnv_32a_str(strings[i], FNV1_32A_INIT); 
        index = (hval % arr_size + arr_size) % arr_size;

        printf("hash value from '%s': %d, hval: %d\n", strings[i], hval, FNV1_32A_INIT);
        printf("Index: %d\n", index);
    }
    
    return 0;
}
