#ifndef MALLOC_NEW_H
#define MALLOC_NEW_H

#include <stddef.h> // For size_t

// Initialize the allocator
void init_allocator();

// Allocate memory of a given size
void *naive_malloc(size_t size);

#endif // MALLOC_NEW_H
