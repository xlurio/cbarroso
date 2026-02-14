# SinglyLinkedList API

The SinglyLinkedList provides a simple linked list implementation for storing generic values with forward traversal.

## Overview

A singly linked list is a linear data structure where each node contains a value and a pointer to the next node. It allows O(1) insertion at the tail (when you have the tail reference) and O(n) traversal.

## Data Structure

```c
typedef struct SinglyLinkedListNode {
    void *value;
    struct SinglyLinkedListNode *next;
} SinglyLinkedListNode;
```

## Creating a Node

```c
#include <cbarroso/sngllnkdlist.h>

SinglyLinkedListNode *SinglyLinkedListNode__new(void *value);
```

### Parameters

- `value` - Pointer to the value to store in the node

### Returns

- Pointer to the newly created node on success
- `NULL` on failure (memory allocation error)

### Example

```c
int data = 42;
SinglyLinkedListNode *node = SinglyLinkedListNode__new(&data);
if (!node) {
    fprintf(stderr, "Failed to create node\n");
    return 1;
}
```

## Inserting at Tail

```c
int8_t SinglyLinkedListNode__insertAtTail(SinglyLinkedListNode *self, void *value);
```

Inserts a new node at the end of the list.

### Parameters

- `self` - Pointer to any node in the list (function traverses to the tail)
- `value` - Pointer to the value to store in the new node

### Returns

- `0` (CBR_SUCCESS) on success
- `-1` (CBR_ERROR) on error (memory allocation failure)

### Example

```c
int a = 1, b = 2, c = 3;
SinglyLinkedListNode *head = SinglyLinkedListNode__new(&a);
SinglyLinkedListNode__insertAtTail(head, &b);
SinglyLinkedListNode__insertAtTail(head, &c);
// List now contains: 1 -> 2 -> 3
```

## Deleting the List

```c
void SinglyLinkedListNode__del(SinglyLinkedListNode *self);
```

Deletes the entire list starting from the given node, freeing all nodes forward.

### Parameters

- `self` - Pointer to the head node of the list to delete

### Returns

- None (void)

### Example

```c
SinglyLinkedListNode__del(head);
// All nodes from head onwards are freed
```

**Important**: This function frees all nodes in the list but does not free the values stored in them. You are responsible for managing the lifetime of your data.

## Traversing the List

To traverse a singly linked list, use the `next` pointer:

```c
SinglyLinkedListNode *current = head;
while (current != NULL) {
    // Access current->value
    printf("%d\n", *(int *)current->value);
    current = current->next;
}
```

## Complete Example

```c
#include <stdio.h>
#include <cbarroso/sngllnkdlist.h>

int main(void) {
    // Create list with three nodes
    int a = 1, b = 2, c = 3;
    
    SinglyLinkedListNode *head = SinglyLinkedListNode__new(&a);
    if (!head) {
        fprintf(stderr, "Failed to create node\n");
        return 1;
    }
    
    SinglyLinkedListNode__insertAtTail(head, &b);
    SinglyLinkedListNode__insertAtTail(head, &c);

    // Print all values
    printf("List contents: ");
    SinglyLinkedListNode *current = head;
    while (current) {
        printf("%d ", *(int *)current->value);
        current = current->next;
    }
    printf("\n");

    // Find a specific value
    current = head;
    int search = 2;
    while (current) {
        if (*(int *)current->value == search) {
            printf("Found value: %d\n", search);
            break;
        }
        current = current->next;
    }

    // Clean up - frees all nodes
    SinglyLinkedListNode__del(head);
    
    return 0;
}
```

## Working with Different Types

The list stores `void *` pointers, so it can hold any type:

### With Strings

```c
char *str1 = "Hello";
char *str2 = "World";

SinglyLinkedListNode *head = SinglyLinkedListNode__new(str1);
SinglyLinkedListNode__insertAtTail(head, str2);

SinglyLinkedListNode *current = head;
while (current) {
    printf("%s\n", (char *)current->value);
    current = current->next;
}

SinglyLinkedListNode__del(head);
```

### With Structs

```c
typedef struct {
    int id;
    char name[50];
} Person;

Person p1 = {1, "Alice"};
Person p2 = {2, "Bob"};

SinglyLinkedListNode *head = SinglyLinkedListNode__new(&p1);
SinglyLinkedListNode__insertAtTail(head, &p2);

SinglyLinkedListNode *current = head;
while (current) {
    Person *p = (Person *)current->value;
    printf("ID: %d, Name: %s\n", p->id, p->name);
    current = current->next;
}

SinglyLinkedListNode__del(head);
```

## Best Practices

1. **Keep track of the head**: Always maintain a pointer to the head node

2. **Check for NULL**: Always verify node creation succeeded
   ```c
   SinglyLinkedListNode *node = SinglyLinkedListNode__new(&value);
   if (!node) {
       // Handle error
   }
   ```

3. **Value lifetime**: Ensure values remain valid while the list exists
   ```c
   // Good - persistent data
   int *value = malloc(sizeof(int));
   *value = 42;
   SinglyLinkedListNode *node = SinglyLinkedListNode__new(value);
   
   // Bad - temporary data
   int temp = 42;
   SinglyLinkedListNode *node = SinglyLinkedListNode__new(&temp); // temp goes out of scope!
   ```

4. **Clean up properly**: Call `SinglyLinkedListNode__del()` on the head when done

5. **Type consistency**: Use the same type throughout the list for type safety

## Performance Characteristics

- **Insert at tail**: O(n) - must traverse to find tail
- **Delete entire list**: O(n) - must visit each node
- **Traversal**: O(n)
- **Memory**: O(n) - one node per element

## Limitations

- No built-in function to delete a single node
- No reverse traversal capability
- Must traverse from head to reach any position
- No built-in search functionality

For bidirectional traversal and single-node deletion, see [DoublyLinkedList API](DoublyLinkedList-API.md).

## See Also

- [DoublyLinkedList API](DoublyLinkedList-API.md) - For bidirectional traversal
- [HashMap API](HashMap-API.md)
- [API Overview](API-Overview.md)
