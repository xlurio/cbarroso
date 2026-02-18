#include <cbarroso/stack.h>
#include <ccauchy.h>

// Test: Create a new Stack
TEST(test_stack_new)
{
    Stack *stack = Stack__new();
    ASSERT_NOT_NULL(stack, "Stack should not be NULL");
    ASSERT_EQ(stack->stackSize, 0, "Stack size should be 0");
    ASSERT(stack->top == NULL, "Stack top should be NULL");
    stack = Stack__del(stack);
}

// Test: Push single integer
TEST(test_stack_push_single_int)
{
    Stack *stack = Stack__new();
    ASSERT_NOT_NULL(stack, "Stack should not be NULL");

    int value = 42;
    int8_t result = Stack__push(stack, &value, sizeof(int));
    ASSERT_EQ(result, 0, "Push should return 0 on success");
    ASSERT_EQ(stack->stackSize, 1, "Stack size should be 1");
    ASSERT_NOT_NULL(stack->top, "Stack top should not be NULL");

    stack = Stack__del(stack);
}

// Test: Push multiple integers
TEST(test_stack_push_multiple_ints)
{
    Stack *stack = Stack__new();
    ASSERT_NOT_NULL(stack, "Stack should not be NULL");

    for (int i = 0; i < 10; i++)
    {
        int8_t result = Stack__push(stack, &i, sizeof(int));
        ASSERT_EQ(result, 0, "Push should succeed");
        ASSERT_EQ(stack->stackSize, (size_t)(i + 1), "Stack size should increment");
    }

    ASSERT_EQ(stack->stackSize, 10, "Stack size should be 10");
    stack = Stack__del(stack);
}

// Test: Pop single value
TEST(test_stack_pop_single)
{
    Stack *stack = Stack__new();
    ASSERT_NOT_NULL(stack, "Stack should not be NULL");

    int value = 42;
    Stack__push(stack, &value, sizeof(int));

    void *popped;
    int8_t result = Stack__pop(stack, &popped);
    ASSERT_EQ(result, 0, "Pop should return 0 on success");
    ASSERT_NOT_NULL(popped, "Popped value should not be NULL");
    ASSERT_EQ(*(int *)popped, 42, "Popped value should be 42");
    ASSERT_EQ(stack->stackSize, 0, "Stack size should be 0 after pop");

    free(popped);
    stack = Stack__del(stack);
}

// Test: Pop empty stack
TEST(test_stack_pop_empty)
{
    Stack *stack = Stack__new();
    ASSERT_NOT_NULL(stack, "Stack should not be NULL");

    void *popped;
    int8_t result = Stack__pop(stack, &popped);
    ASSERT_EQ(result, -1, "Pop on empty stack should return -1");

    stack = Stack__del(stack);
}

// Test: LIFO order
TEST(test_stack_lifo_order)
{
    Stack *stack = Stack__new();
    ASSERT_NOT_NULL(stack, "Stack should not be NULL");

    // Push 1, 2, 3, 4, 5
    for (int i = 1; i <= 5; i++)
    {
        Stack__push(stack, &i, sizeof(int));
    }

    // Should pop in reverse order: 5, 4, 3, 2, 1
    for (int i = 5; i >= 1; i--)
    {
        void *popped;
        int8_t result = Stack__pop(stack, &popped);
        ASSERT_EQ(result, 0, "Pop should succeed");
        ASSERT_EQ(*(int *)popped, i, "Value should match LIFO order");
        free(popped);
    }

    ASSERT_EQ(stack->stackSize, 0, "Stack should be empty");
    stack = Stack__del(stack);
}

