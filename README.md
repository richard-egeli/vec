# Generic Vector Implementation in C
A lightweight, generic vector (dynamic array) implementation in C that provides automatic resizing and type-safe operations. This implementation focuses on safety, efficiency, and ease of use while maintaining a small footprint.

## Features
- Generic type support through macros and void pointers
- Automatic growth and shrinking (halves capacity when 75% empty)
- Bounds checking and error handling
- Memory safety with proper error reporting
- Type-safe element access through direct array indexing
- Efficient implementation using struct hack with flexible array member
- Comprehensive unit tests using Unity framework

## API
```c
// Create a new vector
#define vec_create(typesize) __vec_create_impl(typesize)

// Add element to vector
#define vec_push(vec, element) __vec_push_impl((void**)&vec, (void*)element)

// Remove last element
#define vec_pop(vec, out) __vec_pop_impl((void**)&vec, (void*)out)

// Get current element count
#define vec_count(vec) __vec_count_impl((const void*)&vec)

// Get current capacity
#define vec_capacity(vec) __vec_capacity_impl((const void*)&vec)

// Free vector
#define vec_free(vec) __vec_free_impl((void**)&vec)
```

## Usage Example
```c
// Create vector of integers
int* vec = vec_create(sizeof(int));

// Push elements
int value = 42;
vec_push(vec, &value);

// Access elements by index (direct array access)
printf("First element: %d\n", vec[0]);

// You can also use vec_at for bounds checking (if implemented in your code)
// int* element = vec_at(vec, 0);

// Pop elements
int out;
vec_pop(vec, &out);

// Clean up
vec_free(vec);
```

## Building
```bash
cmake -S . -B build
cmake --build build
```

### Building With Tests
```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build
./bin/test_vec
```

## Error Handling
The library has mixed error handling:
- Most functions return negative error codes directly (e.g., -EINVAL, -ENODATA)
- Some internal functions (like vec_create_impl) still set errno
- Error codes include:
  - EINVAL: Invalid arguments (NULL pointers, zero size)
  - ERANGE: Index out of bounds
  - ENOMEM: Memory allocation failure
  - ENODATA: Operation on empty vector

## Contributing
Contributions welcome! Please feel free to submit a Pull Request.
