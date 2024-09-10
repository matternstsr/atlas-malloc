#include "malloc.h"

void *naive_malloc(size_t size)
{
    static void *heap_end = NULL;
    void *prev_heap_end;
    void *ptr;
    size_t aligned_size;

    if (size == 0)
        return NULL;

    /* Align size to the next page boundary, including the size of the block header */
    aligned_size = ALIGN_SIZE(size + sizeof(size_t));

    if (heap_end == NULL)
        heap_end = sbrk(0);  /* Make the heap_end to whatever break is at this time */

    prev_heap_end = heap_end;
    if (sbrk(aligned_size) == (void *)-1)
        return NULL;  /* sbrk didn't work */
    
    /* Store the size of the block at the beginning of the allocated block */
    *(size_t *)prev_heap_end = size;

    /* Move heap_end to the new end of the heap */
    heap_end = (char *)prev_heap_end + aligned_size;

    /* Return a pointer to the memory immediately after the block size */
    ptr = (char *)prev_heap_end + sizeof(size_t);
    
    return ptr;
}
