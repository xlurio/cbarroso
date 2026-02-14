#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cbarroso/constants.h>
#include <cbarroso/dblylnkdlist.h>

DoublyLinkedListNode *DoublyLinkedListNode__new(void *value, size_t valueSize)
{
    DoublyLinkedListNode *node = malloc(sizeof(DoublyLinkedListNode));

    if (node == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the doubly linked list\n");
        return NULL;
    }

    node->value = malloc(valueSize);

    if (node->value == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the doubly linked list node value\n");
        return NULL;
    }

    memcpy(node->value, value, valueSize);

    node->valueSize = valueSize;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

int8_t DoublyLinkedListNode__insertAtTail(DoublyLinkedListNode *self,
                                          void *value,
                                          size_t valueSize)
{
    DoublyLinkedListNode *new_node = DoublyLinkedListNode__new(value, valueSize);
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

/* Traverse the whole linked list freeing the space allocated */
DoublyLinkedListNode *DoublyLinkedListNode__del(DoublyLinkedListNode *self)
{
    if (self == NULL)
    {
        return NULL;
    }

    DoublyLinkedListNode *head = self;
    while (head->prev != NULL)
    {
        head = head->prev;
    }

    DoublyLinkedListNode *current = head;
    DoublyLinkedListNode *next;

    while (current != NULL)
    {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }

    return NULL;
}
