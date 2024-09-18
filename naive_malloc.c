#include "malloc.h"

/**
* naive_malloc - Allocates memory in the heap
* @size: size of memory to allocate
* Return: returns a pointer to the allocated memory
*/

void *naive_malloc(size_t size) {
    static void *heap_end = NULL;
    void *prev_heap_end;
    size_t aligned_size;

    if (size == 0)
        return NULL;

    aligned_size = ALIGN_SIZED(size + sizeof(size_t));

    if (heap_end == NULL)
        heap_end = sbrk(0);  // Initial heap end

    prev_heap_end = heap_end;
    if (sbrk(aligned_size) == (void *)-1)
        return NULL;  // sbrk failed
    heap_end = (char *)heap_end + aligned_size;  // Update heap end

    *(size_t *)prev_heap_end = aligned_size;  // Store size
    return (char *)prev_heap_end + sizeof(size_t);  // Return pointer
}
