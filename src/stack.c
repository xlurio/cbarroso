#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <cbarroso/constants.h>
#include <cbarroso/stack.h>

int8_t Stack__push(Stack *self,
                   void *value,
                   size_t valueSize)
{
    StackNode *node = malloc(sizeof(StackNode));

    if (node == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the parser stack node\n");
        return CBR_ERROR;
    }

    node->value = malloc(valueSize);

    if (node->value == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the parser stack node value\n");
        return CBR_ERROR;
    }

    memcpy(node->value, value, valueSize);

    if (self->stackSize > 0)
        node->next = self->top;
    else
        node->next = NULL;

    self->top = node;
    self->stackSize++;

    return CBR_SUCCESS;
}

Stack *Stack__new()
{
    Stack *stack = malloc(sizeof(Stack));

    if (stack == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the parser stack\n");
        return NULL;
    }

    stack->top = NULL;
    stack->stackSize = 0;

    return stack;
}

int8_t Stack__pop(Stack *self, void **valueAddress)
{
    if (self->top == NULL)
    {
        fprintf(stderr, "Empty stack\n");
        return CBR_ERROR;
    }

    *valueAddress = self->top->value;
    StackNode *newTop = self->top->next;
    free(self->top);
    self->top = newTop;
    self->stackSize--;

    return CBR_SUCCESS;
}

Stack *Stack__del(Stack *self)
{
    while (self->stackSize > 0)
    {
        void *value;
        Stack__pop(self, &value);
        free(value);
        self->stackSize--;
    }

    free(self);
    return NULL;
}
