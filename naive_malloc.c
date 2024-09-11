#include "malloc.h"

/* Initialize the free list to NULL */
Block *free_list = NULL;

/**
 * naive_malloc - Allocates memory in the heap
 * @size: size of memory to allocate
 * Return: returns a pointer to the allocated memory
 */
void *naive_malloc(size_t size)
{
    static void *heap_end = NULL;
    void *prev_heap_end;
    Block *current, *prev = NULL;
    size_t aligned_size;

    if (size == 0)
        return NULL;

    aligned_size = ALIGN_SIZE(size); // Align size to the page boundary

    // Search for a suitable block in the free list
    current = free_list;
    while (current) {
        if (current->size >= aligned_size) {
            // Remove the block from the free list
            if (prev)
                prev->next = current->next;
            else
                free_list = current->next;

            // Return a pointer to the memory after the header
            return (char *)current + sizeof(Block);
        }
        prev = current;
        current = current->next;
    }

    // If no suitable block found, allocate new memory
    if (heap_end == NULL)
        heap_end = sbrk(0);

    prev_heap_end = heap_end;
    if (sbrk(aligned_size) == (void *)-1)
        return NULL;

    heap_end = (char *)heap_end + aligned_size;

    // Set the size in the block header
    ((Block *)prev_heap_end)->size = aligned_size;

    // Return a pointer to the memory after the header
    return (char *)prev_heap_end + sizeof(Block);
}

/**
 * naive_free - Frees memory previously allocated by naive_malloc
 * @ptr: pointer to the memory to free
 */
void naive_free(void *ptr)
{
    if (!ptr)
        return;

    Block *block_to_free = (Block *)((char *)ptr - sizeof(Block));

    // Add the block to the beginning of the free list
    block_to_free->next = free_list;
    free_list = block_to_free;
}
