#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cbarroso/_hash.h>
#include <cbarroso/_types.h>
#include <cbarroso/hashmap.h>
#include <stdio.h>
#include <assert.h>

#define MKIX_EMPTY -1
#define MKIX_DUMMY -2
#define MKIX_ERROR -3
#define PERTURB_SHIFT 5

#define USABLE_FRACTION(n) (((n) << 1) / 3)

HashMap *HashMap__new(uint8_t log2_size)
{
    uint8_t log2_index_bytes = 0;
    ssize_t usable = USABLE_FRACTION(1 << log2_size);

    size_t indicesSize = (1 << log2_size);
    size_t entriesSize = sizeof(HashMapEntry) * usable;
    HashMap *keysObj = malloc(
        sizeof(HashMap) + indicesSize + indicesSize);

    if (keysObj == NULL)
    {
        return NULL;
    }

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

    keysObj->usable = usable;
    keysObj->nentries = 0;
    keysObj->log2_size = log2_size;
    keysObj->log2_index_bytes = log2_index_bytes;
    // Intialize `indices` as an array of `MKIX_EMPTY`s
    memset(keysObj->indices, MKIX_EMPTY, (size_t)1 << log2_index_bytes);
    // Intialize `entries` as an array of nulls
    memset(&keysObj->indices[(size_t)1 << log2_index_bytes],
           0,
           sizeof(HashMap) * usable);

    return keysObj;
}

int8_t HashMap__setItem(HashMap *self, char *key, size_t keySize, void *value)
{
    assert(key);
    assert(value);

    if (self->usable <= 0)
    {
        if (sHashMap__insertionResize(self) < 0)
        {
            return -1;
        }
    }

    hash_t hash = hashBuffer(key, keySize);
    ssize_t hashPos = sHashMap__findEmptySlot(self, hash);
    sHashMap__setIndex(self, hashPos, self->nentries);
    HashMapEntry *entry = sHashMap__getEntries(self)[self->nentries];
    entry->hash = hash;
    entry->key = key;
    entry->keySize = keySize;
    entry->value = value;
    sHashMap__keysEntryAdded(self);

    return 0;
}

int8_t HashMap__getItem(HashMap *self, char *key, size_t keySize, void **valueAddr)
{
    ssize_t index = sHashMap__doLookup(self, key, keySize, hashBuffer(key, keySize));
    *valueAddr = sHashMap__getEntries(self)[index];

    return 0;
}

static ssize_t sHashMap__getIndex(HashMap *self, ssize_t maskedHash)
{
    if (self->log2_size < 8)
    {
        return ((uint8_t *)self->indices)[maskedHash];
    }
    else if (self->log2_size < 16)
    {
        return ((uint16_t *)self->indices)[maskedHash];
    }
    else if (self->log2_size >= 32)
    {
        return ((uint64_t *)self->indices)[maskedHash];
    }
    else
    {
        return ((uint32_t *)self->indices)[maskedHash];
    }
}

static HashMapEntry **sHashMap__getEntries(HashMap *self)
{
    return (&self->indices[1 << self->log2_index_bytes]);
}

static size_t sHashMap__getMask(HashMap *self)
{
    return ((int64_t)1 << self->log2_size) - 1;
}

static ssize_t sHashMap__doLookup(HashMap *self, char *key, size_t keySize, hash_t hash)
{
    size_t mask = sHashMap__getMask(self);
    size_t maskedHash = (size_t)hash & mask;
    size_t perturb = hash;
    ssize_t index = MKIX_DUMMY;
    uint8_t isSameKey = 0;
    HashMapEntry **entries = sHashMap__getEntries(self);
    uint8_t shouldStopLoop = 0;

    do
    {
        index = sHashMap__getIndex(self, maskedHash);

        if (index >= 0)
        {
            isSameKey = strncmp(key, entries[index]->key, keySize);
        }

        perturb >>= PERTURB_SHIFT;
        maskedHash = mask & (maskedHash * 5 + perturb + 1);

        shouldStopLoop = index == MKIX_EMPTY || (isSameKey && index >= 0);
    } while (shouldStopLoop);

    return index;
}

static ssize_t sHashMap__findEmptySlot(HashMap *self, hash_t hash)
{
    const size_t mask = sHashMap__getMask(self);
    size_t maskedHash = hash & mask;
    ssize_t index = sHashMap__getIndex(self, maskedHash);
    for (size_t perturb = hash; is_unusable_slot(index);)
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

static ssize_t sHashMap__insertKey(HashMap *self, char *key, size_t keySize, hash_t hash)
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
    HashMapEntry **entries = sHashMap__getEntries(self);
    ssize_t index = 0;

    for (int entryIndex = 0; entryIndex < self->nentries; entryIndex++)
    {
        if (entries[entryIndex]->key != NULL && entries[entryIndex]->value != NULL)
        {
            HashMap__setItem(newHashMap,
                             entries[entryIndex]->key,
                             entries[entryIndex]->keySize,
                             entries[entryIndex]->value);
        }
    }

    *self = *newHashMap;
    free(newHashMap);

    return 0;
}
