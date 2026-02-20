#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <cbarroso/_hash.h>
#include <cbarroso/constants.h>
#include <cbarroso/hashmap.h>

#define MKIX_EMPTY -1
#define MKIX_DUMMY -2
#define MKIX_ERROR -3
#define PERTURB_SHIFT 5

#define USABLE_FRACTION(n) (((n) << 1) / 3)

static ssize_t sHashMap__getIndex(HashMap *self, ssize_t maskedHash)
{
    if (self->log2_size < 8)
    {
        return ((int8_t *)self->indices)[maskedHash];
    }
    else if (self->log2_size < 16)
    {
        return ((int16_t *)self->indices)[maskedHash];
    }
    else if (self->log2_size >= 32)
    {
        return ((int64_t *)self->indices)[maskedHash];
    }
    else
    {
        return ((int32_t *)self->indices)[maskedHash];
    }
}

HashMapEntry **HashMap__getEntries(HashMap *self)
{
    return (HashMapEntry **)(&self->indices[1 << self->log2_index_bytes]);
}

static size_t sHashMap__getMask(HashMap *self)
{
    return ((int64_t)1 << self->log2_size) - 1;
}

static ssize_t sHashMap__doLookup(HashMap *self, void *key, size_t keySize, hash_t hash)
{
    size_t mask = sHashMap__getMask(self);
    size_t maskedHash = (size_t)hash & mask;
    size_t perturb = hash;
    ssize_t index = MKIX_DUMMY;
    uint8_t isSameKey = 0;
    HashMapEntry **entries = HashMap__getEntries(self);
    uint8_t shouldStopLoop = 0;

    do
    {
        index = sHashMap__getIndex(self, maskedHash);

        if (index >= 0)
        {
            uint8_t isSameSize = (entries[index]->keySize == keySize);
            isSameKey = isSameSize && memcmp(key, entries[index]->key, keySize) == 0;
        }

        perturb >>= PERTURB_SHIFT;
        maskedHash = mask & (maskedHash * 5 + perturb + 1);

        shouldStopLoop = index == MKIX_EMPTY || (isSameKey && index >= 0);
    } while (!shouldStopLoop);

    return index;
}

static uint8_t sIsUnusableSlot(ssize_t index)
{
    return index > MKIX_EMPTY;
}

static ssize_t sHashMap__findEmptySlot(HashMap *self, hash_t hash)
{
    const size_t mask = sHashMap__getMask(self);
    size_t maskedHash = hash & mask;
    ssize_t index = sHashMap__getIndex(self, maskedHash);

    for (size_t perturb = hash; sIsUnusableSlot(index);)
    {
        perturb >>= PERTURB_SHIFT;
        maskedHash = (maskedHash * 5 + perturb + 1) & mask;
        index = sHashMap__getIndex(self, maskedHash);
    }

    return maskedHash;
}

static void sHashMap__setIndex(HashMap *self, size_t hashPos, ssize_t index)
{
    assert(index > MKIX_DUMMY);

    if (self->log2_size < 8)
    {
        ((uint8_t *)self->indices)[hashPos] = index;
    }
    else if (self->log2_size < 16)
    {
        ((uint16_t *)self->indices)[hashPos] = index;
    }
    else if (self->log2_size >= 32)
    {
        ((uint64_t *)self->indices)[hashPos] = index;
    }
    else
    {
        ((uint32_t *)self->indices)[hashPos] = index;
    }
}

static void sHashMap__keysEntryAdded(HashMap *self)
{
    self->nentries++;
    self->usable--;
}

static ssize_t sHashMap__insertKey(HashMap *self, void *key, size_t keySize, hash_t hash)
{
    ssize_t index = sHashMap__doLookup(self, key, keySize, hash);

    if (index >= 0)
    {
        return index;
    }

    if (index == MKIX_EMPTY)
    {
    }

    return index;
}

static uint8_t sHashMap__getNextSize(HashMap *self)
{
    ssize_t minsize = self->nentries * 3;
    uint8_t log2_size;

    for (log2_size = LOG2_MINSIZE;
         (((ssize_t)1) << log2_size) < minsize;
         log2_size++)
        ;
    return log2_size;
}

