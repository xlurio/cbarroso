#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cbarroso/constants.h>
#include <cbarroso/sngllnkdlist.h>

SinglyLinkedListNode *SinglyLinkedListNode__new(void *value, size_t valueSize)
{
    SinglyLinkedListNode *node = malloc(sizeof(SinglyLinkedListNode));

    if (node == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the singly linked list\n");
        return NULL;
    }

    node->value = malloc(valueSize);

    if (node == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the singly linked list node value\n");
        return NULL;
    }

    memcpy(node->value, value, valueSize);
    node->valueSize = valueSize;
    node->next = NULL;
    return node;
}

int8_t SinglyLinkedListNode__insertAtTail(SinglyLinkedListNode *self, void *value, size_t valueSize)
{
    SinglyLinkedListNode *new_node = SinglyLinkedListNode__new(value, valueSize);

    if (new_node == NULL)
    {
        fprintf(stderr, "Failed to create the new linked list node\n");
        return CBR_ERROR;
    }

    SinglyLinkedListNode *current = self;
    while (current->next != NULL)
    {
        current = current->next;
    }

    current->next = new_node;
    return CBR_SUCCESS;
}

void SinglyLinkedListNode__del(SinglyLinkedListNode *self)
{
    SinglyLinkedListNode *current = self;
    SinglyLinkedListNode *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}
