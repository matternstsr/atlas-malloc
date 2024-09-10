#include "malloc.h"

size_t align_size(size_t size) {
    return ((size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;
}

/**
 * naive_malloc - Allocates memory in the heap
 * @size: size of memory to allocate
 * Return: returns a pointer to the allocated memory
 * When the program starts, the heap is initialized. The sbrk(0) call gets the current end of the heap, 0x22000
 * When naive_malloc is called, it extends the heap by the size required (plus the size of the block header).
 * The break should only be moved once per sbrk call to allocate space, and should remain constant
 * between allocations as long as the allocated sizes fit within the existing space.
 */
void *naive_malloc(size_t size)
{
    static void *heap_end = NULL;
    void *prev_heap_end;
    void *ptr;
    size_t aligned_size;

    if (size == 0)
        return NULL;

    /* Align size to the next page boundary */
    aligned_size = align_size(size + sizeof(size_t));

    /* Initialize heap_end if this is the first call */
    if (heap_end == NULL)
    {
        heap_end = sbrk(0); /* Get current end of heap */
        if (heap_end == (void *)-1)
            return NULL; /* sbrk failed */
    }

    prev_heap_end = heap_end;
    /* Extend heap if not enough space */
    if (sbrk(aligned_size) == (void *)-1)
        return NULL; /* sbrk failed */
    heap_end = (char *)heap_end + aligned_size; /* Update heap_end */

    /* Store block size at the beginning of the block */
    *(size_t *)prev_heap_end = aligned_size;
    
    /* Return pointer to the memory after the block header */
    ptr = (char *)prev_heap_end + sizeof(size_t);

    return ptr;
}