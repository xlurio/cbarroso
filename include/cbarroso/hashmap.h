#ifndef HASHMAP_H
#define HASHMAP_H

#include <cbarroso/_hash.h>
#include <stdint.h>

#define LOG2_MINSIZE 3

typedef struct HashMap
{
    /* $\log_2{size_of_the_index}$ */
    uint8_t log2_size;
    /* $\log_2{size_of_the_indices_array}$ */
    uint8_t log2_index_bytes;
    /* Number of unused slots */
    ssize_t usable;
    /* Number of used slots */
    ssize_t nentries;
    /* An array of `uint{2^log2_index_bytes}_t` indices for the `entries` array*/
    char indices[];
    /* When allocating memory for `HashMap`, an extra space of
    `(1 << log2_index_bytes)` is allocated for the
    `indices` and `(sizeof(HashMapEntry) * usable)` for the `entries` array*/
    /* The hidden `entries` array is where the values actually reside */
} HashMap;

typedef struct HashMapEntry
{
    hash_t hash;
    void *key;
    /* The size of the key buffer in bytes */
    size_t keySize;
    void *value;
    /* The size of the value buffer in bytes */
    size_t valueSize;
} HashMapEntry;

HashMap *HashMap__new(uint8_t log2_size);
HashMapEntry **HashMap__getEntries(HashMap *self);
int8_t HashMap__setItem(HashMap *self,
                        void *key,
                        size_t keySize,
                        void *value,
                        size_t valueSize);
int8_t HashMap__getItem(HashMap *self,
                        void *key,
                        size_t keySize,
                        void **valueAddr);
void HashMap__del(HashMap * self);

#endif
