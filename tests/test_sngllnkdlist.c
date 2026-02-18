
#include <stdint.h>
#include <cbarroso/sngllnkdlist.h>
#include <cbarroso/constants.h>
#include <ccauchy.h>


TEST(test_sngllnkdlist_create_and_insert)
{
    int a = 10, b = 20, c = 30;
    SinglyLinkedListNode *head = SinglyLinkedListNode__new(&a, sizeof(int));
    ASSERT_NOT_NULL(head, "Node creation failed");
    ASSERT_EQ(SinglyLinkedListNode__insertAtTail(head, &b, sizeof(int)), CBR_SUCCESS, "Insert failed");
    ASSERT_EQ(SinglyLinkedListNode__insertAtTail(head, &c, sizeof(int)), CBR_SUCCESS, "Insert failed");

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
    SinglyLinkedListNode *head = SinglyLinkedListNode__new(&a, sizeof(int));
    ASSERT_NOT_NULL(head, "Node creation failed");
    SinglyLinkedListNode__del(head);
}
