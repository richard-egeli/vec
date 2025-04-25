#include "vec.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Vector structure holding the array and its metadata
 */
typedef struct vec {
    size_t size;     /**< Size of each element in bytes */
    size_t count;    /**< Number of elements currently in the vector */
    size_t capacity; /**< Total number of elements that can be stored */
    uint8_t data[];
} vec_t;

static inline ssize_t vec_resize(vec_t** vec, size_t new_capacity) {
    size_t new_size = sizeof(vec_t) + (new_capacity * (*vec)->size);

    vec_t* new_vec  = realloc(*vec, new_size);
    if (new_vec == NULL) {
        errno = ENOMEM;
        return -1;
    }

    new_vec->capacity = new_capacity;
    *vec              = new_vec;
    return 0;
}

size_t __vec_capacity_impl(uintptr_t addr) {
    const size_t offset = offsetof(vec_t, data) - offsetof(vec_t, capacity);
    return (void*)addr != NULL ? *(size_t*)(addr - offset) : 0;
}

size_t __vec_length_impl(uintptr_t addr) {
    const size_t offset = offsetof(vec_t, data) - offsetof(vec_t, count);
    return (void*)addr != NULL ? *(size_t*)(addr - offset) : 0;
}

ssize_t __vec_pop_impl(void** data, void* out) {
    if (data == NULL || out == NULL) {
        return -EINVAL;
    }

    vec_t* vec = ((vec_t*)((uintptr_t)(*data) - offsetof(vec_t, data)));
    if (vec->count == 0) {
        return -ENODATA;
    }

    vec->count--;
    if (out != NULL) {
        const size_t offset = (vec->count * vec->size);
        void* ptr           = (uint8_t*)vec->data + offset;
        memcpy(out, ptr, vec->size);
    }

    const size_t threshold = vec->capacity / 4;
    if (vec->count < threshold && vec->capacity > VEC_MIN_CAPACITY) {
        ssize_t rc = vec_resize(&vec, vec->capacity / 2);
        if (rc < 0) {
            return rc;
        }

        *data = vec->data;
    }

    return 0;
}

ssize_t __vec_push_impl(void** data, const void* element) {
    if (data == NULL || element == NULL) {
        return -EINVAL;
    }

    vec_t* vec = ((vec_t*)((uintptr_t)(*data) - offsetof(vec_t, data)));
    if (vec->count >= vec->capacity) {
        ssize_t rc = vec_resize(&vec, vec->capacity * 2);
        if (rc < 0) {
            return rc;
        }

        *data = vec->data;
    }

    const size_t offset = (vec->count * vec->size);
    uint8_t* ptr        = (uint8_t*)vec->data + offset;
    memcpy(ptr, element, vec->size);
    vec->count++;
    return 0;
}

void __vec_free_impl(uintptr_t addr) {
    if ((void*)addr != NULL) {
        free(((vec_t*)(addr - offsetof(vec_t, data))));
    }
}

void* __vec_create_impl(size_t size) {
    if (size == 0) {
        return NULL;
    }

    vec_t* vec = malloc(sizeof(*vec) + VEC_MIN_CAPACITY * size);
    if (vec == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    vec->size     = size;
    vec->capacity = VEC_MIN_CAPACITY;
    vec->count    = 0;
    return vec->data;
}
