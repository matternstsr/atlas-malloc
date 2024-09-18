#include "malloc.h"

static mem_heap_t memory_heap = {NULL, 0, 0, 0};

/**
 * naive_malloc - Allocates memory in the heap
 * @requested_size: size of memory to allocate
 * Return: returns a pointer to the allocated memory
*/

void *naive_malloc(size_t requested_size)
{
    static int initialized;
    size_t adjusted_size = ((requested_size + 7) / 8) * 8;
    mem_header_t *block_ptr = NULL;

    if (!initialized)
    {
        memory_heap.initial_block = sbrk(0);
        while (memory_heap.total_size < (adjusted_size + 8))
        {
            sbrk(getpagesize());
            memory_heap.total_size += getpagesize();
            memory_heap.free_size += getpagesize();
        }
        memory_heap.initial_block->allocated_bytes = adjusted_size;
        memory_heap.free_size = memory_heap.total_size - (8 + adjusted_size);
        memory_heap.block_count = 1;
        block_ptr = memory_heap.initial_block + 1;
        initialized = 1;
        return ((void *) block_ptr);
    }
    while ((adjusted_size + 8) > memory_heap.free_size)
    {
        sbrk(getpagesize());
        memory_heap.total_size += getpagesize();
        memory_heap.free_size += getpagesize();
    }
    block_ptr = navigate_block((8 + adjusted_size));
    memory_heap.block_count++;
    memory_heap.free_size -= (8 + adjusted_size);
    return ((void *) ++block_ptr);
}


/**
 * navigate_block - Function for traversing the header blocks
 * @size: Size user requests plus block header
 * Return: pointer to big enough chunk for size
*/
mem_header_t *navigate_block(size_t size)
{
    size_t index = 0, total_allocated = 0;
    mem_header_t *current_block;

    current_block = memory_heap.initial_block;
    for (; index < memory_heap.block_count; index++)
    {
        total_allocated = current_block->allocated_bytes;
        current_block = (mem_header_t *)((char *)current_block + sizeof(mem_header_t) + total_allocated);
    }
    current_block->allocated_bytes = size - 8;
    return (current_block);
}
