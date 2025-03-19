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

size_t __vec_capacity_impl(const void** vec);
#define vec_capacity(vec) __vec_capacity_impl((void*)&vec)

/**
 * @brief Get the number of elements in the vector
 * @param vec Pointer to the vector
 * @return Current number of elements
 */
size_t __vec_count_impl(const void** vec);
#define vec_count(vec) __vec_count_impl((void*)&vec)

// /**
//  * @brief Access element at specified index
//  * @param vec Pointer to the vector
//  * @param index Index of element to access
//  * @return Pointer to element if found, NULL if index out of bounds or vec is NULL
//  * @note Sets errno to EINVAL if vec is NULL, ERANGE if index out of bounds
//  */
// void* vec_at(void* vec, size_t index);

/**
 * @brief Remove and optionally return the last element
 * @param vec Pointer to the vector
 * @param out Optional pointer to store popped element (can be NULL)
 * @return 0 on success, -1 on failure
 * @note Sets errno to EINVAL if vec is NULL, ENODATA if vector is empty
 */
ssize_t __vec_pop_impl(void** vec, void* out);
#define vec_pop(vec, out) __vec_pop_impl((void**)&vec, (void*)out)

/**
 * @brief Add element to end of vector
 * @param vec Pointer to the vector
 * @param element Pointer to element to add
 * @return Pointer to newly added element, NULL on failure
 * @note Sets errno to EINVAL if vec or element is NULL, ENOMEM if allocation fails
 */
ssize_t __vec_push_impl(void** vec, const void* element);
#define vec_push(vec, element) __vec_push_impl((void**)&vec, (void*)element)

/**
 * @brief Create new vector with specified capacity and element size
 * @param size Size of each element in bytes
 * @return Pointer to new vector, NULL on failure
 * @note Sets errno to EINVAL if capacity or size is 0, ENOMEM if allocation fails
 */
void* __vec_create_impl(size_t typesize);
#define vec_create(type) __vec_create_impl(sizeof(type))

/**
 * @brief Free vector and its data
 * @param vec Pointer to vector to free
 */
void __vec_free_impl(void** vec);
#define vec_free(vec) __vec_free_impl((void**)&vec)

#endif  // VEC_VEC_H_
