#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <cbarroso/constants.h>
#include <cbarroso/dblylnkdlist.h>

DoublyLinkedListNode *DoublyLinkedListNode__new(void *value)
{
    DoublyLinkedListNode *node = malloc(sizeof(DoublyLinkedListNode));

    if (node == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the doubly linked list\n");
        return NULL;
    }

    node->value = value;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

int8_t DoublyLinkedListNode__insertAtTail(DoublyLinkedListNode *self, void *value)
{
    DoublyLinkedListNode *new_node = DoublyLinkedListNode__new(value);
    if (new_node == NULL)
    {
        return CBR_ERROR;
    }

    DoublyLinkedListNode *current = self;
    while (current->next != NULL)
    {
        current = current->next;
    }

    current->next = new_node;
    new_node->prev = current;
    return CBR_SUCCESS;
}

DoublyLinkedListNode *DoublyLinkedListNode__del(DoublyLinkedListNode *self)
{
    if (self == NULL)
    {
        return NULL;
    }

    DoublyLinkedListNode *prev_node = self->prev;
    DoublyLinkedListNode *next_node = self->next;

    if (prev_node != NULL)
    {
        prev_node->next = next_node;
    }

    if (next_node != NULL)
    {
        next_node->prev = prev_node;
    }

    free(self);

    if (prev_node != NULL)
    {
        return prev_node;
    }

    return next_node;
}
