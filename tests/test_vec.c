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
    vec_t* vec = vec_create(VEC_MIN_CAPACITY, sizeof(int));
    TEST_ASSERT_NOT_NULL(vec);
    TEST_ASSERT_NOT_NULL(vec->data);
    TEST_ASSERT_EQUAL_size_t(VEC_MIN_CAPACITY, vec->capacity);
    TEST_ASSERT_EQUAL_size_t(sizeof(int), vec->size);
    TEST_ASSERT_EQUAL_size_t(0, vec->count);
    vec_free(vec);

    // Test zero capacity (should fail)
    vec = vec_create(0, sizeof(int));
    TEST_ASSERT_NULL(vec);
    TEST_ASSERT_EQUAL_INT(EINVAL, errno);
}

static void test_vec_push(void) {
    vec_t* vec = vec_create(VEC_MIN_CAPACITY, sizeof(int));

    // Test basic push
    int value    = 42;
    void* result = vec_push(vec, &value);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_INT(42, *(int*)result);
    TEST_ASSERT_EQUAL_size_t(1, vec->count);

    // Test push causing resize
    for (int i = 0; i < VEC_MIN_CAPACITY; i++) {
        result = vec_push(vec, &i);
        TEST_ASSERT_NOT_NULL(result);
    }
    TEST_ASSERT_EQUAL_size_t(VEC_MIN_CAPACITY * 2, vec->capacity);

    // Test null checks
    TEST_ASSERT_NULL(vec_push(NULL, &value));
    TEST_ASSERT_EQUAL_INT(EINVAL, errno);
    TEST_ASSERT_NULL(vec_push(vec, NULL));
    TEST_ASSERT_EQUAL_INT(EINVAL, errno);

    vec_free(vec);
}

static void test_vec_pop(void) {
    vec_t* vec = vec_create(VEC_MIN_CAPACITY, sizeof(int));

    // Test pop from empty vector
    int out;
    TEST_ASSERT_EQUAL_INT(-1, vec_pop(vec, &out));
    TEST_ASSERT_EQUAL_INT(ENODATA, errno);

    // Test pop after push
    int value = 42;
    vec_push(vec, &value);
    TEST_ASSERT_EQUAL_INT(0, vec_pop(vec, &out));
    TEST_ASSERT_EQUAL_INT(42, out);
    TEST_ASSERT_EQUAL_size_t(0, vec->count);

    // Test null checks
    TEST_ASSERT_EQUAL_INT(-1, vec_pop(NULL, &out));
    TEST_ASSERT_EQUAL_INT(EINVAL, errno);

    // Test pop without out parameter
    vec_push(vec, &value);
    TEST_ASSERT_EQUAL_INT(0, vec_pop(vec, NULL));
    TEST_ASSERT_EQUAL_size_t(0, vec->count);

    vec_free(vec);
}

static void test_vec_at(void) {
    vec_t* vec = vec_create(VEC_MIN_CAPACITY, sizeof(int));

    // Test out of bounds
    TEST_ASSERT_NULL(vec_at(vec, 0));
    TEST_ASSERT_EQUAL_INT(ERANGE, errno);

    // Test valid access
    int value = 42;
    vec_push(vec, &value);
    void* result = vec_at(vec, 0);
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_INT(42, *(int*)result);

    // Test null checks
    TEST_ASSERT_NULL(vec_at(NULL, 0));
    TEST_ASSERT_EQUAL_INT(EINVAL, errno);

    vec_free(vec);
}

static void test_vec_resize(void) {
    vec_t* vec = vec_create(VEC_MIN_CAPACITY, sizeof(int));

    // Fill vector to trigger resize
    for (int i = 0; i < VEC_MIN_CAPACITY + 1; i++) {
        vec_push(vec, &i);
    }
    TEST_ASSERT_EQUAL_size_t(VEC_MIN_CAPACITY * 2, vec->capacity);

    // Pop items to trigger shrink
    int out;
    while (vec->count > VEC_MIN_CAPACITY / 4) {
        vec_pop(vec, &out);
    }
    TEST_ASSERT_EQUAL_size_t(VEC_MIN_CAPACITY, vec->capacity);

    vec_free(vec);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_vec_create);
    RUN_TEST(test_vec_push);
    RUN_TEST(test_vec_pop);
    RUN_TEST(test_vec_at);
    RUN_TEST(test_vec_resize);
    return UNITY_END();
}
