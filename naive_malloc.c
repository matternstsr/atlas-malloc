#include "malloc.h"

void *naive_malloc(size_t size)
{
    static void *heap_end = NULL;
    void *prev_heap_end;
    void *ptr;
    size_t aligned_size;

    if (size == 0)
        return NULL;

    aligned_size = ALIGN_SIZE(size);

    if (heap_end == NULL)
        heap_end = sbrk(0);  /* Make the heap_end to whatever break is at this time */

    prev_heap_end = heap_end;
    if (sbrk(aligned_size) == (void *)-1)
        return NULL;  /* sbrk didnt work */
    heap_end = (char *)heap_end + aligned_size;  /* Updating the heap_end */

    ptr = (char *)prev_heap_end + sizeof(size_t);
    /* Store what size is at the beginning of the block */
    *(size_t *)prev_heap_end = aligned_size;
    return ptr;
}
