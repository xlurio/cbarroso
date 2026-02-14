#include <cbarroso/queue.h>
#include "test_utils.h"

// Test: Create a new Queue
TEST(test_queue_new)
{
    Queue *queue = Queue__new();
    ASSERT_NOT_NULL(queue, "Queue should not be NULL");
    ASSERT_EQ(queue->numberOfNodes, 0, "Queue should be empty");
    ASSERT(queue->head == NULL, "Queue head should be NULL");
    ASSERT(queue->tail == NULL, "Queue tail should be NULL");
    free(queue);
}

// Test: Enqueue single integer
TEST(test_queue_enqueue_single_int)
{
    Queue *queue = Queue__new();
    ASSERT_NOT_NULL(queue, "Queue should not be NULL");

    int value = 42;
    int8_t result = Queue__enqueue(queue, &value, sizeof(int));
    ASSERT_EQ(result, 0, "Enqueue should return 0 on success");
    ASSERT_EQ(queue->numberOfNodes, 1, "Queue size should be 1");
    ASSERT_NOT_NULL(queue->head, "Queue head should not be NULL");
    ASSERT_NOT_NULL(queue->tail, "Queue tail should not be NULL");
    ASSERT(queue->head == queue->tail, "Head and tail should point to same node");

    free(queue->head->value);
    free(queue->head);
    free(queue);
}

// Test: Enqueue multiple integers
TEST(test_queue_enqueue_multiple_ints)
{
    Queue *queue = Queue__new();
    ASSERT_NOT_NULL(queue, "Queue should not be NULL");

    for (int i = 0; i < 10; i++)
    {
        int8_t result = Queue__enqueue(queue, &i, sizeof(int));
        ASSERT_EQ(result, 0, "Enqueue should succeed");
        ASSERT_EQ(queue->numberOfNodes, (size_t)(i + 1), "Queue size should increment");
    }

    ASSERT_EQ(queue->numberOfNodes, 10, "Queue size should be 10");
    ASSERT_NOT_NULL(queue->head, "Queue head should not be NULL");
    ASSERT_NOT_NULL(queue->tail, "Queue tail should not be NULL");

    // Clean up
    while (queue->numberOfNodes > 0)
    {
        void *value;
        Queue__dequeue(queue, &value);
        free(value);
    }
    free(queue);
}

// Test: Dequeue single value
TEST(test_queue_dequeue_single)
{
    Queue *queue = Queue__new();
    ASSERT_NOT_NULL(queue, "Queue should not be NULL");

    int value = 42;
    Queue__enqueue(queue, &value, sizeof(int));

    void *dequeued;
    int8_t result = Queue__dequeue(queue, &dequeued);
    ASSERT_EQ(result, 0, "Dequeue should return 0 on success");
    ASSERT_NOT_NULL(dequeued, "Dequeued value should not be NULL");
    ASSERT_EQ(*(int *)dequeued, 42, "Dequeued value should be 42");
    ASSERT_EQ(queue->numberOfNodes, 0, "Queue size should be 0 after dequeue");
    ASSERT(queue->head == NULL, "Queue head should be NULL when empty");
    ASSERT(queue->tail == NULL, "Queue tail should be NULL when empty");

    free(dequeued);
    free(queue);
}

// Test: Dequeue empty queue
TEST(test_queue_dequeue_empty)
{
    Queue *queue = Queue__new();
    ASSERT_NOT_NULL(queue, "Queue should not be NULL");

    void *dequeued;
    int8_t result = Queue__dequeue(queue, &dequeued);
    ASSERT_EQ(result, -1, "Dequeue on empty queue should return -1");

    free(queue);
}

// Test: FIFO order
TEST(test_queue_fifo_order)
{
    Queue *queue = Queue__new();
    ASSERT_NOT_NULL(queue, "Queue should not be NULL");

    // Enqueue 1, 2, 3, 4, 5
    for (int i = 1; i <= 5; i++)
    {
        Queue__enqueue(queue, &i, sizeof(int));
    }

    // Should dequeue in same order: 1, 2, 3, 4, 5
    for (int i = 1; i <= 5; i++)
    {
        void *dequeued;
        int8_t result = Queue__dequeue(queue, &dequeued);
        ASSERT_EQ(result, 0, "Dequeue should succeed");
        ASSERT_EQ(*(int *)dequeued, i, "Value should match FIFO order");
        free(dequeued);
    }

    ASSERT_EQ(queue->numberOfNodes, 0, "Queue should be empty");
    free(queue);
}

// Test: Enqueue strings
TEST(test_queue_enqueue_strings)
{
    Queue *queue = Queue__new();
    ASSERT_NOT_NULL(queue, "Queue should not be NULL");

    char *str1 = "Hello";
    char *str2 = "World";
    
    Queue__enqueue(queue, str1, strlen(str1) + 1);
    Queue__enqueue(queue, str2, strlen(str2) + 1);
    
    ASSERT_EQ(queue->numberOfNodes, 2, "Queue should have 2 elements");

    void *dequeued1;
    Queue__dequeue(queue, &dequeued1);
    ASSERT_STR_EQ((char *)dequeued1, "Hello", "First dequeued should be 'Hello'");
    free(dequeued1);

    void *dequeued2;
    Queue__dequeue(queue, &dequeued2);
    ASSERT_STR_EQ((char *)dequeued2, "World", "Second dequeued should be 'World'");
    free(dequeued2);

    free(queue);
}

