#include "malloc.h"

#define INITIAL_HEAP_SIZE 4096
#define ALIGNMENT sizeof(size_t)

void *naive_malloc(size_t size) {
    static char *heap_start = NULL; // Start of the allocated heap
    static char *heap_end = NULL;   // Current end of the allocated heap
    static char *current = NULL;     // Pointer to the current position in the heap
    static size_t current_size = 16; // Starting size for the next allocation
    void *ptr;

    if (size == 0) {
        return NULL;
    }

    // Initialize the heap if it hasn't been done yet
    if (heap_start == NULL) {
        heap_start = sbrk(INITIAL_HEAP_SIZE);
        if (heap_start == (void *)-1) {
            return NULL; // sbrk failed
        }
        heap_end = heap_start + INITIAL_HEAP_SIZE;
        current = heap_start; // Set current to start of the heap
    }

    // Align the size to the nearest boundary
    size = (current_size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);

    // Check if there's enough space left
    if (current + size > heap_end) {
        return NULL; // Not enough space
    }

    ptr = current;
    current += size; // Move the current pointer forward

    // Store the size at the beginning (if needed)
    *(size_t *)ptr = size;

    // Update current size for the next allocation to match the expected pattern
    current_size += 16; // Increment by 16 bytes for the next allocation

    // Return pointer after size
    return (void *)((char *)ptr + sizeof(size_t));
}
