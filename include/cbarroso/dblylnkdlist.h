#ifndef DBLYLNKDLIST_H
#define DBLYLNKDLIST_H

#include <stdint.h>

typedef struct DoublyLinkedListNode
{
    void *value;
    struct DoublyLinkedListNode *prev;
    struct DoublyLinkedListNode *next;
} DoublyLinkedListNode;

DoublyLinkedListNode *DoublyLinkedListNode__new(void *value);
int8_t DoublyLinkedListNode__insertAtTail(DoublyLinkedListNode *self, void *value);
DoublyLinkedListNode *DoublyLinkedListNode__del(DoublyLinkedListNode *self);

#endif
