#ifndef TREE_H
#define TREE_H

#include <stddef.h>
#include "sngllnkdlist.h"

typedef struct TreeNode
{
    void *value;
    size_t valueSize;
    SinglyLinkedListNode *childrenHead;
    SinglyLinkedListNode *childrenTail;
    size_t numOfChildren;
} TreeNode;

TreeNode *TreeNode__new(void *value, size_t valueSize);
int8_t TreeNode__insert(TreeNode *self, TreeNode *newChild);
void TreeNode__del(TreeNode *self);

#endif
