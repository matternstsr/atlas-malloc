#include "malloc.h"

#define INITIAL_HEAP_SIZE 4096

void *naive_malloc(size_t size) {
    static char *heap_start = NULL; // Start of the allocated heap
    static char *heap_end = NULL;   // Current end of the allocated heap
    static char *current = NULL;     // Pointer to the current position in the heap
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

    // Check if there's enough space left
    if (current + size > heap_end) {
        return NULL; // Not enough space
    }

    ptr = current;
    current += size; // Move the current pointer forward

    // Store the size at the beginning (if needed)
    *(size_t *)ptr = size;

    return (void *)(ptr + sizeof(size_t)); // Return pointer after size
}
