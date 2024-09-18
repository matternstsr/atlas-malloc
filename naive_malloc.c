#include "malloc.h"

/**
* naive_malloc - Allocates memory in the heap
* @size: size of memory to allocate
* Return: returns a pointer to the allocated memory
*/

void *naive_malloc(size_t size) {
    if (size == 0) {
        return NULL; // No allocation needed for size 0
    }

    // Align the requested size
    size_t aligned_size = ALIGN_SIZE(size + sizeof(size_t));

    // Check if there is enough space in the buffer
    if (allocated_size + aligned_size > BUFFER_SIZE) {
        return NULL; // Not enough space
    }

    // Allocate memory from the buffer
    void *ptr = buffer + allocated_size;
    allocated_size += aligned_size;

    // Store the size at the beginning of the block
    *(size_t *)ptr = aligned_size;

    // Return pointer to the user
    return (char *)ptr + sizeof(size_t);
}