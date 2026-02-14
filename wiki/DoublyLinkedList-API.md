# DoublyLinkedList API

The DoublyLinkedList provides a linked list implementation with bidirectional traversal capabilities and individual node deletion.

## Overview

A doubly linked list is a linear data structure where each node contains a value, a pointer to the next node, and a pointer to the previous node. This allows efficient traversal in both directions and deletion of individual nodes while maintaining list integrity.

## Data Structure

```c
typedef struct DoublyLinkedListNode {
    void *value;
    struct DoublyLinkedListNode *prev;
    struct DoublyLinkedListNode *next;
} DoublyLinkedListNode;
```

## Creating a Node

```c
#include <cbarroso/dblylnkdlist.h>

DoublyLinkedListNode *DoublyLinkedListNode__new(void *value);
```

### Parameters

- `value` - Pointer to the value to store in the node

### Returns

- Pointer to the newly created node on success
- `NULL` on failure (memory allocation error)

### Example

```c
int data = 42;
DoublyLinkedListNode *node = DoublyLinkedListNode__new(&data);
if (!node) {
    fprintf(stderr, "Failed to create node\n");
    return 1;
}
```

## Inserting at Tail

```c
int8_t DoublyLinkedListNode__insertAtTail(DoublyLinkedListNode *self, void *value);
```

Inserts a new node at the end of the list, automatically setting up bidirectional links.

### Parameters

- `self` - Pointer to any node in the list (function traverses to the tail)
- `value` - Pointer to the value to store in the new node

### Returns

- `0` (CBR_SUCCESS) on success
- `-1` (CBR_ERROR) on error (memory allocation failure)

### Example

```c
int a = 1, b = 2, c = 3;
DoublyLinkedListNode *head = DoublyLinkedListNode__new(&a);
DoublyLinkedListNode__insertAtTail(head, &b);
DoublyLinkedListNode__insertAtTail(head, &c);
// List now: NULL <- 1 <-> 2 <-> 3 -> NULL
```

## Deleting a Node

```c
DoublyLinkedListNode *DoublyLinkedListNode__del(DoublyLinkedListNode *self);
```

Deletes a single node and properly maintains the links between its neighbors.

### Parameters

- `self` - Pointer to the node to delete

### Returns

- Pointer to the previous node if it exists
- Pointer to the next node if no previous node exists
- `NULL` if the node was the only node in the list

### Behavior

The function automatically:
- Updates the `next` pointer of the previous node
- Updates the `prev` pointer of the next node
- Frees the memory of the deleted node
- Returns a valid node you can continue working with

### Example

```c
// Delete middle node
DoublyLinkedListNode *middle = head->next;
DoublyLinkedListNode *result = DoublyLinkedListNode__del(middle);
// result points to the previous node (head)
// List integrity is maintained

// Delete head node
result = DoublyLinkedListNode__del(head);
// result points to the next node (new head)
// The previous head is freed

// Delete last remaining node
result = DoublyLinkedListNode__del(only_node);
// result is NULL
```

**Important**: This function only frees the node structure, not the value it points to. You are responsible for managing your data.

## Traversing the List

### Forward Traversal

```c
DoublyLinkedListNode *current = head;
while (current != NULL) {
    printf("%d ", *(int *)current->value);
    current = current->next;
}
```

### Backward Traversal

```c
// First, find the tail
DoublyLinkedListNode *tail = head;
while (tail->next != NULL) {
    tail = tail->next;
}

// Then traverse backward
DoublyLinkedListNode *current = tail;
while (current != NULL) {
    printf("%d ", *(int *)current->value);
    current = current->prev;
}
```

## Complete Example

