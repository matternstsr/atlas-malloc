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

    /* Align size to the next page boundary, including the size of the block header */
    aligned_size = ALIGN_SIZE(size + sizeof(size_t));

    if (heap_end == NULL)
    {
        heap_end = sbrk(0);  /* Make the heap_end to whatever break is at this time */
        if (heap_end == (void *)-1)
            return NULL; /* sbrk failed */

        /* Allocate initial page size if necessary */
        if (sbrk(PAGE_SIZE) == (void *)-1)
            return NULL; /* sbrk failed */

        heap_end = sbrk(0);  /* Make the heap_end to whatever break is at this time */
    }
    prev_heap_end = heap_end;
    /* Make heap bigger if not enough space  */   
    if (sbrk(aligned_size) == (void *)-1)
        return NULL;  /* sbrk didnt work */
    heap_end = (char *)heap_end + aligned_size;  /* Updating the heap_end */

    /* Store what size is at the beginning of the block */
    *(size_t *)prev_heap_end = aligned_size;
    
    ptr = (char *)prev_heap_end + sizeof(size_t);
    /* Store what size is at the beginning of the block */
    /* *(size_t *)prev_heap_end = aligned_size; */
    /*  moved  */
    return ptr;
}






