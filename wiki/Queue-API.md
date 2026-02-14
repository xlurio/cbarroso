# Queue API

The Queue provides a First-In-First-Out (FIFO) data structure for managing elements.

## Overview

The Queue implementation uses a singly-linked list internally with head and tail pointers, allowing efficient O(1) enqueue and dequeue operations. It stores generic void pointers as values with deep copies, enabling storage of any data type.

## Creating a Queue

```c
#include <cbarroso/queue.h>

Queue *queue = Queue__new();
```

### Returns

- Pointer to the newly created Queue on success
- `NULL` on failure (memory allocation error)

### Example

```c
Queue *queue = Queue__new();
if (!queue) {
    fprintf(stderr, "Failed to create queue\n");
    return 1;
}
```

## Enqueuing Values

```c
int8_t Queue__enqueue(Queue *self, void *value, size_t valueSize);
```

### Parameters

- `self` - Pointer to the Queue
- `value` - Pointer to the value to enqueue
- `valueSize` - Size of the value in bytes

### Returns

- `0` (CBR_SUCCESS) on success
- `-1` (CBR_ERROR) on error (memory allocation failure)

### Example

```c
int value = 42;
int8_t result = Queue__enqueue(queue, &value, sizeof(int));
if (result != 0) {
    fprintf(stderr, "Failed to enqueue value\n");
}
```

### Behavior

- The value is copied into the queue (deep copy)
- The new value is added to the tail of the queue
- Queue size is incremented
- Head and tail pointers are updated automatically

## Dequeuing Values

```c
int8_t Queue__dequeue(Queue *self, void **valueAddress);
```

### Parameters

- `self` - Pointer to the Queue
- `valueAddress` - Pointer to a void pointer that will receive the dequeued value

### Returns

- `0` (CBR_SUCCESS) on success
- `-1` (CBR_ERROR) if the queue is empty

### Example

```c
void *value_ptr;
int8_t result = Queue__dequeue(queue, &value_ptr);
if (result == 0) {
    int *value = (int *)value_ptr;
    printf("Dequeued value: %d\n", *value);
    
    // Important: You must free the returned value
    free(value_ptr);
} else {
    printf("Queue is empty\n");
}
```

### Behavior

- Returns a pointer to the head value
- Removes the head node from the queue
- Queue size is decremented
- Updates head pointer to next node
- If queue becomes empty, both head and tail are set to NULL
- **Important**: The caller is responsible for freeing the returned value

## Checking Queue Size

The queue maintains a `numberOfNodes` field that tracks the number of elements:

```c
size_t size = queue->numberOfNodes;
printf("Queue has %zu elements\n", size);
```

## Checking if Queue is Empty

```c
if (queue->numberOfNodes == 0) {
    printf("Queue is empty\n");
} else {
    printf("Queue has elements\n");
}
```

## Destroying a Queue

Currently, there is no dedicated destructor function. To properly clean up a queue:

```c
// Dequeue and free all elements
while (queue->numberOfNodes > 0) {
    void *value;
    Queue__dequeue(queue, &value);
    free(value);
}

// Free the queue structure
free(queue);
```

### Example

```c
// Clean up queue
while (queue->numberOfNodes > 0) {
    void *value;
    Queue__dequeue(queue, &value);
    free(value);
}
free(queue);
queue = NULL;
```

## Complete Example

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cbarroso/queue.h>

int main(void) {
    // Create a new queue
    Queue *queue = Queue__new();
    if (!queue) {
        fprintf(stderr, "Failed to create queue\n");
        return 1;
    }

    // Enqueue some integers
    for (int i = 1; i <= 5; i++) {
        if (Queue__enqueue(queue, &i, sizeof(int)) != 0) {
            fprintf(stderr, "Failed to enqueue value %d\n", i);
        }
    }

    printf("Queue size: %zu\n", queue->numberOfNodes);

    // Dequeue and print all values
    while (queue->numberOfNodes > 0) {
        void *value_ptr;
        if (Queue__dequeue(queue, &value_ptr) == 0) {
            int value = *(int *)value_ptr;
            printf("Dequeued: %d\n", value);
            free(value_ptr);
        }
    }

    // Clean up
    free(queue);
    return 0;
}
```

## Working with Different Data Types

### Strings

```c
Queue *queue = Queue__new();

