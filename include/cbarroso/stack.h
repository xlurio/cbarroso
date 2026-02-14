#ifndef CBARROSO_STACK_H
#define CBARROSO_STACK_H

#include <stddef.h>
#include <stdint.h>

typedef struct StackNode
{
    void *value;
    size_t valueSize;
    struct StackNode *next;
} StackNode;

typedef struct Stack
{
    StackNode *top;
    size_t stackSize;
} Stack;

Stack *Stack__new();
int8_t Stack__push(Stack *self, void *value, size_t valueSize);
int8_t Stack__pop(Stack *self, void **valueAddress);
Stack *Stack__del(Stack *self);

#endif
