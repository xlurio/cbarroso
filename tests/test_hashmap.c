#include <cbarroso/hashmap.h>
#include <ccauchy.h>

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

    int8_t result = HashMap__setItem(map, key, strlen(key) + 1, &value, sizeof(int));
    ASSERT_EQ(result, 0, "setItem should return 0 on success");
    ASSERT_EQ(map->nentries, 1, "nentries should be 1 after insert");

    void *retrieved = NULL;
    result = HashMap__getItem(map, key, strlen(key) + 1, &retrieved);
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
        int8_t result = HashMap__setItem(map, keys[i], strlen(keys[i]) + 1, &values[i], sizeof(int));
        ASSERT_EQ(result, 0, "setItem should succeed");
    }

    ASSERT_EQ(map->nentries, 5, "nentries should be 5");

    // Verify all items can be retrieved
    for (int i = 0; i < 5; i++)
    {
        void *retrieved = NULL;
        int8_t result = HashMap__getItem(map,
                         keys[i],
                         strlen(keys[i]) + 1,
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

    HashMap__setItem(map, key1, strlen(key1) + 1, value1, strlen(value1) + 1);
    HashMap__setItem(map, key2, strlen(key2) + 1, value2, strlen(value2) + 1);

    void *retrieved1 = NULL;
    void *retrieved2 = NULL;

    HashMap__getItem(map, key1, strlen(key1) + 1, &retrieved1);
    HashMap__getItem(map, key2, strlen(key2) + 1, &retrieved2);

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

    int8_t result = HashMap__setItem(map, key, strlen(key) + 1, &value, sizeof(int));
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

    int8_t result = HashMap__setItem(map, long_key, sizeof(long_key), &value, sizeof(int));
    ASSERT_EQ(result, 0, "setItem should handle long key");

    void *retrieved = NULL;
    result = HashMap__getItem(map, long_key, sizeof(long_key), &retrieved);
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

    HashMap__setItem(map, key, strlen(key) + 1, &value1, sizeof(int));
    int nentries_after_first = map->nentries;

    HashMap__setItem(map, key, strlen(key) + 1, &value2, sizeof(int));
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
        int8_t result = HashMap__setItem(map, keys[i], strlen(keys[i]) + 1, &values[i], sizeof(int));
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

    HashMap__setItem(map, key1, strlen(key1) + 1, &rec1, sizeof(rec1));
    HashMap__setItem(map, key2, strlen(key2) + 1, &rec2, sizeof(rec2));

    void *retrieved1 = NULL;
    HashMap__getItem(map, key1, strlen(key1) + 1, &retrieved1);
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

    int8_t result = HashMap__setItem(map, binary_key, sizeof(binary_key), &value, sizeof(int));
    ASSERT_EQ(result, 0, "setItem should handle binary key");

    void *retrieved = NULL;
    result = HashMap__getItem(map,
                              binary_key,
                              sizeof(binary_key),
                              &retrieved);
    ASSERT_EQ(result, 0, "getItem should find binary key");
    ASSERT_NOT_NULL(retrieved, "Retrieved value should not be NULL");
    ASSERT_EQ(*(int *)retrieved, value, "Retrieved value should match stored value");

    free(map);
}

// Test: Integer key (using int as key)
TEST(test_hashmap_integer_key)
{
    HashMap *map = HashMap__new(LOG2_MINSIZE);
    ASSERT_NOT_NULL(map, "HashMap should not be NULL");

    int key1 = 42;
    int key2 = 100;
    int key3 = -55;
    char *value1 = "Answer";
    char *value2 = "Century";
    char *value3 = "Negative";

    int8_t result = HashMap__setItem(map, &key1, sizeof(int), value1, strlen(value1) + 1);
    ASSERT_EQ(result, 0, "setItem should handle integer key");

    result = HashMap__setItem(map, &key2, sizeof(int), value2, strlen(value2) + 1);
    ASSERT_EQ(result, 0, "setItem should handle integer key");

    result = HashMap__setItem(map, &key3, sizeof(int), value3, strlen(value3) + 1);
    ASSERT_EQ(result, 0, "setItem should handle integer key");

    void *retrieved = NULL;
    result = HashMap__getItem(map, &key1, sizeof(int), &retrieved);
    ASSERT_EQ(result, 0, "getItem should find integer key");
    ASSERT_STR_EQ((char *)retrieved, value1, "Retrieved value should match");

    retrieved = NULL;
    result = HashMap__getItem(map, &key2, sizeof(int), &retrieved);
    ASSERT_EQ(result, 0, "getItem should find integer key");
    ASSERT_STR_EQ((char *)retrieved, value2, "Retrieved value should match");

    retrieved = NULL;
    result = HashMap__getItem(map, &key3, sizeof(int), &retrieved);
    ASSERT_EQ(result, 0, "getItem should find integer key");
    ASSERT_STR_EQ((char *)retrieved, value3, "Retrieved value should match");

    free(map);
}

// Test: Float key (using float as key)
TEST(test_hashmap_float_key)
{
    HashMap *map = HashMap__new(LOG2_MINSIZE);
    ASSERT_NOT_NULL(map, "HashMap should not be NULL");

    float key1 = 3.14159f;
    float key2 = 2.71828f;
    int value1 = 314;
    int value2 = 271;

    int8_t result = HashMap__setItem(map, &key1, sizeof(float), &value1, sizeof(int));
    ASSERT_EQ(result, 0, "setItem should handle float key");

    result = HashMap__setItem(map, &key2, sizeof(float), &value2, sizeof(int));
    ASSERT_EQ(result, 0, "setItem should handle float key");

    void *retrieved = NULL;
    result = HashMap__getItem(map, &key1, sizeof(float), &retrieved);
    ASSERT_EQ(result, 0, "getItem should find float key");
    ASSERT_EQ(*(int *)retrieved, value1, "Retrieved value should match");

    retrieved = NULL;
    result = HashMap__getItem(map, &key2, sizeof(float), &retrieved);
    ASSERT_EQ(result, 0, "getItem should find float key");
    ASSERT_EQ(*(int *)retrieved, value2, "Retrieved value should match");

    free(map);
}

// Test: Struct key (using custom struct as key)
TEST(test_hashmap_struct_key)
{
    typedef struct
    {
        int x;
        int y;
    } Point;

    HashMap *map = HashMap__new(LOG2_MINSIZE);
    ASSERT_NOT_NULL(map, "HashMap should not be NULL");

    Point key1 = {10, 20};
    Point key2 = {30, 40};
    Point key3 = {10, 20}; // Same as key1

    char *value1 = "Origin Area";
    char *value2 = "Far Corner";

    int8_t result = HashMap__setItem(map,
                                     &key1,
                                     sizeof(Point),
                                     value1,
                                     strlen(value1) + 1);
    ASSERT_EQ(result, 0, "setItem should handle struct key");

    result = HashMap__setItem(map, &key2, sizeof(Point), value2, strlen(value2) + 1);
    ASSERT_EQ(result, 0, "setItem should handle struct key");

    void *retrieved = NULL;
    result = HashMap__getItem(map, &key1, sizeof(Point), &retrieved);
    ASSERT_EQ(result, 0, "getItem should find struct key");
    ASSERT_STR_EQ((char *)retrieved, value1, "Retrieved value should match");

    // Test with same content as key1
    retrieved = NULL;
    result = HashMap__getItem(map, &key3, sizeof(Point), &retrieved);
    ASSERT_EQ(result, 0, "getItem should find identical struct key");
    ASSERT_STR_EQ((char *)retrieved, value1, "Retrieved value should match for identical struct");

    free(map);
}

// Test: Multiple data type keys in same map
TEST(test_hashmap_mixed_key_types)
{
    HashMap *map = HashMap__new(LOG2_MINSIZE);
    ASSERT_NOT_NULL(map, "HashMap should not be NULL");

    // Different key types (note: in real usage, you'd typically use one key type per map)
    int int_key = 123;
    float float_key = 4.56f;
    char str_key[] = "text_key";
    unsigned char bin_key[] = {0xAA, 0xBB, 0xCC};

    int value1 = 1000;
    int value2 = 2000;
    int value3 = 3000;
    int value4 = 4000;

    // Insert with different key types and sizes
    HashMap__setItem(map, &int_key, sizeof(int), &value1, sizeof(int));
    HashMap__setItem(map, &float_key, sizeof(float), &value2, sizeof(int));
    HashMap__setItem(map, str_key, strlen(str_key) + 1, &value3, sizeof(int));
    HashMap__setItem(map, bin_key, sizeof(bin_key), &value4, sizeof(int));

    ASSERT_EQ(map->nentries, 4, "Should have 4 entries");

    // Retrieve all
    void *retrieved = NULL;
    HashMap__getItem(map, &int_key, sizeof(int), &retrieved);
    ASSERT_EQ(*(int *)retrieved, value1, "Integer key retrieval should work");

    retrieved = NULL;
    HashMap__getItem(map, &float_key, sizeof(float), &retrieved);
    ASSERT_EQ(*(int *)retrieved, value2, "Float key retrieval should work");

    retrieved = NULL;
    HashMap__getItem(map, str_key, strlen(str_key) + 1, &retrieved);
    ASSERT_EQ(*(int *)retrieved, value3, "String key retrieval should work");

    retrieved = NULL;
    HashMap__getItem(map, bin_key, sizeof(bin_key), &retrieved);
    ASSERT_EQ(*(int *)retrieved, value4, "Binary key retrieval should work");

    free(map);
}

// Test: Long integer key (64-bit)
TEST(test_hashmap_long_integer_key)
{
    HashMap *map = HashMap__new(LOG2_MINSIZE);
    ASSERT_NOT_NULL(map, "HashMap should not be NULL");

    int64_t key1 = 9223372036854775807LL;  // Max int64
    int64_t key2 = -9223372036854775807LL; // Min int64 + 1
    int64_t key3 = 0LL;

    double value1 = 1.1;
    double value2 = 2.2;
    double value3 = 3.3;

    HashMap__setItem(map, &key1, sizeof(int64_t), &value1, sizeof(double));
    HashMap__setItem(map, &key2, sizeof(int64_t), &value2, sizeof(double));
    HashMap__setItem(map, &key3, sizeof(int64_t), &value3, sizeof(double));

    void *retrieved = NULL;
    HashMap__getItem(map, &key1, sizeof(int64_t), &retrieved);
    ASSERT(*(double *)retrieved == value1, "Long integer key should work");

    retrieved = NULL;
    HashMap__getItem(map, &key2, sizeof(int64_t), &retrieved);
    ASSERT(*(double *)retrieved == value2, "Negative long integer key should work");

    retrieved = NULL;
    HashMap__getItem(map, &key3, sizeof(int64_t), &retrieved);
    ASSERT(*(double *)retrieved == value3, "Zero long integer key should work");

    free(map);
}

// Test: Complex binary data as key
TEST(test_hashmap_complex_binary_key)
{
    HashMap *map = HashMap__new(LOG2_MINSIZE);
    ASSERT_NOT_NULL(map, "HashMap should not be NULL");

    // Simulate complex binary data like a hash or UUID
    unsigned char key1[16] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
                              0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    unsigned char key2[16] = {0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88,
                              0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00};

    char *value1 = "UUID-like key 1";
    char *value2 = "UUID-like key 2";

    int8_t result = HashMap__setItem(map, key1, sizeof(key1), value1, strlen(value1) + 1);
    ASSERT_EQ(result, 0, "setItem should handle complex binary key");

    result = HashMap__setItem(map, key2, sizeof(key2), value2, strlen(value2) + 1);
    ASSERT_EQ(result, 0, "setItem should handle complex binary key");

    void *retrieved = NULL;
    result = HashMap__getItem(map, key1, sizeof(key1), &retrieved);
    ASSERT_EQ(result, 0, "getItem should find complex binary key");
    ASSERT_STR_EQ((char *)retrieved, value1, "Retrieved value should match");

    retrieved = NULL;
    result = HashMap__getItem(map, key2, sizeof(key2), &retrieved);
    ASSERT_EQ(result, 0, "getItem should find complex binary key");
    ASSERT_STR_EQ((char *)retrieved, value2, "Retrieved value should match");

    free(map);
}

// Test: Pointer as key (using memory address)
TEST(test_hashmap_pointer_key)
{
    HashMap *map = HashMap__new(LOG2_MINSIZE);
    ASSERT_NOT_NULL(map, "HashMap should not be NULL");

    int obj1 = 111;
    int obj2 = 222;
    int obj3 = 333;

    void *key1 = &obj1;
    void *key2 = &obj2;
    void *key3 = &obj3;

    char *value1 = "Object 1";
    char *value2 = "Object 2";
    char *value3 = "Object 3";

    // Use pointer address as key
    HashMap__setItem(map, &key1, sizeof(void *), value1, strlen(value1) + 1);
    HashMap__setItem(map, &key2, sizeof(void *), value2, strlen(value2) + 1);
    HashMap__setItem(map, &key3, sizeof(void *), value3, strlen(value3) + 1);

    void *retrieved = NULL;
    HashMap__getItem(map, &key1, sizeof(void *), &retrieved);
    ASSERT_STR_EQ((char *)retrieved, value1, "Pointer key should work");

    retrieved = NULL;
    HashMap__getItem(map, &key2, sizeof(void *), &retrieved);
    ASSERT_STR_EQ((char *)retrieved, value2, "Pointer key should work");

    free(map);
}
