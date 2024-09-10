#include "malloc.h"
#include <unistd.h>  /* For sbrk() */

/**
 * static heap_management heap = {NULL, 0, 0, 0, 0};
 * - Static variable to manage heap memory.
 * - Tracks the first block, total size, free size, used size, and block count.
 */
static heap_management heap = {NULL, 0, 0, 0, 0};

/* Define the size of the memory block header */
#define HEADER_SIZE sizeof(memory_block)

/* Define the alignment boundary (e.g., 8 bytes) */
#define ALIGNMENT 8

/**
 * naive_malloc - Allocates memory from a managed space
 * @size: The number of bytes to allocate
 * 
 * Return: Pointer to the allocated memory or NULL if allocation fails
 * 
 * This function allocates memory by adjusting the heap size and tracking
 * allocations. It uses a fixed alignment boundary to ensure proper alignment.
 */
void *naive_malloc(size_t size)
{
    static int initialized = 0;
    size_t aligned_size;
    memory_block *block;

    if (size == 0)
        return NULL;

    /* Calculate aligned size, including space for the block header */
    aligned_size = ((size + HEADER_SIZE + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT;

    /* Initialize the heap if it hasn't been initialized yet */
    if (!initialized)
    {
        heap.first_block = sbrk(0);
        if (heap.first_block == (void *)-1)
            return NULL;  /* sbrk failed */

        heap.total_size = PAGE_SIZE;
        heap.free_size = PAGE_SIZE;

        /* Ensure there's enough space for the initial allocation */
        while (heap.free_size < aligned_size + HEADER_SIZE)
        {
            if (sbrk(PAGE_SIZE) == (void *)-1)
                return NULL;  /* sbrk failed */

            heap.total_size += PAGE_SIZE;
            heap.free_size += PAGE_SIZE;
        }

        /* Set up the first block */
        heap.first_block->size = aligned_size;
        heap.free_size -= (aligned_size + HEADER_SIZE);
        initialized = 1;

        return (void *)(heap.first_block + 1);
    }

    /* Expand memory if needed */
    while (heap.free_size < aligned_size + HEADER_SIZE)
    {
        if (sbrk(PAGE_SIZE) == (void *)-1)
            return NULL;  /* sbrk failed */

        heap.total_size += PAGE_SIZE;
        heap.free_size += PAGE_SIZE;
    }

    /* Find a block that can fit the requested size */
    block = find_block(aligned_size);
    if (block)
    {
        heap.free_size -= (aligned_size + HEADER_SIZE);
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

        /* Move to the next block */
        current_block = (memory_block *)((char *)current_block + HEADER_SIZE + block_size);
    }

    return NULL;  /* No suitable block found */
}
