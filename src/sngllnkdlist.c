#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <cbarroso/constants.h>
#include <cbarroso/sngllnkdlist.h>

SinglyLinkedListNode *SinglyLinkedListNode__new(void *value)
{
    SinglyLinkedListNode *node = malloc(sizeof(SinglyLinkedListNode));

    if (node == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the linked list\n");
        return NULL;
    }

    node->value = value;
    node->next = NULL;
    return node;
}

int8_t SinglyLinkedListNode__insertAtTail(SinglyLinkedListNode *self, void *value)
{
    SinglyLinkedListNode *new_node = SinglyLinkedListNode__new(value);

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
