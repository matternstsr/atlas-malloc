#include "malloc.h"
#include <unistd.h>  /* For sbrk() */

/**
 * static heap_management heap = {NULL, 0, 0, 0, 0};
 * - Static variable to manage heap memory.
 * - Tracks the first block, total size, free size, used size, and block count.
 */
static heap_management heap = {NULL, 0, 0, 0, 0};

/**
 * native_malloc - Allocates memory from a managed space
 * @size: The number of bytes to allocate
 * 
 * Return: Pointer to the allocated memory or NULL if allocation fails
 * 
 * This function allocates memory by adjusting the heap size and tracking
 * allocations. It uses a fixed page size for alignment and ensures that
 * allocations fit within the current heap.
 */
void *native_malloc(size_t size)
{
    static int initialized = 0;
    size_t aligned_size;
    memory_block *block;

    if (size == 0)
        return NULL;

    /* Define a fixed page size constant (e.g., 4096 bytes) */
    const size_t page_size = PAGE_SIZE;

    /* Calculate aligned size, including space for the block header */
    aligned_size = ((size + sizeof(memory_block) + page_size - 1) / page_size) * page_size;

    /* Initialize the heap if it hasn't been initialized yet */
    if (!initialized)
    {
        heap.first_block = sbrk(0);
        heap.total_size = page_size;
        heap.free_size = heap.total_size;

        /* Ensure there's enough space for the initial allocation */
        while (heap.free_size < aligned_size + sizeof(memory_block))
        {
            sbrk(page_size);
            heap.total_size += page_size;
            heap.free_size = heap.total_size;
        }

        /* Set up the first block */
        heap.first_block->size = aligned_size;
        heap.free_size -= (aligned_size + sizeof(memory_block));
        initialized = 1;

        return (void *)(heap.first_block + 1);
    }

    /* Expand memory if needed */
    while (heap.free_size < aligned_size + sizeof(memory_block))
    {
        sbrk(page_size);
        heap.total_size += page_size;
        heap.free_size += page_size;
    }

    /* Find a block that can fit the requested size */
    block = find_block(aligned_size);
    if (block)
    {
        heap.free_size -= (aligned_size + sizeof(memory_block));
        return (void *)(block + 1);
    }

    return NULL;  /* No suitable block found */
}

/**
 * find_block - Searches for a block in memory that fits the requested size
 * @size: The size needed, including the block header
 * 
 * Return: Pointer to the block that meets the size requirement or NULL if not found
 * 
 * This function traverses the list of memory blocks to find one that is
 * large enough to satisfy the allocation request.
 */
memory_block *find_block(size_t size)
{
    memory_block *current_block = heap.first_block;
    size_t block_size;

    while (current_block)
    {
        block_size = current_block->size;
        if (block_size >= size)
            return current_block;

        current_block = (memory_block *)((char *)current_block + sizeof(memory_block) + block_size);
    }

    return NULL;  /* No suitable block found */
}
