# cbarroso

A C library providing a high-performance hashmap implementation.

## Features

- **HashMap**: Efficient key-value storage with O(1) average-case lookup
- **Modern CMake**: Supports both direct integration and system-wide installation
- **C99 Standard**: Clean, portable C code

## Building

### Prerequisites

- CMake 3.15 or higher
- C99-compatible compiler (GCC, Clang, MSVC, etc.)

### Build as Static Library

```bash
cmake -B build
cmake --build build
```

### Install System-Wide

```bash
cmake -B build
cmake --build build
sudo cmake --install build --prefix /usr/local
```

### Running Tests

To build and run the test suite:

```bash
# Configure with testing enabled
cmake -B build . -DBUILD_TESTING=ON

# Build the library and tests
cmake --build build --target test_hashmap

# Run all tests with CTest
cd build
ctest --output-on-failure

# Or run the test executable directly
./test_hashmap
```

For verbose test output:

```bash
ctest --output-on-failure --verbose
```

## Usage

### Option 1: Direct Integration (add_subdirectory)

Add this library as a subdirectory in your project:

```cmake
cmake_minimum_required(VERSION 3.15)
project(MyProject C)

# Add cbarroso as a subdirectory
add_subdirectory(path/to/cbarroso)

# Link against cbarroso
add_executable(my_app main.c)
target_link_libraries(my_app PRIVATE cbarroso::cbarroso)
```

### Option 2: Using find_package (after installation)

```cmake
cmake_minimum_required(VERSION 3.15)
project(MyProject C)

# Find the installed library
find_package(cbarroso REQUIRED)

# Link against cbarroso
add_executable(my_app main.c)
target_link_libraries(my_app PRIVATE cbarroso::cbarroso)
```

## API Reference

### HashMap

Create and manage hash maps for efficient key-value storage.

### SinglyLinkedList

Create and manage singly linked lists for generic value storage.

#### Creating a Node

```c
#include <cbarroso/sngllnkdlist.h>

SinglyLinkedListNode *node = SinglyLinkedListNode__new(value);
```

#### Inserting a Value

```c
int8_t result = SinglyLinkedListNode__insert(node, value);
// Returns 0 on success, -1 on error
```

#### Deleting the List

```c
SinglyLinkedListNode__del(node);
```

#### Complete Example

```c
#include <stdio.h>
#include <cbarroso/sngllnkdlist.h>

int main(void) {
    int a = 1, b = 2, c = 3;
    SinglyLinkedListNode *head = SinglyLinkedListNode__new(&a);
    if (!head) {
        fprintf(stderr, "Failed to create node\n");
        return 1;
    }
    SinglyLinkedListNode__insert(head, &b);
    SinglyLinkedListNode__insert(head, &c);

    // Print values
    SinglyLinkedListNode *current = head;
    while (current) {
        printf("%d\n", *(int *)current->value);
        current = current->next;
    }

    // Clean up
    SinglyLinkedListNode__del(head);
    return 0;
}
```

#### Creating a HashMap

```c
#include <cbarroso/hashmap.h>

// Create a hashmap with 2^8 = 256 slots
HashMap *map = HashMap__new(8);
```

The `log2_size` parameter determines the initial size: `size = 2^log2_size`

#### Setting Values

```c
char *key = "my_key";
int value;

int8_t result = HashMap__setItem(map, key, strlen(key), &value);
// Returns 0 on success, -1 on error
```

#### Getting Values

```c
char *key = "my_key";
void *value_ptr;

int8_t result = HashMap__getItem(map, key, strlen(key), &value_ptr);
if (result == 0) {
    int *value = (int *)value_ptr;
    printf("Value: %d\n", *value);
}
// Returns 0 on success, -1 if key not found
```

#### Complete Example

```c
#include <stdio.h>
#include <string.h>
#include <cbarroso/hashmap.h>

int main(void) {
    // Create hashmap
    HashMap *map = HashMap__new(8);
    if (!map) {
        fprintf(stderr, "Failed to create hashmap\n");
        return 1;
    }

    // Store a value
    int data = 123;
    if (HashMap__setItem(map, "key1", 4, &data) != 0) {
        fprintf(stderr, "Failed to set item\n");
        return 1;
    }

    // Retrieve the value
    void *result;
    if (HashMap__getItem(map, "key1", 4, &result) == 0) {
        printf("Retrieved: %d\n", *(int *)result);
    }

    // Clean up
    free(map);
    return 0;
}
```


## License

See LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.
