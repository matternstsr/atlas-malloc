#include "malloc.h"

/* Define the start of the heap */
static void *start_of_heap = NULL;
static void *heap_end = NULL;

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
    size_t space_available;

    if (size == 0)
        return NULL;

    /* Align size to the next page boundary, including the size of the block header */
    aligned_size = ALIGN_SIZE(size + sizeof(size_t));

    /* Initialize heap_end and start_of_heap */
    if (heap_end == NULL)
    {
        heap_end = sbrk(0);
        start_of_heap = heap_end;
    }

    /* Check if there's enough space from start_of_heap to heap_end */
    space_available = (size_t)((char *)heap_end - (char *)start_of_heap);
    if (space_available >= aligned_size)
    {
        /* Allocate from the beginning of the heap */
        prev_heap_end = start_of_heap;
        start_of_heap = (char *)start_of_heap + aligned_size;  /* Update start_of_heap */

        /* Store the size at the beginning of the block */
        *(size_t *)prev_heap_end = aligned_size;

        ptr = (char *)prev_heap_end + sizeof(size_t);
        return ptr;
    }

    /* If not enough space at the beginning, use sbrk to extend the heap */
    prev_heap_end = heap_end;
    if (sbrk(aligned_size) == (void *)-1)
        return NULL;  /* sbrk didn't work */

    heap_end = (char *)heap_end + aligned_size;  /* Update heap_end */

    /* Store the size at the beginning of the block */
    *(size_t *)prev_heap_end = aligned_size;

    ptr = (char *)prev_heap_end + sizeof(size_t);
    return ptr;
}
