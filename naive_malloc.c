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
    size_t total_size;

    if (size == 0)
        return NULL;

    // Calculate the total size needed, including block header and alignment
    total_size = size + sizeof(Block);
    
    // Align total size to the next 8-byte boundary
    total_size = (total_size + 7) & ~7;

    if (heap_end == NULL)
        heap_end = sbrk(0);  /* Initialize heap_end to the current end of the heap */

    prev_heap_end = heap_end;
    if (sbrk(total_size) == (void *)-1)
        return NULL;  /* sbrk failed */

    heap_end = (char *)heap_end + total_size;  /* Update the heap_end */

    /* Initialize the block header */
    Block *block = (Block *)prev_heap_end;
    block->size = total_size;
    block->next = NULL;

    /* Return the memory after the block header */
    ptr = (char *)block + sizeof(Block);

    return ptr;
}
