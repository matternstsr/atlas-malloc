#include "malloc.h"
#include <stdio.h>  // For debugging purposes

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
    aligned_size = ALIGN_SIZE(size + sizeof(Block));

    if (heap_end == NULL)
        heap_end = sbrk(0);  /* Initialize heap_end to the current end of the heap */

    prev_heap_end = heap_end;
    if (sbrk(aligned_size) == (void *)-1)
        return NULL;  /* sbrk failed */

    heap_end = (char *)heap_end + aligned_size;  /* Update the heap_end */

    /* Initialize the block header */
    Block *block = (Block *)prev_heap_end;
    block->size = aligned_size;
    block->next = NULL;

    /* Return the memory after the block header */
    ptr = (char *)block + sizeof(Block);
    
    /* Debugging output */
    printf("%p -> [%s]\n", ptr, "Holberton");
    printf("Current break is %p\n", heap_end);

    return ptr;
}
