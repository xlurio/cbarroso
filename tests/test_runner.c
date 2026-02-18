#include <ccauchy.h>

// Forward declarations for all test functions
// Stack tests
void test_stack_new(void);
void test_stack_push_single_int(void);
void test_stack_push_multiple_ints(void);
void test_stack_pop_single(void);
void test_stack_pop_empty(void);
void test_stack_lifo_order(void);
void test_stack_strings(void);
void test_stack_structs(void);
void test_stack_del_with_elements(void);
void test_stack_push_after_pop(void);
void test_stack_large_operations(void);
void test_stack_alternating_operations(void);
void test_stack_size_tracking(void);
void test_stack_different_sizes(void);
void test_stack_deep_copy(void);

// Queue tests
void test_queue_new(void);
void test_queue_enqueue_single_int(void);
void test_queue_enqueue_multiple_ints(void);
void test_queue_dequeue_single(void);
void test_queue_dequeue_empty(void);
void test_queue_fifo_order(void);
void test_queue_enqueue_strings(void);
void test_queue_enqueue_structs(void);
void test_queue_mixed_operations(void);
void test_queue_large(void);
void test_queue_alternating_operations(void);

// HashMap tests
void test_hashmap_new(void);
void test_hashmap_different_sizes(void);
void test_hashmap_insert_and_get_single(void);
void test_hashmap_insert_multiple(void);
void test_hashmap_string_values(void);
void test_hashmap_empty_key(void);
void test_hashmap_long_key(void);
void test_hashmap_update_value(void);
void test_hashmap_large_insertions(void);
void test_hashmap_struct_values(void);
void test_hashmap_binary_key(void);
void test_hashmap_integer_key(void);
void test_hashmap_float_key(void);
void test_hashmap_struct_key(void);
void test_hashmap_mixed_key_types(void);
void test_hashmap_long_integer_key(void);
void test_hashmap_complex_binary_key(void);
void test_hashmap_pointer_key(void);

// DoublyLinkedList tests
void test_dblylnkdlist_create_node(void);
void test_dblylnkdlist_insert_at_tail(void);
void test_dblylnkdlist_delete_list(void);
void test_dblylnkdlist_delete_from_middle(void);
void test_dblylnkdlist_delete_single_node(void);
void test_dblylnkdlist_delete_null(void);

// SinglyLinkedList tests
void test_sngllnkdlist_create_and_insert(void);
void test_sngllnkdlist_delete(void);

// Tree tests
void test_tree_create_node(void);
void test_tree_create_with_string(void);
void test_tree_insert_single_child(void);
void test_tree_insert_multiple_children(void);
void test_tree_nested_children(void);
void test_tree_delete_null(void);
void test_tree_delete_with_children(void);
void test_tree_with_struct_values(void);

int main(void)
{
    printf("===========================================\n");
    printf("         cbarroso Test Suite\n");
    printf("===========================================\n\n");

    // Stack Tests
    printf("--- Stack Tests ---\n");
    RUN_TEST(test_stack_new);
    RUN_TEST(test_stack_push_single_int);
    RUN_TEST(test_stack_push_multiple_ints);
    RUN_TEST(test_stack_pop_single);
    RUN_TEST(test_stack_pop_empty);
    RUN_TEST(test_stack_lifo_order);
    RUN_TEST(test_stack_strings);
    RUN_TEST(test_stack_structs);
    RUN_TEST(test_stack_del_with_elements);
    RUN_TEST(test_stack_push_after_pop);
    RUN_TEST(test_stack_large_operations);
    RUN_TEST(test_stack_alternating_operations);
    RUN_TEST(test_stack_size_tracking);
    RUN_TEST(test_stack_different_sizes);
    RUN_TEST(test_stack_deep_copy);

    // Queue Tests
    printf("\n--- Queue Tests ---\n");
    RUN_TEST(test_queue_new);
    RUN_TEST(test_queue_enqueue_single_int);
    RUN_TEST(test_queue_enqueue_multiple_ints);
    RUN_TEST(test_queue_dequeue_single);
    RUN_TEST(test_queue_dequeue_empty);
    RUN_TEST(test_queue_fifo_order);
    RUN_TEST(test_queue_enqueue_strings);
    RUN_TEST(test_queue_enqueue_structs);
    RUN_TEST(test_queue_mixed_operations);
    RUN_TEST(test_queue_large);
    RUN_TEST(test_queue_alternating_operations);

    // HashMap Tests
    printf("\n--- HashMap Tests ---\n");
    RUN_TEST(test_hashmap_new);
    RUN_TEST(test_hashmap_different_sizes);
    RUN_TEST(test_hashmap_insert_and_get_single);
    RUN_TEST(test_hashmap_insert_multiple);
    RUN_TEST(test_hashmap_string_values);
    RUN_TEST(test_hashmap_empty_key);
    RUN_TEST(test_hashmap_long_key);
    RUN_TEST(test_hashmap_update_value);
    RUN_TEST(test_hashmap_large_insertions);
    RUN_TEST(test_hashmap_struct_values);
    RUN_TEST(test_hashmap_binary_key);
    RUN_TEST(test_hashmap_integer_key);
    RUN_TEST(test_hashmap_float_key);
    RUN_TEST(test_hashmap_struct_key);
    RUN_TEST(test_hashmap_mixed_key_types);
    RUN_TEST(test_hashmap_long_integer_key);
    RUN_TEST(test_hashmap_complex_binary_key);
    RUN_TEST(test_hashmap_pointer_key);

    // DoublyLinkedList Tests
    printf("\n--- DoublyLinkedList Tests ---\n");
    RUN_TEST(test_dblylnkdlist_create_node);
    RUN_TEST(test_dblylnkdlist_insert_at_tail);
    RUN_TEST(test_dblylnkdlist_delete_list);
    RUN_TEST(test_dblylnkdlist_delete_from_middle);
    RUN_TEST(test_dblylnkdlist_delete_single_node);
    RUN_TEST(test_dblylnkdlist_delete_null);

    // SinglyLinkedList Tests
    printf("\n--- SinglyLinkedList Tests ---\n");
    RUN_TEST(test_sngllnkdlist_create_and_insert);
    RUN_TEST(test_sngllnkdlist_delete);

    // Tree Tests
    printf("\n--- Tree Tests ---\n");
    RUN_TEST(test_tree_create_node);
    RUN_TEST(test_tree_create_with_string);
    RUN_TEST(test_tree_insert_single_child);
    RUN_TEST(test_tree_insert_multiple_children);
    RUN_TEST(test_tree_nested_children);
    RUN_TEST(test_tree_delete_null);
    RUN_TEST(test_tree_delete_with_children);
    RUN_TEST(test_tree_with_struct_values);

    // Summary
    printf("\n===========================================\n");
    printf("              Test Summary\n");
    printf("===========================================\n");
    printf("Tests run:    %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_run - tests_passed);

    if (tests_passed == tests_run)
    {
        printf("\n✓ All tests passed!\n");
        return 0;
    }
    else
    {
        printf("\n✗ Some tests failed!\n");
        return 1;
    }
}
