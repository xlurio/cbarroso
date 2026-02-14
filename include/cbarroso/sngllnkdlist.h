#ifndef _SNGLLNKDLIST
#define _SNGLLNKDLIST

#include <stdint.h>

typedef struct SinglyLinkedListNode
{
    void *value;
    struct SinglyLinkedListNode *next;
} SinglyLinkedListNode;

SinglyLinkedListNode *SinglyLinkedListNode__new(void *value);
int8_t SinglyLinkedListNode__insert(SinglyLinkedListNode *self, void *value);
void SinglyLinkedListNode__del(SinglyLinkedListNode *self);

#endif
