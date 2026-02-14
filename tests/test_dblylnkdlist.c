#include <stdint.h>
#include <cbarroso/dblylnkdlist.h>
#include <cbarroso/constants.h>
#include "test_utils.h"


TEST(test_dblylnkdlist_create_node)
{
    int a = 42;
    DoublyLinkedListNode *node = DoublyLinkedListNode__new(&a);
    ASSERT_NOT_NULL(node, "Node creation failed");
    ASSERT_EQ(*(int *)node->value, 42, "Value incorrect");
    ASSERT(node->prev == NULL, "Prev should be NULL");
    ASSERT(node->next == NULL, "Next should be NULL");
    free(node);
}

TEST(test_dblylnkdlist_insert_at_tail)
{
    int a = 10, b = 20, c = 30;
    DoublyLinkedListNode *head = DoublyLinkedListNode__new(&a);
    ASSERT_NOT_NULL(head, "Node creation failed");
    ASSERT_EQ(DoublyLinkedListNode__insertAtTail(head, &b), CBR_SUCCESS, "Insert failed");
    ASSERT_EQ(DoublyLinkedListNode__insertAtTail(head, &c), CBR_SUCCESS, "Insert failed");

    int *vals[3];
    DoublyLinkedListNode *cur = head;
    for (int i = 0; i < 3 && cur; ++i) {
        vals[i] = (int *)cur->value;
        cur = cur->next;
    }
    ASSERT(!cur, "List should have exactly 3 nodes");
    ASSERT_EQ(*vals[0], 10, "First value incorrect");
    ASSERT_EQ(*vals[1], 20, "Second value incorrect");
    ASSERT_EQ(*vals[2], 30, "Third value incorrect");

    cur = head->next->next;
    ASSERT_EQ(*(int *)cur->value, 30, "Tail value incorrect");
    ASSERT_EQ(*(int *)cur->prev->value, 20, "Middle value incorrect via prev");
    ASSERT_EQ(*(int *)cur->prev->prev->value, 10, "Head value incorrect via prev");
    ASSERT(cur->prev->prev->prev == NULL, "Head prev should be NULL");

    cur = head;
    while (cur) {
        DoublyLinkedListNode *next = cur->next;
        free(cur);
        cur = next;
    }
}

TEST(test_dblylnkdlist_delete_middle_node)
{
    int a = 10, b = 20, c = 30;
    DoublyLinkedListNode *head = DoublyLinkedListNode__new(&a);
    DoublyLinkedListNode__insertAtTail(head, &b);
    DoublyLinkedListNode__insertAtTail(head, &c);

    DoublyLinkedListNode *middle = head->next;
    DoublyLinkedListNode *result = DoublyLinkedListNode__del(middle);
    
    ASSERT_NOT_NULL(result, "Should return previous node");
    ASSERT_EQ(*(int *)result->value, 10, "Should return head node");
    ASSERT_NOT_NULL(head->next, "Head next should point to third node");
    ASSERT_EQ(*(int *)head->next->value, 30, "Head next should be third node");
    ASSERT_EQ(head->next->prev, head, "Third node prev should point to head");

    free(head->next);
    free(head);
}

TEST(test_dblylnkdlist_delete_head_node)
{
    int a = 10, b = 20;
    DoublyLinkedListNode *head = DoublyLinkedListNode__new(&a);
    DoublyLinkedListNode__insertAtTail(head, &b);

    // Delete head node
    DoublyLinkedListNode *result = DoublyLinkedListNode__del(head);
    
    ASSERT_NOT_NULL(result, "Should return next node");
    ASSERT_EQ(*(int *)result->value, 20, "Should return second node");
    ASSERT(result->prev == NULL, "New head prev should be NULL");

    free(result);
}

TEST(test_dblylnkdlist_delete_tail_node)
{
    int a = 10, b = 20;
    DoublyLinkedListNode *head = DoublyLinkedListNode__new(&a);
    DoublyLinkedListNode__insertAtTail(head, &b);

    DoublyLinkedListNode *tail = head->next;
    DoublyLinkedListNode *result = DoublyLinkedListNode__del(tail);
    
    ASSERT_NOT_NULL(result, "Should return previous node");
    ASSERT_EQ(*(int *)result->value, 10, "Should return head node");
    ASSERT(result->next == NULL, "Head next should be NULL");

    free(result);
}

TEST(test_dblylnkdlist_delete_single_node)
{
    int a = 10;
    DoublyLinkedListNode *node = DoublyLinkedListNode__new(&a);
    
    DoublyLinkedListNode *result = DoublyLinkedListNode__del(node);
    ASSERT(result == NULL, "Deleting single node should return NULL");
}

TEST(test_dblylnkdlist_delete_null)
{
    DoublyLinkedListNode *result = DoublyLinkedListNode__del(NULL);
    ASSERT(result == NULL, "Deleting NULL should return NULL");
}

int main(void)
{
    printf("=== DoublyLinkedList Test Suite ===\n\n");
    RUN_TEST(test_dblylnkdlist_create_node);
    RUN_TEST(test_dblylnkdlist_insert_at_tail);
    RUN_TEST(test_dblylnkdlist_delete_middle_node);
    RUN_TEST(test_dblylnkdlist_delete_head_node);
    RUN_TEST(test_dblylnkdlist_delete_tail_node);
    RUN_TEST(test_dblylnkdlist_delete_single_node);
    RUN_TEST(test_dblylnkdlist_delete_null);

    printf("\n=== Test Results ===\n");
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);

    if (tests_passed == tests_run)
    {
        printf("\n\u2713 All tests passed!\n");
        return 0;
    }
    else
    {
        printf("\n\u2717 Some tests failed!\n");
        return 1;
    }
}
