#ifndef SNGLLNKDLIST_H
#define SNGLLNKDLIST_H

#include <stddef.h>
#include <stdint.h>

typedef struct SinglyLinkedListNode
{
    void *value;
    size_t valueSize;
    struct SinglyLinkedListNode *next;
} SinglyLinkedListNode;

SinglyLinkedListNode *SinglyLinkedListNode__new(void *value, size_t valueSize);
int8_t SinglyLinkedListNode__insertAtTail(SinglyLinkedListNode *self,
                                          void *value,
                                          size_t valueSize);
void SinglyLinkedListNode__del(SinglyLinkedListNode *self);

#endif
