# Generic Vector Implementation in C
A lightweight, generic vector (dynamic array) implementation in C that provides automatic resizing and type-safe operations. This implementation focuses on safety, efficiency, and ease of use while maintaining a small footprint.

## Features
- Generic type support through void pointers
- Automatic growth and shrinking
- Bounds checking and error handling
- Memory safety with proper error reporting
- Type-safe element access
- Comprehensive unit tests using Unity framework

## API
```c
// Create a new vector
vec_t* vec_create(size_t capacity, size_t size);

// Add element to vector
void* vec_push(vec_t* vec, const void* element);

// Remove last element
ssize_t vec_pop(vec_t* vec, void* out);

// Access element at index
void* vec_at(vec_t* vec, size_t index);

// Get current element count
size_t vec_count(const vec_t* vec);

// Get iterator to beginning of data
void* vec_iter(const vec_t* vec);

// Free vector
void vec_free(vec_t* vec);
```

## Usage Example
```c
// Create vector of integers
vec_t* vec = vec_create(16, sizeof(int));

// Push elements
int value = 42;
vec_push(vec, &value);

// Access elements
int* element = vec_at(vec, 0);

// Pop elements
int out;
vec_pop(vec, &out);

// Clean up
vec_free(vec);
```

## Building
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Building With Tests
```bash
cmake -DBUILD_TESTING=ON ..
cmake --build .
ctest --output-on-failure
```

## Error Handling
The library uses errno to report errors:
- EINVAL: Invalid arguments (NULL pointers, zero size)
- ERANGE: Index out of bounds
- ENOMEM: Memory allocation failure
- ENODATA: Operation on empty vector

## Contributing
Contributions welcome! Please feel free to submit a Pull Request.