char *str1 = "Hello";
char *str2 = "World";

Queue__enqueue(queue, str1, strlen(str1) + 1);
Queue__enqueue(queue, str2, strlen(str2) + 1);

void *dequeued;
Queue__dequeue(queue, &dequeued);
printf("String: %s\n", (char *)dequeued);
free(dequeued);

// Clean up remaining elements...
```

### Structs

```c
typedef struct {
    int id;
    char name[50];
    double salary;
} Employee;

Queue *queue = Queue__new();

Employee emp1 = {1, "Alice", 75000.0};
Employee emp2 = {2, "Bob", 82000.0};

Queue__enqueue(queue, &emp1, sizeof(Employee));
Queue__enqueue(queue, &emp2, sizeof(Employee));

void *dequeued;
Queue__dequeue(queue, &dequeued);
Employee *emp = (Employee *)dequeued;
printf("Employee: %d - %s - $%.2f\n", emp->id, emp->name, emp->salary);
free(dequeued);

// Clean up remaining elements...
```

### Pointers

```c
Queue *queue = Queue__new();

int *data = malloc(sizeof(int));
*data = 42;

// Store the pointer value itself
Queue__enqueue(queue, &data, sizeof(int *));

void *dequeued;
Queue__dequeue(queue, &dequeued);
int **ptr = (int **)dequeued;
printf("Value: %d\n", **ptr);

// Free both the dequeued memory and the original data
free(*ptr);  // Free the allocated integer
free(dequeued);  // Free the queue node memory
```

## Common Use Cases

### Task Queue

```c
typedef struct {
    char task_name[100];
    int priority;
} Task;

Queue *task_queue = Queue__new();

// Add tasks
Task t1 = {"Process file", 1};
Task t2 = {"Send email", 2};
Queue__enqueue(task_queue, &t1, sizeof(Task));
Queue__enqueue(task_queue, &t2, sizeof(Task));

// Process tasks in FIFO order
while (task_queue->numberOfNodes > 0) {
    void *task_ptr;
    Queue__dequeue(task_queue, &task_ptr);
    Task *task = (Task *)task_ptr;
    printf("Processing: %s (priority: %d)\n", task->task_name, task->priority);
    free(task_ptr);
}

free(task_queue);
```

### Message Buffer

```c
typedef struct {
    char sender[50];
    char message[200];
    time_t timestamp;
} Message;

Queue *message_queue = Queue__new();

// Add messages
Message msg = {"user@example.com", "Hello World", time(NULL)};
Queue__enqueue(message_queue, &msg, sizeof(Message));

// Process messages
void *msg_ptr;
if (Queue__dequeue(message_queue, &msg_ptr) == 0) {
    Message *m = (Message *)msg_ptr;
    printf("From: %s\nMessage: %s\n", m->sender, m->message);
    free(msg_ptr);
}

free(message_queue);
```

## Performance Characteristics

- **Enqueue**: O(1) - constant time
- **Dequeue**: O(1) - constant time
- **Space**: O(n) - linear space for n elements
- **Memory overhead**: One pointer per node (next pointer)

## Best Practices

1. **Always check return values** from enqueue and dequeue operations
2. **Free dequeued values** - the caller is responsible for memory management
3. **Check if queue is empty** before dequeuing to avoid errors
4. **Clean up properly** - dequeue all elements and free the queue when done
5. **Use appropriate value sizes** - ensure sizeof() matches the actual data type

## Memory Management

- Queue creates deep copies of values during enqueue
- Dequeued values must be freed by the caller
- The queue node itself is freed during dequeue
- When finished, dequeue all elements and free the queue structure

## Thread Safety

This queue implementation is **not thread-safe**. If you need to use it in a multi-threaded environment:

- Use mutexes to protect enqueue and dequeue operations
- Consider implementing a lock-free queue for better performance
- Ensure proper synchronization when accessing queue size

## Limitations

- No built-in destructor function (must manually clean up)
- Not thread-safe
- No built-in capacity limits
- No peek operation (must dequeue to access values)
