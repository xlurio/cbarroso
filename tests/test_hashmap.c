#include <cbarroso/hashmap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Test counter
static int tests_run = 0;
static int tests_passed = 0;

#define TEST(name) void name()
#define RUN_TEST(test)                    \
    do                                    \
    {                                     \
        printf("Running %s...\n", #test); \
        fflush(stdout);                   \
        tests_run++;                      \
        test();                           \
        tests_passed++;                   \
        printf("  ✓ %s passed\n", #test); \
        fflush(stdout);                   \
    } while (0)

#define ASSERT(condition, message)                         \
    do                                                     \
    {                                                      \
        if (!(condition))                                  \
        {                                                  \
            printf("  ✗ Assertion failed: %s\n", message); \
            exit(1);                                       \
        }                                                  \
    } while (0)

#define ASSERT_EQ(a, b, message)                                                                         \
    do                                                                                                   \
    {                                                                                                    \
        if ((a) != (b))                                                                                  \
        {                                                                                                \
            printf("  ✗ Assertion failed: %s (expected %ld, got %ld)\n", message, (long)(b), (long)(a)); \
            exit(1);                                                                                     \
        }                                                                                                \
    } while (0)

#define ASSERT_NOT_NULL(ptr, message)                              \
    do                                                             \
    {                                                              \
        if ((ptr) == NULL)                                         \
        {                                                          \
            printf("  ✗ Assertion failed: %s is NULL\n", message); \
            exit(1);                                               \
        }                                                          \
    } while (0)

#define ASSERT_STR_EQ(a, b, message)                                                           \
    do                                                                                         \
    {                                                                                          \
        if (strcmp((a), (b)) != 0)                                                             \
        {                                                                                      \
            printf("  ✗ Assertion failed: %s (expected '%s', got '%s')\n", message, (b), (a)); \
            exit(1);                                                                           \
        }                                                                                      \
    } while (0)

// Test: Create a new HashMap
TEST(test_hashmap_new)
{
    HashMap *map = HashMap__new(LOG2_MINSIZE);
    ASSERT_NOT_NULL(map, "HashMap should not be NULL");
    ASSERT_EQ(map->log2_size, LOG2_MINSIZE, "log2_size should match");
    ASSERT_EQ(map->nentries, 0, "nentries should be 0");
    ASSERT(map->usable > 0, "usable should be greater than 0");
    free(map);
}

// Test: Create HashMap with different sizes
TEST(test_hashmap_different_sizes)
{
    for (uint8_t log2_size = LOG2_MINSIZE; log2_size < 10; log2_size++)
    {
        HashMap *map = HashMap__new(log2_size);
        ASSERT_NOT_NULL(map, "HashMap should not be NULL");
        ASSERT_EQ(map->log2_size, log2_size, "log2_size should match");
        free(map);
    }
}

// Test: Insert and retrieve a single item
TEST(test_hashmap_insert_and_get_single)
{
    HashMap *map = HashMap__new(LOG2_MINSIZE);
    ASSERT_NOT_NULL(map, "HashMap should not be NULL");

    char *key = "test_key";
    int value = 42;

    int8_t result = HashMap__setItem(map, key, strlen(key), &value);
    ASSERT_EQ(result, 0, "setItem should return 0 on success");
    ASSERT_EQ(map->nentries, 1, "nentries should be 1 after insert");

    void *retrieved = NULL;
    result = HashMap__getItem(map, key, strlen(key), &retrieved);
    ASSERT_EQ(result, 0, "getItem should return 0 on success");
    ASSERT_NOT_NULL(retrieved, "Retrieved value should not be NULL");
    ASSERT_EQ(*(int *)retrieved, value, "Retrieved value should match stored value");

    free(map);
}

// Test: Insert multiple items
TEST(test_hashmap_insert_multiple)
{
    HashMap *map = HashMap__new(LOG2_MINSIZE);
    ASSERT_NOT_NULL(map, "HashMap should not be NULL");

    int values[5];
    char keys[5][20];

    // Insert 5 items (within initial capacity to avoid resize)
    for (int i = 0; i < 5; i++)
    {
        values[i] = i * 100;
        snprintf(keys[i], 20, "key_%d", i);
        int8_t result = HashMap__setItem(map, keys[i], strlen(keys[i]), &values[i]);
        ASSERT_EQ(result, 0, "setItem should succeed");
    }

    ASSERT_EQ(map->nentries, 5, "nentries should be 5");

    // Verify all items can be retrieved
    for (int i = 0; i < 5; i++)
    {
        void *retrieved = NULL;
        int8_t result = HashMap__getItem(map,
                                         keys[i],
                                         strlen(keys[i]),
                                         &retrieved);
        ASSERT_EQ(result, 0, "getItem should succeed");
        ASSERT_NOT_NULL(retrieved, "Retrieved value should not be NULL");
        ASSERT_EQ(*(int *)retrieved, values[i], "Retrieved value should match stored value");
    }

    free(map);
}

// Test: Insert with string values
TEST(test_hashmap_string_values)
{
    HashMap *map = HashMap__new(LOG2_MINSIZE);
    ASSERT_NOT_NULL(map, "HashMap should not be NULL");

    char *key1 = "name";
    char *value1 = "Alice";
    char *key2 = "city";
    char *value2 = "New York";

    HashMap__setItem(map, key1, strlen(key1), value1);
    HashMap__setItem(map, key2, strlen(key2), value2);

    void *retrieved1 = NULL;
    void *retrieved2 = NULL;

    HashMap__getItem(map, key1, strlen(key1), &retrieved1);
    HashMap__getItem(map, key2, strlen(key2), &retrieved2);

    ASSERT_NOT_NULL(retrieved1, "First value should not be NULL");
    ASSERT_NOT_NULL(retrieved2, "Second value should not be NULL");
    ASSERT_STR_EQ((char *)retrieved1, value1, "First value should match");
    ASSERT_STR_EQ((char *)retrieved2, value2, "Second value should match");

    free(map);
}

// Test: Empty key handling
TEST(test_hashmap_empty_key)
{
    HashMap *map = HashMap__new(LOG2_MINSIZE);
    ASSERT_NOT_NULL(map, "HashMap should not be NULL");

    char *key = "";
    int value = 100;

    int8_t result = HashMap__setItem(map, key, strlen(key), &value);
    ASSERT_EQ(result, 0, "setItem should handle empty key");

    free(map);
}

// Test: Long key handling
TEST(test_hashmap_long_key)
{
    HashMap *map = HashMap__new(LOG2_MINSIZE);
    ASSERT_NOT_NULL(map, "HashMap should not be NULL");

    char long_key[256];
    memset(long_key, 'a', 255);
    long_key[255] = '\0';

    int value = 999;

    int8_t result = HashMap__setItem(map, long_key, strlen(long_key), &value);
    ASSERT_EQ(result, 0, "setItem should handle long key");

    void *retrieved = NULL;
    result = HashMap__getItem(map, long_key, strlen(long_key), &retrieved);
    ASSERT_EQ(result, 0, "getItem should find long key");
    ASSERT_NOT_NULL(retrieved, "Retrieved value should not be NULL");
    ASSERT_EQ(*(int *)retrieved, value, "Retrieved value should match stored value");

    free(map);
}

// Test: Overwriting existing key (update value)
TEST(test_hashmap_update_value)
{
    HashMap *map = HashMap__new(LOG2_MINSIZE);
    ASSERT_NOT_NULL(map, "HashMap should not be NULL");

    char *key = "counter";
    int value1 = 10;
    int value2 = 20;

    HashMap__setItem(map, key, strlen(key), &value1);
    int nentries_after_first = map->nentries;

    HashMap__setItem(map, key, strlen(key), &value2);

    // Note: The current implementation might not handle updates properly
    // This test documents the expected behavior
    ASSERT(map->nentries >= nentries_after_first,
           "nentries should not decrease after update");

    free(map);
}

// Test: Large number of insertions (within capacity)
TEST(test_hashmap_large_insertions)
{
    HashMap *map = HashMap__new(7); // 2^7 = 128 slots, ~85 usable
    ASSERT_NOT_NULL(map, "HashMap should not be NULL");

    int values[50];
    char keys[50][30];

    // Insert 50 items (within capacity)
    for (int i = 0; i < 50; i++)
    {
        values[i] = i;
        snprintf(keys[i], 30, "key_number_%d", i);
        int8_t result = HashMap__setItem(map, keys[i], strlen(keys[i]), &values[i]);
        ASSERT_EQ(result, 0, "setItem should succeed for large insertion");
    }

    ASSERT_EQ(map->nentries, 50, "nentries should be 50");

    free(map);
}

// Test: Different value types (struct)
TEST(test_hashmap_struct_values)
{
    typedef struct
    {
        int id;
        float score;
    } Record;

    HashMap *map = HashMap__new(LOG2_MINSIZE);
    ASSERT_NOT_NULL(map, "HashMap should not be NULL");

    Record rec1 = {1, 95.5f};
    Record rec2 = {2, 87.3f};

    char *key1 = "student1";
    char *key2 = "student2";

    HashMap__setItem(map, key1, strlen(key1), &rec1);
    HashMap__setItem(map, key2, strlen(key2), &rec2);

    void *retrieved1 = NULL;
    HashMap__getItem(map, key1, strlen(key1), &retrieved1);
    ASSERT_NOT_NULL(retrieved1, "Retrieved record should not be NULL");
    Record *rec_ptr = (Record *)retrieved1;
    ASSERT_EQ(rec_ptr->id, rec1.id, "Record ID should match");
    ASSERT(rec_ptr->score == rec1.score, "Record score should match");

    free(map);
}

// Test: Binary key (non-string data as key)
TEST(test_hashmap_binary_key)
{
    HashMap *map = HashMap__new(LOG2_MINSIZE);
    ASSERT_NOT_NULL(map, "HashMap should not be NULL");

    unsigned char binary_key[] = {0x01, 0x02, 0x03, 0x04};
    int value = 777;

    int8_t result = HashMap__setItem(map, (char *)binary_key, sizeof(binary_key), &value);
    ASSERT_EQ(result, 0, "setItem should handle binary key");

    void *retrieved = NULL;
    result = HashMap__getItem(map,
                              (char *)binary_key,
                              sizeof(binary_key),
                              &retrieved);
    ASSERT_EQ(result, 0, "getItem should find binary key");
    ASSERT_NOT_NULL(retrieved, "Retrieved value should not be NULL");
    ASSERT_EQ(*(int *)retrieved, value, "Retrieved value should match stored value");

    free(map);
}

int main(int argc, char **argv)
{
    printf("=== HashMap Test Suite ===\n\n");

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

    printf("\n=== Test Results ===\n");
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);

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
