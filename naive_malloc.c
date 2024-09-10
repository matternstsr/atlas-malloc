#include "malloc.h"

/**
 * naive_malloc - Allocates memory in the heap
 * @size: size of memory to allocate
 * Return: returns a pointer to the allocated memory
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
    aligned_size = ALIGN_SIZE(size + sizeof(size_t));

    if (heap_end == NULL) {
        heap_end = sbrk(0);  /* Make the heap_end to whatever break is at this time */
    }

    prev_heap_end = heap_end;

    /* Extend the heap */
    if (sbrk(aligned_size) == (void *)-1)
        return NULL;  /* sbrk failed */

    heap_end = (char *)heap_end + aligned_size;  /* Update the heap_end */

    /* Store the size of the block at the beginning */
    *(size_t *)prev_heap_end = aligned_size;

    /* Return a pointer to the memory area just after the header */
    ptr = (char *)prev_heap_end + sizeof(size_t);

    return ptr;
}
