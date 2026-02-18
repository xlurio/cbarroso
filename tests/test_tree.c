#include <stdint.h>
#include <string.h>
#include <cbarroso/tree.h>
#include <cbarroso/constants.h>
#include <ccauchy.h>

TEST(test_tree_create_node)
{
    int value = 42;
    TreeNode *node = TreeNode__new(&value, sizeof(int));
    
    ASSERT_NOT_NULL(node, "TreeNode creation failed");
    ASSERT_NOT_NULL(node->value, "TreeNode value is NULL");
    ASSERT_EQ(*(int *)node->value, 42, "TreeNode value incorrect");
    ASSERT_EQ(node->valueSize, sizeof(int), "TreeNode valueSize incorrect");
    ASSERT_EQ(node->numOfChildren, 0, "TreeNode should have 0 children");
    ASSERT(node->childrenHead == NULL, "TreeNode childrenHead should be NULL");
    ASSERT(node->childrenTail == NULL, "TreeNode childrenTail should be NULL");
    
    TreeNode__del(node);
}

TEST(test_tree_create_with_string)
{
    char *value = "test_string";
    TreeNode *node = TreeNode__new(value, strlen(value) + 1);
    
    ASSERT_NOT_NULL(node, "TreeNode creation failed");
    ASSERT_NOT_NULL(node->value, "TreeNode value is NULL");
    ASSERT_STR_EQ((char *)node->value, "test_string", "TreeNode string value incorrect");
    ASSERT_EQ(node->valueSize, strlen(value) + 1, "TreeNode valueSize incorrect");
    
    TreeNode__del(node);
}

TEST(test_tree_insert_single_child)
{
    int rootValue = 1;
    int childValue = 2;
    
    TreeNode *root = TreeNode__new(&rootValue, sizeof(int));
    TreeNode *child = TreeNode__new(&childValue, sizeof(int));
    
    ASSERT_NOT_NULL(root, "Root node creation failed");
    ASSERT_NOT_NULL(child, "Child node creation failed");
    
    int8_t result = TreeNode__insert(root, child);
    ASSERT_EQ(result, CBR_SUCCESS, "Insert should succeed");
    ASSERT_EQ(root->numOfChildren, 1, "Root should have 1 child");
    ASSERT_NOT_NULL(root->childrenHead, "childrenHead should not be NULL");
    ASSERT_NOT_NULL(root->childrenTail, "childrenTail should not be NULL");
    ASSERT(root->childrenHead == root->childrenTail, "Head and tail should be the same for single child");
    
    TreeNode *retrievedChild = (TreeNode *)root->childrenHead->value;
    ASSERT_EQ(*(int *)retrievedChild->value, 2, "Child value should be 2");
    
    TreeNode__del(root);
}

TEST(test_tree_insert_multiple_children)
{
    int rootValue = 0;
    TreeNode *root = TreeNode__new(&rootValue, sizeof(int));
    ASSERT_NOT_NULL(root, "Root node creation failed");
    
    // Insert 5 children
    for (int i = 1; i <= 5; i++)
    {
        int *childValue = malloc(sizeof(int));
        *childValue = i * 10;
        TreeNode *child = TreeNode__new(childValue, sizeof(int));
        ASSERT_NOT_NULL(child, "Child node creation failed");
        
        int8_t result = TreeNode__insert(root, child);
        ASSERT_EQ(result, CBR_SUCCESS, "Insert should succeed");
        free(childValue);
    }
    
    ASSERT_EQ(root->numOfChildren, 5, "Root should have 5 children");
    
    // Verify all children
    SinglyLinkedListNode *current = root->childrenHead;
    int expectedValue = 10;
    int count = 0;
    
    while (current != NULL)
    {
        TreeNode *child = (TreeNode *)current->value;
        ASSERT_EQ(*(int *)child->value, expectedValue, "Child value incorrect");
        expectedValue += 10;
        count++;
        current = current->next;
    }
    
    ASSERT_EQ(count, 5, "Should have exactly 5 children");
    
    TreeNode__del(root);
}