```c
#include <stdio.h>
#include <cbarroso/dblylnkdlist.h>

int main(void) {
    // Create list with three nodes
    int a = 1, b = 2, c = 3;
    
    DoublyLinkedListNode *head = DoublyLinkedListNode__new(&a);
    if (!head) {
        fprintf(stderr, "Failed to create node\n");
        return 1;
    }
    
    DoublyLinkedListNode__insertAtTail(head, &b);
    DoublyLinkedListNode__insertAtTail(head, &c);

    // Print values forward
    printf("Forward: ");
    DoublyLinkedListNode *current = head;
    while (current) {
        printf("%d ", *(int *)current->value);
        current = current->next;
    }
    printf("\n");

    // Find tail for backward traversal
    DoublyLinkedListNode *tail = head;
    while (tail->next) {
        tail = tail->next;
    }

    // Print values backward
    printf("Backward: ");
    current = tail;
    while (current) {
        printf("%d ", *(int *)current->value);
        current = current->prev;
    }
    printf("\n");

    // Delete middle node
    DoublyLinkedListNode *middle = head->next;
    printf("Deleting node with value: %d\n", *(int *)middle->value);
    head = DoublyLinkedListNode__del(middle);
    
    // Print remaining values
    printf("After deletion: ");
    current = head;
    while (current) {
        printf("%d ", *(int *)current->value);
        current = current->next;
    }
    printf("\n");

    // Clean up remaining nodes
    current = head;
    while (current) {
        DoublyLinkedListNode *next = current->next;
        DoublyLinkedListNode__del(current);
        current = next;
    }
    
    return 0;
}
```

Expected output:
```
Forward: 1 2 3 
Backward: 3 2 1 
Deleting node with value: 2
After deletion: 1 3 
```

## Advanced Usage

### Removing All Nodes

```c
void deleteAllNodes(DoublyLinkedListNode *head) {
    DoublyLinkedListNode *current = head;
    while (current) {
        DoublyLinkedListNode *next = current->next;
        DoublyLinkedListNode__del(current);
        current = next;
    }
}
```

### Finding and Deleting by Value

```c
DoublyLinkedListNode *findAndDelete(DoublyLinkedListNode *head, int target) {
    DoublyLinkedListNode *current = head;
    
    while (current) {
        if (*(int *)current->value == target) {
            DoublyLinkedListNode *result = DoublyLinkedListNode__del(current);
            
            // If we deleted the head, return new head
            if (current == head) {
                return result; // This is the new head
            }
            return head; // Head unchanged
        }
        current = current->next;
    }
    
    return head; // Not found
}
```

### Inserting Before a Node

```c
// Manual insertion before a node
DoublyLinkedListNode *insertBefore(DoublyLinkedListNode *target, void *value) {
    DoublyLinkedListNode *new_node = DoublyLinkedListNode__new(value);
    if (!new_node) return NULL;
    
    new_node->next = target;
    new_node->prev = target->prev;
    
    if (target->prev) {
        target->prev->next = new_node;
    }
    
    target->prev = new_node;
    
    return new_node;
}
```

## Best Practices

1. **Track the head**: Always keep a reference to the head node
   ```c
   DoublyLinkedListNode *head = DoublyLinkedListNode__new(&value);
   ```

2. **Handle deletion return values**: Update your head pointer when deleting the head node
   ```c
   if (to_delete == head) {
       head = DoublyLinkedListNode__del(head);
   } else {
       DoublyLinkedListNode__del(to_delete);
   }
   ```

3. **Value lifetime**: Ensure values remain valid while in the list
   ```c
   // Good - heap-allocated data
   int *value = malloc(sizeof(int));
   *value = 42;
   DoublyLinkedListNode *node = DoublyLinkedListNode__new(value);
   
   // Bad - stack variable
   int temp = 42;
   DoublyLinkedListNode *node = DoublyLinkedListNode__new(&temp); // Dangerous!
   ```

4. **NULL checks**: Always check node creation
   ```c
   DoublyLinkedListNode *node = DoublyLinkedListNode__new(&value);
   if (!node) {
       // Handle allocation failure
   }
   ```

5. **Clean traversal**: Check for NULL when traversing
   ```c
   while (current != NULL) {
       // Process node
       current = current->next;
   }
   ```

## Performance Characteristics

- **Insert at tail**: O(n) - must traverse to find tail
- **Delete single node**: O(1) - with direct node reference
- **Forward/backward traversal**: O(n)
- **Memory**: O(n) - two pointers per node vs one in singly linked list

## Advantages Over SinglyLinkedList

1. **Bidirectional traversal**: Can move backward through the list
2. **Efficient deletion**: Delete any node with O(1) when you have its reference
3. **Easier manipulation**: Insert before/after, remove from middle more naturally

## Limitations

- **More memory**: Uses 2 pointers per node (prev + next) vs 1 for singly linked list
- **No tail reference**: insertAtTail still requires O(n) traversal
- **Manual management**: No built-in list head structure with size tracking

## See Also

- [SinglyLinkedList API](SinglyLinkedList-API.md) - Simpler, forward-only list
- [HashMap API](HashMap-API.md)
- [API Overview](API-Overview.md)
