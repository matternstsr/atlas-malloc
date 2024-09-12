#include "malloc.h"

/* #define MEMORY_POOL_SIZE 1024  // Adjust size as needed for your system */

/**
* naive_malloc - Allocates memory in the heap
* @size: size of memory to allocate
* Return: returns a pointer to the allocated memory
*/
void *naive_malloc(size_t size)
{
    static void *heap_end;
    void *prev_heap_end;
    void *ptr;
    size_t aligned_sz = ((size + sizeof(Block_n) + 7) / 8) * 8;
    
    if (size == 0)
        return NULL;
    
    if (heap_end == NULL)
        heap_end = sbrk(0);
    
    prev_heap_end = heap_end;
    
    if (sbrk(aligned_sz) == (void *)-1)
        return NULL;
    
    heap_end = sbrk(0);
    
    ptr = prev_heap_end;
    
    // Initialize the Block_n header
    Block_n *block_header = (Block_n *)ptr;
    block_header->size = aligned_sz;
    
    // Return pointer to the memory after the header
    return (void *)((char *)ptr + sizeof(Block_n));
}