TEST(test_tree_nested_children)
{
    // Create a tree structure:
    //        root (0)
    //       /    \
    //    child1  child2
    //    (10)    (20)
    //    /
    // grandchild
    //   (100)
    
    int rootValue = 0;
    int child1Value = 10;
    int child2Value = 20;
    int grandchildValue = 100;
    
    TreeNode *root = TreeNode__new(&rootValue, sizeof(int));
    TreeNode *child1 = TreeNode__new(&child1Value, sizeof(int));
    TreeNode *child2 = TreeNode__new(&child2Value, sizeof(int));
    TreeNode *grandchild = TreeNode__new(&grandchildValue, sizeof(int));
    
    ASSERT_NOT_NULL(root, "Root creation failed");
    ASSERT_NOT_NULL(child1, "Child1 creation failed");
    ASSERT_NOT_NULL(child2, "Child2 creation failed");
    ASSERT_NOT_NULL(grandchild, "Grandchild creation failed");
    
    // Build tree structure
    ASSERT_EQ(TreeNode__insert(root, child1), CBR_SUCCESS, "Insert child1 failed");
    ASSERT_EQ(TreeNode__insert(root, child2), CBR_SUCCESS, "Insert child2 failed");
    ASSERT_EQ(TreeNode__insert(child1, grandchild), CBR_SUCCESS, "Insert grandchild failed");
    
    // Verify structure
    ASSERT_EQ(root->numOfChildren, 2, "Root should have 2 children");
    ASSERT_EQ(child1->numOfChildren, 1, "Child1 should have 1 child");
    ASSERT_EQ(child2->numOfChildren, 0, "Child2 should have 0 children");
    
    TreeNode *retrievedChild1 = (TreeNode *)root->childrenHead->value;
    ASSERT_EQ(*(int *)retrievedChild1->value, 10, "First child value incorrect");
    
    TreeNode *retrievedGrandchild = (TreeNode *)child1->childrenHead->value;
    ASSERT_EQ(*(int *)retrievedGrandchild->value, 100, "Grandchild value incorrect");
    
    TreeNode__del(root);
}

TEST(test_tree_delete_null)
{
    // Should not crash
    TreeNode__del(NULL);
}

TEST(test_tree_delete_with_children)
{
    int rootValue = 1;
    TreeNode *root = TreeNode__new(&rootValue, sizeof(int));
    
    for (int i = 0; i < 3; i++)
    {
        int *childValue = malloc(sizeof(int));
        *childValue = i + 10;
        TreeNode *child = TreeNode__new(childValue, sizeof(int));
        TreeNode__insert(root, child);
        free(childValue);
    }
    
    // This should free root and all children recursively
    TreeNode__del(root);
    // If we get here without a crash, the test passes
}

TEST(test_tree_with_struct_values)
{
    typedef struct {
        int id;
        char name[20];
    } Person;
    
    Person rootPerson = {1, "Alice"};
    Person child1Person = {2, "Bob"};
    Person child2Person = {3, "Charlie"};
    
    TreeNode *root = TreeNode__new(&rootPerson, sizeof(Person));
    TreeNode *child1 = TreeNode__new(&child1Person, sizeof(Person));
    TreeNode *child2 = TreeNode__new(&child2Person, sizeof(Person));
    
    ASSERT_NOT_NULL(root, "Root creation failed");
    ASSERT_NOT_NULL(child1, "Child1 creation failed");
    ASSERT_NOT_NULL(child2, "Child2 creation failed");
    
    TreeNode__insert(root, child1);
    TreeNode__insert(root, child2);
    
    Person *rootData = (Person *)root->value;
    ASSERT_EQ(rootData->id, 1, "Root id incorrect");
    ASSERT_STR_EQ(rootData->name, "Alice", "Root name incorrect");
    
    TreeNode *retrievedChild1 = (TreeNode *)root->childrenHead->value;
    Person *child1Data = (Person *)retrievedChild1->value;
    ASSERT_EQ(child1Data->id, 2, "Child1 id incorrect");
    ASSERT_STR_EQ(child1Data->name, "Bob", "Child1 name incorrect");
    
    TreeNode__del(root);
}
