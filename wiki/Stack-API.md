# Stack API

The Stack provides a Last-In-First-Out (LIFO) data structure for managing elements.

## Overview

The Stack implementation uses a singly-linked list internally, allowing efficient push and pop operations at the top. It stores generic void pointers as values, allowing storage of any data type.

## Creating a Stack

```c
#include <cbarroso/stack.h>

Stack *stack = Stack__new();
```

### Returns

- Pointer to the newly created Stack on success
- `NULL` on failure (memory allocation error)

### Example

```c
Stack *stack = Stack__new();
if (!stack) {
    fprintf(stderr, "Failed to create stack\n");
    return 1;
}
```

## Pushing Values

```c
int8_t Stack__push(Stack *self, void *value, size_t valueSize);
```

### Parameters

- `self` - Pointer to the Stack
- `value` - Pointer to the value to push
- `valueSize` - Size of the value in bytes

### Returns

- `0` (CBR_SUCCESS) on success
- `-1` (CBR_ERROR) on error (memory allocation failure)

### Example

```c
int value = 42;
int8_t result = Stack__push(stack, &value, sizeof(int));
if (result != 0) {
    fprintf(stderr, "Failed to push value\n");
}
```

### Behavior

- The value is copied into the stack (deep copy)
- The new value becomes the top of the stack
- Stack size is incremented

## Popping Values

```c
int8_t Stack__pop(Stack *self, void **valueAddress);
```

### Parameters

- `self` - Pointer to the Stack
- `valueAddress` - Pointer to a void pointer that will receive the popped value

### Returns

- `0` (CBR_SUCCESS) on success
- `-1` (CBR_ERROR) if the stack is empty

### Example

```c
void *value_ptr;
int8_t result = Stack__pop(stack, &value_ptr);
if (result == 0) {
    int *value = (int *)value_ptr;
    printf("Popped value: %d\n", *value);
    
    // Important: You must free the returned value
    free(value_ptr);
} else {
    printf("Stack is empty\n");
}
```

### Behavior

- Returns a pointer to the top value
- Removes the top node from the stack
- Stack size is decremented
- **Important**: The caller is responsible for freeing the returned value

## Checking Stack Size

The stack maintains a `stackSize` field that tracks the number of elements:

```c
size_t size = stack->stackSize;
printf("Stack has %zu elements\n", size);
```

## Checking if Stack is Empty

```c
if (stack->stackSize == 0) {
    printf("Stack is empty\n");
} else {
    printf("Stack has elements\n");
}
```

## Destroying a Stack

```c
Stack *Stack__del(Stack *self);
```

### Parameters

- `self` - Pointer to the Stack to destroy

### Returns

- Always returns `NULL`

### Example

```c
stack = Stack__del(stack);
```

### Behavior

- Pops and frees all remaining values in the stack
- Frees all internal nodes
- Frees the stack structure itself
- Returns `NULL` (allows pattern: `stack = Stack__del(stack)`)

## Complete Example

```c
#include <cbarroso/stack.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    // Create a new stack
    Stack *stack = Stack__new();
    if (!stack) {
        fprintf(stderr, "Failed to create stack\n");
        return 1;
    }
    
    // Push multiple integers
    for (int i = 1; i <= 5; i++) {
        if (Stack__push(stack, &i, sizeof(int)) != 0) {
            fprintf(stderr, "Failed to push value %d\n", i);
            Stack__del(stack);
            return 1;
        }
    }
    
    printf("Stack size: %zu\n", stack->stackSize);  // Output: 5
    
    // Pop and print all values (LIFO order: 5, 4, 3, 2, 1)
    while (stack->stackSize > 0) {
        void *value_ptr;
        if (Stack__pop(stack, &value_ptr) == 0) {
            int value = *(int *)value_ptr;
            printf("Popped: %d\n", value);
            free(value_ptr);
        }
    }
    
    // Clean up
    stack = Stack__del(stack);
    
    return 0;
}
```

## Working with Strings

```c
Stack *stack = Stack__new();

// Push strings
char *str1 = "Hello";
char *str2 = "World";

Stack__push(stack, str1, strlen(str1) + 1);
Stack__push(stack, str2, strlen(str2) + 1);

// Pop strings
void *value_ptr;
if (Stack__pop(stack, &value_ptr) == 0) {
    printf("Popped: %s\n", (char *)value_ptr);  // Output: "World"
    free(value_ptr);
}

if (Stack__pop(stack, &value_ptr) == 0) {
    printf("Popped: %s\n", (char *)value_ptr);  // Output: "Hello"
    free(value_ptr);
}

stack = Stack__del(stack);
```

## Working with Structures

```c
typedef struct {
    int id;
    char name[50];
} Person;

Stack *stack = Stack__new();

Person p1 = {1, "Alice"};
Person p2 = {2, "Bob"};

Stack__push(stack, &p1, sizeof(Person));
Stack__push(stack, &p2, sizeof(Person));

void *value_ptr;
if (Stack__pop(stack, &value_ptr) == 0) {
    Person *person = (Person *)value_ptr;
    printf("ID: %d, Name: %s\n", person->id, person->name);
    free(value_ptr);
}

stack = Stack__del(stack);
```

## Performance Characteristics

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Stack__new() | O(1) | O(1) |
| Stack__push() | O(1) | O(n) for value copy |
| Stack__pop() | O(1) | O(1) |
| Stack__del() | O(n) | O(1) |

## Memory Management

- **Push**: Creates a deep copy of the value (allocates memory)
- **Pop**: Returns a pointer to the value; **caller must free it**
- **Delete**: Automatically frees all remaining values and nodes

## Common Patterns

### Expression Evaluation

```c
Stack *stack = Stack__new();

// Push operands and operators
// Pop to evaluate expressions

stack = Stack__del(stack);
```

### Undo/Redo Functionality

```c
Stack *undo_stack = Stack__new();
Stack *redo_stack = Stack__new();

// Push actions onto undo_stack
// When undoing, pop from undo_stack and push to redo_stack
// When redoing, pop from redo_stack and push to undo_stack

undo_stack = Stack__del(undo_stack);
redo_stack = Stack__del(redo_stack);
```

### Depth-First Search (DFS)

```c
Stack *stack = Stack__new();

// Push root node
// While stack not empty:
//   Pop node
//   Process node
//   Push children

stack = Stack__del(stack);
```

## Error Handling

All functions that can fail return status codes:

```c
Stack *stack = Stack__new();
if (!stack) {
    // Handle allocation failure
}

int value = 42;
if (Stack__push(stack, &value, sizeof(int)) != 0) {
    // Handle push failure
}

void *popped;
if (Stack__pop(stack, &popped) != 0) {
    // Handle empty stack
} else {
    // Use value
    free(popped);
}
```

## Important Notes

1. **Memory Ownership**: After `Stack__pop()`, the caller owns the returned memory and must `free()` it
2. **Deep Copy**: `Stack__push()` creates a copy of the value, so the original can be safely modified or freed
3. **Type Safety**: Since values are stored as `void*`, casting is required when retrieving values
4. **Size Tracking**: The `stackSize` field always reflects the current number of elements
5. **Empty Stack**: Attempting to pop from an empty stack returns an error and prints to stderr
