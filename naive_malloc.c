#include "malloc.h"

#define MEMORY_POOL_SIZE 1024  // Adjust size as needed for your system

static uint8_t memory_pool[MEMORY_POOL_SIZE];
/* static uint8_t *pool_end = memory_pool;
static uint8_t *pool_current = memory_pool; */

/**
* naive_malloc - Allocates memory in the heap
* @size: size of memory to allocate
* Return: returns a pointer to the allocated memory
*/
void *naive_malloc(size_t size)
{
    static uint8_t *heap_end = NULL;
    uint8_t *prev_heap_end;
    uint8_t *ptr;
    size_t aligned_size = ((size + sizeof(Block) + 7) / 8) * 8;

    if (size == 0)
        return NULL;

    if (heap_end == NULL)
        heap_end = pool_current;

    prev_heap_end = heap_end;

    if (heap_end + aligned_size > memory_pool + MEMORY_POOL_SIZE)
        return NULL;  // Out of memory

    heap_end += aligned_size;
    ptr = prev_heap_end;

    return (void *)ptr;
}
