#ifndef CBARROSO_DBLYLNKDLIST_H
#define CBARROSO_DBLYLNKDLIST_H

#include <stddef.h>
#include <stdint.h>

typedef struct DoublyLinkedListNode
{
    void *value;
    size_t valueSize;
    struct DoublyLinkedListNode *prev;
    struct DoublyLinkedListNode *next;
} DoublyLinkedListNode;

DoublyLinkedListNode *DoublyLinkedListNode__new(void *value, size_t valueSize);
int8_t DoublyLinkedListNode__insertAtTail(DoublyLinkedListNode *self,
                                          void *value,
                                          size_t valueSize);
DoublyLinkedListNode *DoublyLinkedListNode__del(DoublyLinkedListNode *self);

#endif
