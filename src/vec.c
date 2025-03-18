#include "vec.h"

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Vector structure holding the array and its metadata
 */
typedef struct vec {
    void* data;      /**< Pointer to the array data */
    size_t size;     /**< Size of each element in bytes */
    size_t count;    /**< Number of elements currently in the vector */
    size_t capacity; /**< Total number of elements that can be stored */
} vec_t;

static inline ssize_t vec_resize(vec_t* vec, size_t new_capacity) {
    void* new_data = realloc(vec->data, new_capacity * vec->size);
    if (!new_data) {
        errno = ENOMEM;
        return -1;
    }

    vec->capacity = new_capacity;
    vec->data     = new_data;
    return 0;
}

size_t vec_capacity(const vec_t* vec) {
    return vec->capacity;
}

size_t vec_count(const vec_t* vec) {
    return vec->count;
}

void* vec_at(vec_t* vec, size_t index) {
    if (vec == NULL) {
        errno = EINVAL;
        return NULL;
    }

    if (index >= vec->count) {
        errno = ERANGE;
        return NULL;
    }

    const size_t offset = index * vec->size;
    return (uint8_t*)vec->data + offset;
}

ssize_t vec_pop(vec_t* vec, void* out) {
    if (vec == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (vec->count == 0) {
        errno = ENODATA;
        return -1;
    }

    vec->count--;

    if (out != NULL) {
        const size_t offset = (vec->count * vec->size);
        void* ptr           = (uint8_t*)vec->data + offset;
        memcpy(out, ptr, vec->size);
    }

    const size_t threshold = vec->capacity / 4;
    if (vec->count < threshold && vec->capacity > VEC_MIN_CAPACITY) {
        if (vec_resize(vec, vec->capacity / 2) != 0) {
            return -1;
        }
    }

    return 0;
}

void* vec_push(vec_t* vec, const void* element) {
    if (vec == NULL || element == NULL) {
        errno = EINVAL;
        return NULL;
    }

    if (vec->count >= vec->capacity) {
        if (vec_resize(vec, vec->capacity * 2) != 0) {
            return NULL;
        }
    }

    const size_t offset = (vec->count * vec->size);
    uint8_t* ptr        = (uint8_t*)vec->data + offset;
    memcpy(ptr, element, vec->size);
    vec->count++;
    return ptr;
}

void vec_free(vec_t* vec) {
    free(vec->data);
    free(vec);
}

vec_t* vec_create(size_t size) {
    if (size == 0) {
        errno = EINVAL;
        return NULL;
    }

    vec_t* vec = malloc(sizeof(*vec));
    if (vec == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    vec->data = calloc(1, VEC_MIN_CAPACITY * size);
    if (vec->data == NULL) {
        free(vec);
        errno = ENOMEM;
        return NULL;
    }

    vec->size     = size;
    vec->capacity = VEC_MIN_CAPACITY;
    vec->count    = 0;
    return vec;
}
