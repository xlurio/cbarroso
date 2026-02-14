#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cbarroso/constants.h>
#include <cbarroso/sngllnkdlist.h>
#include <cbarroso/tree.h>

TreeNode *TreeNode__new(void *value, size_t valueSize)
{
    TreeNode *node = malloc(sizeof(TreeNode));

    if (node == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the AST node\n");
        return NULL;
    }

    node->value = malloc(valueSize);
    if (node->value == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the value\n");
        free(node);
        return NULL;
    }

    memcpy(node->value, value, valueSize);
    node->valueSize = valueSize;
    node->childrenHead = NULL;
    node->childrenTail = NULL;
    node->numOfChildren = 0;

    return node;
}

int8_t TreeNode__insert(TreeNode *self, TreeNode *newChild)
{
    SinglyLinkedListNode *childrenListNode;

    if (self->numOfChildren <= 0)
    {
        childrenListNode = SinglyLinkedListNode__new(newChild, sizeof(TreeNode));

        if (childrenListNode == NULL)
        {
            return CBR_ERROR;
        }

        self->childrenHead = childrenListNode;
        self->childrenTail = childrenListNode;
    }
    else
    {
        int8_t result = SinglyLinkedListNode__insertAtTail(self->childrenTail,
                                                           newChild,
                                                           sizeof(TreeNode));

        if (result == CBR_ERROR)
        {
            return CBR_ERROR;
        }

        self->childrenTail = self->childrenTail->next;
    }

    self->numOfChildren++;

    return CBR_SUCCESS;
}

void TreeNode__del(TreeNode *self)
{
    if (self == NULL)
    {
        return;
    }

    SinglyLinkedListNode *current = self->childrenHead;
    while (current != NULL)
    {
        SinglyLinkedListNode *next = current->next;
        TreeNode *childNode = (TreeNode *)current->value;
        TreeNode__del(childNode);
        free(current);
        current = next;
    }

    free(self->value);
    free(self);
}
