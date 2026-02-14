# cbarroso

A C library providing a high-performance data structures implementation.

## Building

### Prerequisites

- CMake 3.10 or higher
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
cmake --build build

# Run all tests with CTest
cd build
ctest --output-on-failure
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

## Features

The library provides three high-performance data structures:

- **HashMap** - Fast key-value storage with O(1) lookups
- **SinglyLinkedList** - Simple forward-only linked list
- **DoublyLinkedList** - Bidirectional linked list with efficient node deletion

## Documentation

For detailed API documentation, examples, and usage guides, see the [wiki](wiki/API-Overview.md):

- **[API Overview](wiki/API-Overview.md)** - Complete guide to all data structures
- **[HashMap API](wiki/HashMap-API.md)** - Hash map documentation and examples
- **[SinglyLinkedList API](wiki/SinglyLinkedList-API.md)** - Singly linked list documentation
- **[DoublyLinkedList API](wiki/DoublyLinkedList-API.md)** - Doubly linked list documentation

## Quick Example

```c
#include <cbarroso/hashmap.h>

int main(void) {
    HashMap *map = HashMap__new(8);
    
    int value = 42;
    HashMap__setItem(map, "answer", 6, &value);
    
    void *result;
    HashMap__getItem(map, "answer", 6, &result);
    printf("The answer is %d\n", *(int *)result);
    
    free(map);
    return 0;
}
```


## License

See LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.
