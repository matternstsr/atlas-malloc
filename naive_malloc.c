#include "malloc.h"

static mem_heap_t memory_heap = {NULL, 0, 0, 0};

void *naive_malloc(size_t requested_size)
{
    static int initialized;
    size_t adjusted_size = ((requested_size + 7) / 8) * 8;
    mem_block_t *block_ptr = NULL;

    if (!initialized)
    {
        memory_heap.initial_block = sbrk(0);
        while (memory_heap.total_size < (adjusted_size + sizeof(mem_block_t)))
        {
            sbrk(getpagesize());
            memory_heap.total_size += getpagesize();
            memory_heap.free_size += getpagesize();
        }
        memory_heap.initial_block->allocated_bytes = adjusted_size;
        memory_heap.free_size = memory_heap.total_size - (sizeof(mem_block_t) + adjusted_size);
        memory_heap.block_count = 1;
        block_ptr = memory_heap.initial_block;
        initialized = 1;
        return ((void *) block_ptr);
    }
    
    while ((adjusted_size + sizeof(mem_block_t)) > memory_heap.free_size)
    {
        sbrk(getpagesize());
        memory_heap.total_size += getpagesize();
        memory_heap.free_size += getpagesize();
    }
    
    block_ptr = navigate_block((sizeof(mem_block_t) + adjusted_size));
    memory_heap.block_count++;
    memory_heap.free_size -= (sizeof(mem_block_t) + adjusted_size);
    return ((void *) ++block_ptr);
}

mem_block_t *navigate_block(size_t size)
{
    size_t index = 0, total_allocated = 0;
    mem_block_t *current_block = memory_heap.initial_block;

    for (; index < memory_heap.block_count; index++)
    {
        total_allocated = current_block->allocated_bytes;
        current_block = (mem_block_t *)((char *)current_block + sizeof(mem_block_t) + total_allocated);
    }
    current_block->allocated_bytes = size - sizeof(mem_block_t);
    return (current_block);
}
