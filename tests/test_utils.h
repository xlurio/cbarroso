#ifndef CBARROSO_TEST_UTILS_H
#define CBARROSO_TEST_UTILS_H

#include <stdio.h>
#include <stdlib.h>

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
        printf("  \u2713 %s passed\n", #test); \
        fflush(stdout);                   \
    } while (0)

#define ASSERT(condition, message)                         \
    do                                                     \
    {                                                      \
        if (!(condition))                                  \
        {                                                  \
            printf("  \u2717 Assertion failed: %s\n", message); \
            exit(1);                                       \
        }                                                  \
    } while (0)

#define ASSERT_EQ(a, b, message)                                                                         \
    do                                                                                                   \
    {                                                                                                    \
        if ((a) != (b))                                                                                  \
        {                                                                                                \
            printf("  \u2717 Assertion failed: %s (expected %ld, got %ld)\n", message, (long)(b), (long)(a)); \
            exit(1);                                                                                     \
        }                                                                                                \
    } while (0)

#define ASSERT_NOT_NULL(ptr, message)                              \
    do                                                             \
    {                                                              \
        if ((ptr) == NULL)                                         \
        {                                                          \
            printf("  \u2717 Assertion failed: %s is NULL\n", message); \
            exit(1);                                               \
        }                                                          \
    } while (0)

#endif // CBARROSO_TEST_UTILS_H