// Test: Enqueue structs
TEST(test_queue_enqueue_structs)
{
    Queue *queue = Queue__new();
    ASSERT_NOT_NULL(queue, "Queue should not be NULL");

    typedef struct {
        int id;
        char name[20];
    } Person;

    Person p1 = {1, "Alice"};
    Person p2 = {2, "Bob"};

    Queue__enqueue(queue, &p1, sizeof(Person));
    Queue__enqueue(queue, &p2, sizeof(Person));

    void *dequeued;
    Queue__dequeue(queue, &dequeued);
    Person *person1 = (Person *)dequeued;
    ASSERT_EQ(person1->id, 1, "First person ID should be 1");
    ASSERT_STR_EQ(person1->name, "Alice", "First person name should be Alice");
    free(dequeued);

    Queue__dequeue(queue, &dequeued);
    Person *person2 = (Person *)dequeued;
    ASSERT_EQ(person2->id, 2, "Second person ID should be 2");
    ASSERT_STR_EQ(person2->name, "Bob", "Second person name should be Bob");
    free(dequeued);

    free(queue);
}

// Test: Mixed operations
TEST(test_queue_mixed_operations)
{
    Queue *queue = Queue__new();
    ASSERT_NOT_NULL(queue, "Queue should not be NULL");

    // Enqueue 1, 2, 3
    for (int i = 1; i <= 3; i++)
    {
        Queue__enqueue(queue, &i, sizeof(int));
    }

    // Dequeue 1
    void *dequeued;
    Queue__dequeue(queue, &dequeued);
    ASSERT_EQ(*(int *)dequeued, 1, "Should dequeue 1");
    free(dequeued);

    // Enqueue 4, 5
    for (int i = 4; i <= 5; i++)
    {
        Queue__enqueue(queue, &i, sizeof(int));
    }

    // Dequeue remaining: 2, 3, 4, 5
    for (int i = 2; i <= 5; i++)
    {
        Queue__dequeue(queue, &dequeued);
        ASSERT_EQ(*(int *)dequeued, i, "Value should match expected order");
        free(dequeued);
    }

    ASSERT_EQ(queue->numberOfNodes, 0, "Queue should be empty");
    free(queue);
}

// Test: Large queue
TEST(test_queue_large)
{
    Queue *queue = Queue__new();
    ASSERT_NOT_NULL(queue, "Queue should not be NULL");

    const int count = 1000;

    // Enqueue many values
    for (int i = 0; i < count; i++)
    {
        int8_t result = Queue__enqueue(queue, &i, sizeof(int));
        ASSERT_EQ(result, 0, "Enqueue should succeed");
    }

    ASSERT_EQ(queue->numberOfNodes, count, "Queue should have all elements");

    // Dequeue all values
    for (int i = 0; i < count; i++)
    {
        void *dequeued;
        int8_t result = Queue__dequeue(queue, &dequeued);
        ASSERT_EQ(result, 0, "Dequeue should succeed");
        ASSERT_EQ(*(int *)dequeued, i, "Value should match insertion order");
        free(dequeued);
    }

    ASSERT_EQ(queue->numberOfNodes, 0, "Queue should be empty");
    free(queue);
}

// Test: Stress test - alternating enqueue/dequeue
TEST(test_queue_alternating_operations)
{
    Queue *queue = Queue__new();
    ASSERT_NOT_NULL(queue, "Queue should not be NULL");

    for (int i = 0; i < 100; i++)
    {
        // Enqueue two values
        Queue__enqueue(queue, &i, sizeof(int));
        int next = i + 1;
        Queue__enqueue(queue, &next, sizeof(int));

        // Dequeue one value
        void *dequeued;
        Queue__dequeue(queue, &dequeued);
        free(dequeued);
    }

    ASSERT_EQ(queue->numberOfNodes, 100, "Queue should have 100 elements");

    // Clean up
    while (queue->numberOfNodes > 0)
    {
        void *value;
        Queue__dequeue(queue, &value);
        free(value);
    }
    free(queue);
}

int main(void)
{
    printf("=== Queue Test Suite ===\n\n");
    RUN_TEST(test_queue_new);
    RUN_TEST(test_queue_enqueue_single_int);
    RUN_TEST(test_queue_enqueue_multiple_ints);
    RUN_TEST(test_queue_dequeue_single);
    RUN_TEST(test_queue_dequeue_empty);
    RUN_TEST(test_queue_fifo_order);
    RUN_TEST(test_queue_enqueue_strings);
    RUN_TEST(test_queue_enqueue_structs);
    RUN_TEST(test_queue_mixed_operations);
    RUN_TEST(test_queue_large);
    RUN_TEST(test_queue_alternating_operations);

    printf("\n=== Test Results ===\n");
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);

    if (tests_passed == tests_run)
    {
        printf("\n\u2713 All tests passed!\n");
        return 0;
    }
    else
    {
        printf("\n\u2717 Some tests failed!\n");
        return 1;
    }
}
