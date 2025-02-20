/**
 * @file vec.h
 * @brief A generic, dynamic array implementation in C
 *
 * This vector implementation provides a resizable array that can store elements
 * of any type. The vector automatically grows and shrinks as elements are added
 * or removed.
 */

#ifndef VEC_VEC_H_
#define VEC_VEC_H_

#include <stddef.h>
#include <sys/types.h>

/** Minimum capacity for vector to prevent excessive resizing */
#define VEC_MIN_CAPACITY 16

/**
 * @brief Vector structure holding the array and its metadata
 */
typedef struct vec {
    void* data;      /**< Pointer to the array data */
    size_t size;     /**< Size of each element in bytes */
    size_t count;    /**< Number of elements currently in the vector */
    size_t capacity; /**< Total number of elements that can be stored */
} vec_t;

/**
 * @brief Get the number of elements in the vector
 * @param vec Pointer to the vector
 * @return Current number of elements
 */
static inline size_t vec_count(const vec_t* vec) {
    return vec->count;
}

/**
 * @brief Get iterator to the beginning of the vector's data
 * @param vec Pointer to the vector
 * @return Pointer to first element of vector's data
 * @note Unsafe - does not check if vec is NULL or empty
 * @warning Direct access to internal data - use with caution
 */
static inline void* vec_iter(const vec_t* vec) {
    return vec->data;
}

/**
 * @brief Access element at specified index
 * @param vec Pointer to the vector
 * @param index Index of element to access
 * @return Pointer to element if found, NULL if index out of bounds or vec is NULL
 * @note Sets errno to EINVAL if vec is NULL, ERANGE if index out of bounds
 */
void* vec_at(vec_t* vec, size_t index);

/**
 * @brief Remove and optionally return the last element
 * @param vec Pointer to the vector
 * @param out Optional pointer to store popped element (can be NULL)
 * @return 0 on success, -1 on failure
 * @note Sets errno to EINVAL if vec is NULL, ENODATA if vector is empty
 */
ssize_t vec_pop(vec_t* vec, void* out);

/**
 * @brief Add element to end of vector
 * @param vec Pointer to the vector
 * @param element Pointer to element to add
 * @return Pointer to newly added element, NULL on failure
 * @note Sets errno to EINVAL if vec or element is NULL, ENOMEM if allocation fails
 */
void* vec_push(vec_t* vec, const void* element);

/**
 * @brief Create new vector with specified capacity and element size
 * @param capacity Initial number of elements that can be stored
 * @param size Size of each element in bytes
 * @return Pointer to new vector, NULL on failure
 * @note Sets errno to EINVAL if capacity or size is 0, ENOMEM if allocation fails
 */
vec_t* vec_create(size_t capacity, size_t size);

/**
 * @brief Free vector and its data
 * @param vec Pointer to vector to free
 */
void vec_free(vec_t* vec);

#endif  // VEC_VEC_H_