// Test: Push and pop strings
TEST(test_stack_strings)
{
    Stack *stack = Stack__new();
    ASSERT_NOT_NULL(stack, "Stack should not be NULL");

    char *str1 = "Hello";
    char *str2 = "World";
    char *str3 = "Test";

    Stack__push(stack, str1, strlen(str1) + 1);
    Stack__push(stack, str2, strlen(str2) + 1);
    Stack__push(stack, str3, strlen(str3) + 1);

    ASSERT_EQ(stack->stackSize, 3, "Stack should have 3 elements");

    void *popped;

    Stack__pop(stack, &popped);
    ASSERT_STR_EQ((char *)popped, "Test", "Should pop 'Test'");
    free(popped);

    Stack__pop(stack, &popped);
    ASSERT_STR_EQ((char *)popped, "World", "Should pop 'World'");
    free(popped);

    Stack__pop(stack, &popped);
    ASSERT_STR_EQ((char *)popped, "Hello", "Should pop 'Hello'");
    free(popped);

    stack = Stack__del(stack);
}

// Test: Push and pop structures
TEST(test_stack_structs)
{
    typedef struct
    {
        int id;
        char name[50];
    } Person;

    Stack *stack = Stack__new();
    ASSERT_NOT_NULL(stack, "Stack should not be NULL");

    Person p1 = {1, "Alice"};
    Person p2 = {2, "Bob"};
    Person p3 = {3, "Charlie"};

    Stack__push(stack, &p1, sizeof(Person));
    Stack__push(stack, &p2, sizeof(Person));
    Stack__push(stack, &p3, sizeof(Person));

    ASSERT_EQ(stack->stackSize, 3, "Stack should have 3 elements");

    void *popped;

    Stack__pop(stack, &popped);
    Person *person = (Person *)popped;
    ASSERT_EQ(person->id, 3, "ID should be 3");
    ASSERT_STR_EQ(person->name, "Charlie", "Name should be 'Charlie'");
    free(popped);

    Stack__pop(stack, &popped);
    person = (Person *)popped;
    ASSERT_EQ(person->id, 2, "ID should be 2");
    ASSERT_STR_EQ(person->name, "Bob", "Name should be 'Bob'");
    free(popped);

    Stack__pop(stack, &popped);
    person = (Person *)popped;
    ASSERT_EQ(person->id, 1, "ID should be 1");
    ASSERT_STR_EQ(person->name, "Alice", "Name should be 'Alice'");
    free(popped);

    stack = Stack__del(stack);
}

// Test: Delete stack with elements
TEST(test_stack_del_with_elements)
{
    Stack *stack = Stack__new();
    ASSERT_NOT_NULL(stack, "Stack should not be NULL");

    for (int i = 0; i < 100; i++)
    {
        Stack__push(stack, &i, sizeof(int));
    }

    ASSERT_EQ(stack->stackSize, 100, "Stack should have 100 elements");

    // Delete should free all elements
    stack = Stack__del(stack);
    ASSERT(stack == NULL, "Stack should be NULL after delete");
}

// Test: Push after pop
TEST(test_stack_push_after_pop)
{
    Stack *stack = Stack__new();
    ASSERT_NOT_NULL(stack, "Stack should not be NULL");

    int value1 = 10;
    int value2 = 20;
    int value3 = 30;

    Stack__push(stack, &value1, sizeof(int));
    Stack__push(stack, &value2, sizeof(int));

    void *popped;
    Stack__pop(stack, &popped);
    free(popped);

    ASSERT_EQ(stack->stackSize, 1, "Stack size should be 1");

    Stack__push(stack, &value3, sizeof(int));
    ASSERT_EQ(stack->stackSize, 2, "Stack size should be 2");

    Stack__pop(stack, &popped);
    ASSERT_EQ(*(int *)popped, 30, "Should pop 30");
    free(popped);

    Stack__pop(stack, &popped);
    ASSERT_EQ(*(int *)popped, 10, "Should pop 10");
    free(popped);

    stack = Stack__del(stack);
}

