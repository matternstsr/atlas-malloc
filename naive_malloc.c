#include "malloc.h"

static void *heap_end = NULL; /* Static variable to maintain the end of the heap */

/**
 * naive_malloc - Allocates memory in the heap
 * @size: size of memory to allocate
 * Return: returns a pointer to the allocated memory
 */
void *naive_malloc(size_t size)
{
    void *prev_heap_end;
    void *ptr;
    size_t aligned_size;

    if (size == 0)
        return NULL;

    /* Align size to the next page boundary */
    aligned_size = ALIGN_SIZE(size + sizeof(size_t));

    /* Initialize heap if it is the first call */
    if (heap_end == NULL)
    {
        heap_end = sbrk(0); /* Get the current end of the heap */
        if (heap_end == (void *)-1)
            return NULL; /* sbrk failed */

        /* Allocate initial page size if necessary */
        if (sbrk(PAGE_SIZE) == (void *)-1)
            return NULL; /* sbrk failed */

        heap_end = sbrk(0); /* Update heap_end to the end of the allocated space */
    }

    prev_heap_end = heap_end;

    /* Extend heap if not enough space */
    if (sbrk(aligned_size) == (void *)-1)
        return NULL; /* sbrk failed */

    heap_end = (char *)heap_end + aligned_size; /* Update heap_end */

    /* Store block size at the beginning of the block */
    *(size_t *)prev_heap_end = size;

    /* Return pointer to the memory after the block header */
    ptr = (char *)prev_heap_end + sizeof(size_t);
    return ptr;
}
