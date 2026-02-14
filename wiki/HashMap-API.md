# HashMap API

The HashMap provides efficient key-value storage with O(1) average-case lookup.

## Overview

The HashMap uses a hash table implementation with collision handling via chaining. It stores generic void pointers as values, allowing storage of any data type.

## Creating a HashMap

```c
#include <cbarroso/hashmap.h>

HashMap *map = HashMap__new(uint8_t log2_size);
```

### Parameters

- `log2_size` - The log2 of the initial size. The actual size will be `2^log2_size`
  - Example: `log2_size = 8` creates a hashmap with 256 slots
  - Example: `log2_size = 10` creates a hashmap with 1024 slots

### Returns

- Pointer to the newly created HashMap on success
- `NULL` on failure (memory allocation error)

### Example

```c
// Create a hashmap with 2^8 = 256 slots
HashMap *map = HashMap__new(8);
if (!map) {
    fprintf(stderr, "Failed to create hashmap\n");
    return 1;
}
```

## Setting Values

```c
int8_t HashMap__setItem(HashMap *self, char *key, size_t key_size, void *value);
```

### Parameters

- `self` - Pointer to the HashMap
- `key` - Pointer to the key data
- `key_size` - Size of the key in bytes
- `value` - Pointer to the value to store

### Returns

- `0` (CBR_SUCCESS) on success
- `-1` (CBR_ERROR) on error

### Example

```c
char *key = "my_key";
int value = 42;

int8_t result = HashMap__setItem(map, key, strlen(key), &value);
if (result != 0) {
    fprintf(stderr, "Failed to set item\n");
}
```

## Getting Values

```c
int8_t HashMap__getItem(HashMap *self, char *key, size_t key_size, void **value);
```

### Parameters

- `self` - Pointer to the HashMap
- `key` - Pointer to the key data
- `key_size` - Size of the key in bytes
- `value` - Pointer to a void pointer that will receive the stored value

### Returns

- `0` (CBR_SUCCESS) if key was found
- `-1` (CBR_ERROR) if key was not found

### Example

```c
char *key = "my_key";
void *value_ptr;

int8_t result = HashMap__getItem(map, key, strlen(key), &value_ptr);
if (result == 0) {
    int *value = (int *)value_ptr;
    printf("Value: %d\n", *value);
} else {
    printf("Key not found\n");
}
```

## Memory Management

### Cleanup

```c
free(map);
```

**Important**: The HashMap only manages its own internal memory. It does not free the keys or values you store in it. You are responsible for managing the lifetime of the data you store.

## Complete Example

```c
#include <stdio.h>
#include <string.h>
#include <cbarroso/hashmap.h>

int main(void) {
    // Create hashmap with 256 slots
    HashMap *map = HashMap__new(8);
    if (!map) {
        fprintf(stderr, "Failed to create hashmap\n");
        return 1;
    }

    // Store multiple values
    int data1 = 123;
    int data2 = 456;
    int data3 = 789;
    
    HashMap__setItem(map, "first", 5, &data1);
    HashMap__setItem(map, "second", 6, &data2);
    HashMap__setItem(map, "third", 5, &data3);

    // Retrieve values
    void *result;
    if (HashMap__getItem(map, "first", 5, &result) == 0) {
        printf("first: %d\n", *(int *)result);
    }
    
    if (HashMap__getItem(map, "second", 6, &result) == 0) {
        printf("second: %d\n", *(int *)result);
    }
    
    if (HashMap__getItem(map, "third", 5, &result) == 0) {
        printf("third: %d\n", *(int *)result);
    }

    // Try to get non-existent key
    if (HashMap__getItem(map, "missing", 7, &result) != 0) {
        printf("Key 'missing' not found\n");
    }

    // Clean up
    free(map);
    return 0;
}
```

## Best Practices

1. **Choose appropriate size**: Select `log2_size` based on expected number of entries
   - For ~100 items, use `log2_size = 7` (128 slots)
   - For ~1000 items, use `log2_size = 10` (1024 slots)

2. **Key management**: Keep keys valid for the lifetime of the HashMap or until you remove the entry

3. **Value pointers**: Store pointers to data, not temporary values
   ```c
   // Good - data lifetime managed
   int *value = malloc(sizeof(int));
   *value = 42;
   HashMap__setItem(map, "key", 3, value);
   
   // Bad - temporary value
   int temp = 42;
   HashMap__setItem(map, "key", 3, &temp); // temp goes out of scope!
   ```

4. **Type safety**: Cast retrieved values appropriately and ensure type consistency

## See Also

- [SinglyLinkedList API](SinglyLinkedList-API.md)
- [DoublyLinkedList API](DoublyLinkedList-API.md)
- [API Overview](API-Overview.md)