static int8_t sHashMap__insertionResize(HashMap *self)
{
    uint8_t log2_newsize = sHashMap__getNextSize(self);

    if (log2_newsize >= sizeof(size_t) * 8)
    {
        fprintf(stderr, "HashMap size overflowed");
        return -1;
    }

    assert(log2_newsize >= LOG2_MINSIZE);

    HashMap *newHashMap = HashMap__new(log2_newsize);

    if (newHashMap == NULL)
    {
        return -1;
    }

    assert(newHashMap->usable > self->nentries);
    HashMapEntry **entries = HashMap__getEntries(self);
    ssize_t index = 0;

    for (int entryIndex = 0; entryIndex < self->nentries; entryIndex++)
    {
        if (entries[entryIndex]->key != NULL && entries[entryIndex]->value != NULL)
        {
            HashMap__setItem(newHashMap,
                             entries[entryIndex]->key,
                             entries[entryIndex]->keySize,
                             entries[entryIndex]->value,
                             entries[entryIndex]->valueSize);
        }
    }

    *self = *newHashMap;
    free(newHashMap);

    return 0;
}

HashMap *HashMap__new(uint8_t log2_size)
{
    uint8_t log2_index_bytes = 0;
    ssize_t usable = USABLE_FRACTION(1 << log2_size);

    if (log2_size < 8)
    {
        log2_index_bytes = log2_size;
    }
    else if (log2_size < 16)
    {
        log2_index_bytes = log2_size + 1;
    }
    else if (log2_size >= 32)
    {
        log2_index_bytes = log2_size + 3;
    }
    else
    {
        log2_index_bytes = log2_size + 2;
    }

    size_t indicesSize = (size_t)1 << log2_index_bytes;
    size_t entriesSize = sizeof(HashMapEntry) * usable;
    HashMap *hashMap = malloc(
        sizeof(HashMap) + indicesSize + entriesSize);

    if (hashMap == NULL)
    {
        return NULL;
    }

    hashMap->usable = usable;
    hashMap->nentries = 0;
    hashMap->log2_size = log2_size;
    hashMap->log2_index_bytes = log2_index_bytes;
    // Intialize `indices` as an array of `MKIX_EMPTY`s
    memset(hashMap->indices, MKIX_EMPTY, (size_t)1 << log2_index_bytes);
    // Intialize `entries` as an array of nulls
    memset(&hashMap->indices[(size_t)1 << log2_index_bytes],
           0,
           sizeof(HashMap) * usable);

    return hashMap;
}

int8_t HashMap__setItem(HashMap *self,
                        void *key,
                        size_t keySize,
                        void *value,
                        size_t valueSize)
{
    assert(key);
    assert(value);

    if (self->usable <= 0)
    {
        if (sHashMap__insertionResize(self) < 0)
        {
            return CBR_ERROR;
        }
    }

    hash_t hash = hashBuffer(key, keySize);
    ssize_t hashPos = sHashMap__findEmptySlot(self, hash);
    sHashMap__setIndex(self, hashPos, self->nentries);
    HashMapEntry **entries = HashMap__getEntries(self);
    entries[self->nentries] = malloc(sizeof(HashMapEntry));
    HashMapEntry *entry = entries[self->nentries];
    entry->hash = hash;

    entry->key = malloc(keySize);

    if (entry->key == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for hash map key");
        return CBR_ERROR;
    }

    memcpy(entry->key, key, keySize);

    entry->value = malloc(valueSize);

    if (entry->value == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for hash map value");
        return CBR_ERROR;
    }

    memcpy(entry->value, value, valueSize);

    entry->keySize = keySize;
    entry->valueSize = valueSize;
    sHashMap__keysEntryAdded(self);

    return CBR_SUCCESS;
}

int8_t HashMap__getItem(HashMap *self,
                        void *key,
                        size_t keySize,
                        void **valueAddr)
{
    ssize_t index = sHashMap__doLookup(self, key, keySize, hashBuffer(key, keySize));

    if (index == MKIX_EMPTY)
    {
        *valueAddr = NULL;
        return 0;
    }

    *valueAddr = HashMap__getEntries(self)[index]->value;

    return 0;
}

void HashMap__del(HashMap *self)
{
    HashMapEntry **entries = HashMap__getEntries(self);

    for (size_t i = 0; i < self->nentries; i++)
    {
        if (entries[i] != NULL)
        {
            free(entries[i]);
        }
    }

    free(self);
}
