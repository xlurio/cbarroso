# cbarroso API Overview

Welcome to the cbarroso library documentation. This library provides high-performance, generic data structures implemented in C99.

## Available Data Structures

### [HashMap](HashMap-API.md)
Efficient key-value storage with O(1) average-case lookup.

**Use when you need:**
- Fast key-value lookups
- Associative arrays
- Caching
- Unique key storage

**Key features:**
- Generic void pointer storage
- Configurable initial size
- Collision handling via chaining

---

### [SinglyLinkedList](SinglyLinkedList-API.md)
Simple linked list with forward-only traversal.

**Use when you need:**
- Sequential data storage
- Forward traversal only
- Simple stack/queue implementation
- Minimal memory overhead

**Key features:**
- O(1) deletion of entire list
- Forward traversal
- Generic value storage
- Lightweight (one pointer per node)

---

### [DoublyLinkedList](DoublyLinkedList-API.md)
Linked list with bidirectional traversal and individual node deletion.

**Use when you need:**
- Bidirectional traversal
- Efficient deletion of specific nodes
- Insert before/after operations
- Navigation in both directions

**Key features:**
- Forward and backward traversal
- O(1) node deletion with node reference
- Maintains link integrity automatically
- Generic value storage

---

## Quick Start

### Installation

```bash
# Build as static library
cmake -B build
cmake --build build

# Install system-wide
sudo cmake --install build --prefix /usr/local
```

### Basic Usage

#### Using CMake (Recommended)

```cmake
# Find the installed library
find_package(cbarroso REQUIRED)

# Link against it
add_executable(my_app main.c)
target_link_libraries(my_app PRIVATE cbarroso::cbarroso)
```

#### Direct Include

After installation, include the headers:

```c
#include <cbarroso/hashmap.h>
#include <cbarroso/sngllnkdlist.h>
#include <cbarroso/dblylnkdlist.h>
```

---

## Common Patterns

### Working with Generic Pointers

All data structures store `void *` pointers, allowing storage of any type:

```c
// Storing integers
int value = 42;
node = SinglyLinkedListNode__new(&value);

// Storing strings
char *str = "Hello";
node = SinglyLinkedListNode__new(str);

// Storing structs
typedef struct { int id; char name[50]; } Person;
Person p = {1, "Alice"};
node = SinglyLinkedListNode__new(&p);
```

### Memory Management

**Important**: The library manages memory for its own structures (nodes, hashmaps) but **NOT** for the values you store.

```c
// You are responsible for:
int *data = malloc(sizeof(int));
*data = 42;
SinglyLinkedListNode *node = SinglyLinkedListNode__new(data);

// Later, clean up both:
SinglyLinkedListNode__del(node);  // Frees the node
free(data);                        // You must free your data
```

### Error Handling

Most functions return status codes or NULL:

```c
// Check node creation
SinglyLinkedListNode *node = SinglyLinkedListNode__new(&value);
if (!node) {
    fprintf(stderr, "Failed to create node\n");
    return 1;
}

// Check insertion
if (SinglyLinkedListNode__insertAtTail(node, &value) != 0) {
    fprintf(stderr, "Insertion failed\n");
    return 1;
}

// Check HashMap retrieval
void *result;
if (HashMap__getItem(map, "key", 3, &result) != 0) {
    printf("Key not found\n");
}
```

### Type Safety

Cast retrieved values to the correct type:

```c
// Storing
int value = 42;
HashMap__setItem(map, "key", 3, &value);

// Retrieving
void *result;
if (HashMap__getItem(map, "key", 3, &result) == 0) {
    int *retrieved = (int *)result;
    printf("Value: %d\n", *retrieved);
}
```

---

## Choosing the Right Data Structure

| Need | Use | Why |
|------|-----|-----|
| Fast key lookups | HashMap | O(1) average lookup time |
| Sequential processing | SinglyLinkedList | Simple, efficient forward traversal |
| Bidirectional traversal | DoublyLinkedList | Can move forward and backward |
| Delete specific items | DoublyLinkedList or HashMap | O(1) deletion with reference |
| Minimal memory | SinglyLinkedList | One pointer per node |
| Unique keys | HashMap | Automatically handles key uniqueness |

---

## Return Codes

The library uses these constants (defined in `cbarroso/constants.h`):

- `CBR_SUCCESS` = `0` - Operation succeeded
- `CBR_ERROR` = `-1` - Operation failed

Example:
```c
if (HashMap__setItem(map, key, key_size, value) == CBR_SUCCESS) {
    printf("Item stored successfully\n");
}
```

---

## Performance Characteristics

### HashMap
- **Insert**: O(1) average
- **Lookup**: O(1) average
- **Memory**: O(n)

### SinglyLinkedList
- **Insert at tail**: O(n) (must traverse)
- **Delete all**: O(n)
- **Traversal**: O(n)
- **Memory**: O(n)

### DoublyLinkedList
- **Insert at tail**: O(n) (must traverse)
- **Delete node**: O(1) (with node reference)
- **Traversal**: O(n) (forward or backward)
- **Memory**: O(n) with 2× pointer overhead

---

## Building and Testing

### Run Tests

```bash
# Configure with testing
cmake -B build -DBUILD_TESTING=ON

# Build tests
cmake --build build

# Run all tests
cd build
ctest --output-on-failure
```

### Available Tests
- `test_hashmap` - HashMap functionality
- `test_sngllnkdlist` - SinglyLinkedList functionality
- `test_dblylnkdlist` - DoublyLinkedList functionality

---

## API Documentation

- **[HashMap API](HashMap-API.md)** - Complete HashMap documentation
- **[SinglyLinkedList API](SinglyLinkedList-API.md)** - Complete SinglyLinkedList documentation
- **[DoublyLinkedList API](DoublyLinkedList-API.md)** - Complete DoublyLinkedList documentation

---

## Contributing

Contributions are welcome! Please see the main README for details.

## License

See LICENSE file for details.
