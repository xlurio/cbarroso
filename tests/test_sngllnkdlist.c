
#include <stdint.h>
#include <cbarroso/sngllnkdlist.h>
#include <cbarroso/constants.h>
#include "test_utils.h"


TEST(test_sngllnkdlist_create_and_insert)
{
    int a = 10, b = 20, c = 30;
    SinglyLinkedListNode *head = SinglyLinkedListNode__new(&a);
    ASSERT_NOT_NULL(head, "Node creation failed");
    ASSERT_EQ(SinglyLinkedListNode__insert(head, &b), CBR_SUCCESS, "Insert failed");
    ASSERT_EQ(SinglyLinkedListNode__insert(head, &c), CBR_SUCCESS, "Insert failed");

    int *vals[3];
    SinglyLinkedListNode *cur = head;
    for (int i = 0; i < 3 && cur; ++i) {
        vals[i] = (int *)cur->value;
        cur = cur->next;
    }
    ASSERT(!cur, "List should have exactly 3 nodes");
    ASSERT_EQ(*vals[0], 10, "First value incorrect");
    ASSERT_EQ(*vals[1], 20, "Second value incorrect");
    ASSERT_EQ(*vals[2], 30, "Third value incorrect");
    SinglyLinkedListNode__del(head);
}

TEST(test_sngllnkdlist_delete)
{
    int a = 1;
    SinglyLinkedListNode *head = SinglyLinkedListNode__new(&a);
    ASSERT_NOT_NULL(head, "Node creation failed");
    SinglyLinkedListNode__del(head);
}

int main(void)
{
    printf("=== SinglyLinkedList Test Suite ===\n\n");
    RUN_TEST(test_sngllnkdlist_create_and_insert);
    RUN_TEST(test_sngllnkdlist_delete);

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
