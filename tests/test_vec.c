#include <errno.h>
#include <stdio.h>
#include <unity.h>

#include "vec.h"

void setUp(void) {
    errno = 0;  // Reset errno before each test
}

void tearDown(void) {
    // Nothing to clean up
}

static void test_vec_create(void) {
    // Test normal creation
    int* vec = vec_create(int);
    TEST_ASSERT_NOT_NULL(vec);
    vec_free(vec);
}

static void test_vec_push(void) {
    int* arr = vec_create(int);

    // Test basic push
    int value = 42;
    TEST_ASSERT_EQUAL(0, vec_push(arr, &value));
    TEST_ASSERT_EQUAL_INT(42, arr[0]);
    TEST_ASSERT_EQUAL_size_t(1, vec_count(arr));

    // Test push causing resize
    for (int i = 0; i < VEC_MIN_CAPACITY; i++) {
        TEST_ASSERT_EQUAL_INT(0, vec_push(arr, &i));
    }

    TEST_ASSERT_EQUAL_size_t(VEC_MIN_CAPACITY * 2, vec_capacity(arr));

    vec_free(arr);
}

static void test_vec_pop(void) {
    int* vec = vec_create(int);

    // Test pop from empty vector
    int out;
    TEST_ASSERT_EQUAL_INT(-ENODATA, vec_pop(vec, &out));

    // Test pop after push
    int value = 42;
    vec_push(vec, &value);
    TEST_ASSERT_EQUAL_INT(0, vec_pop(vec, &out));
    TEST_ASSERT_EQUAL_INT(42, out);
    TEST_ASSERT_EQUAL_size_t(0, vec_count(vec));

    // Test pop without out parameter
    vec_push(vec, &value);
    TEST_ASSERT_EQUAL_INT(0, vec_pop(vec, NULL));
    TEST_ASSERT_EQUAL_size_t(0, vec_count(vec));

    vec_free(vec);
}

static void test_vec_resize(void) {
    int* arr = vec_create(int);

    // Fill vector to trigger resize
    for (int i = 0; i < VEC_MIN_CAPACITY + 1; i++) {
        vec_push(arr, &i);
    }
    TEST_ASSERT_EQUAL_size_t(VEC_MIN_CAPACITY * 2, vec_capacity(arr));

    // Pop items to trigger shrink
    int out;
    while (vec_count(arr) > VEC_MIN_CAPACITY / 4) {
        vec_pop(arr, &out);
    }
    TEST_ASSERT_EQUAL_size_t(VEC_MIN_CAPACITY, vec_capacity(arr));

    vec_free(arr);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_vec_create);
    RUN_TEST(test_vec_push);
    RUN_TEST(test_vec_pop);
    RUN_TEST(test_vec_resize);
    return UNITY_END();
}