// Test: Large number of operations
TEST(test_stack_large_operations)
{
    Stack *stack = Stack__new();
    ASSERT_NOT_NULL(stack, "Stack should not be NULL");

    // Push 1000 values
    for (int i = 0; i < 1000; i++)
    {
        int8_t result = Stack__push(stack, &i, sizeof(int));
        ASSERT_EQ(result, 0, "Push should succeed");
    }

    ASSERT_EQ(stack->stackSize, 1000, "Stack should have 1000 elements");

    // Pop 500 values
    for (int i = 999; i >= 500; i--)
    {
        void *popped;
        int8_t result = Stack__pop(stack, &popped);
        ASSERT_EQ(result, 0, "Pop should succeed");
        ASSERT_EQ(*(int *)popped, i, "Value should match");
        free(popped);
    }

    ASSERT_EQ(stack->stackSize, 500, "Stack should have 500 elements left");

    stack = Stack__del(stack);
}

// Test: Alternating push and pop
TEST(test_stack_alternating_operations)
{
    Stack *stack = Stack__new();
    ASSERT_NOT_NULL(stack, "Stack should not be NULL");

    for (int i = 0; i < 10; i++)
    {
        int value = i * 10;
        Stack__push(stack, &value, sizeof(int));

        void *popped;
        Stack__pop(stack, &popped);
        ASSERT_EQ(*(int *)popped, value, "Popped value should match pushed value");
        free(popped);

        ASSERT_EQ(stack->stackSize, 0, "Stack should be empty after each cycle");
    }

    stack = Stack__del(stack);
}

// Test: Stack size tracking
TEST(test_stack_size_tracking)
{
    Stack *stack = Stack__new();
    ASSERT_NOT_NULL(stack, "Stack should not be NULL");

    ASSERT_EQ(stack->stackSize, 0, "Initial size should be 0");

    for (int i = 1; i <= 5; i++)
    {
        Stack__push(stack, &i, sizeof(int));
        ASSERT_EQ(stack->stackSize, (size_t)i, "Size should increment correctly");
    }

    for (int i = 4; i >= 0; i--)
    {
        void *popped;
        Stack__pop(stack, &popped);
        free(popped);
        ASSERT_EQ(stack->stackSize, (size_t)i, "Size should decrement correctly");
    }

    stack = Stack__del(stack);
}

// Test: Different data sizes
TEST(test_stack_different_sizes)
{
    Stack *stack = Stack__new();
    ASSERT_NOT_NULL(stack, "Stack should not be NULL");

    // Push char
    char c = 'A';
    Stack__push(stack, &c, sizeof(char));

    // Push short
    short s = 1000;
    Stack__push(stack, &s, sizeof(short));

    // Push int
    int i = 100000;
    Stack__push(stack, &i, sizeof(int));

    // Push long
    long l = 1000000000L;
    Stack__push(stack, &l, sizeof(long));

    ASSERT_EQ(stack->stackSize, 4, "Stack should have 4 elements");

    void *popped;

    Stack__pop(stack, &popped);
    ASSERT_EQ(*(long *)popped, 1000000000L, "Should pop long");
    free(popped);

    Stack__pop(stack, &popped);
    ASSERT_EQ(*(int *)popped, 100000, "Should pop int");
    free(popped);

    Stack__pop(stack, &popped);
    ASSERT_EQ(*(short *)popped, 1000, "Should pop short");
    free(popped);

    Stack__pop(stack, &popped);
    ASSERT_EQ(*(char *)popped, 'A', "Should pop char");
    free(popped);

    stack = Stack__del(stack);
}

// Test: Deep copy verification
TEST(test_stack_deep_copy)
{
    Stack *stack = Stack__new();
    ASSERT_NOT_NULL(stack, "Stack should not be NULL");

    int value = 42;
    Stack__push(stack, &value, sizeof(int));

    // Modify original value
    value = 100;

    void *popped;
    Stack__pop(stack, &popped);
    // Should still be 42, not 100 (proving deep copy)
    ASSERT_EQ(*(int *)popped, 42, "Value should be 42 (deep copy)");
    free(popped);

    stack = Stack__del(stack);
}
