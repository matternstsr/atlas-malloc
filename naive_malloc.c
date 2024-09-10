#include "malloc.h"

/* Internal function to handle allocation */
static void *naive_malloc_internal(size_t size, Block **free_list_ptr)
{
    static void *heap_end = NULL;
    void *prev_heap_end;
    Block *block, *prev = NULL, *best_fit = NULL;
    size_t aligned_size;

    if (size == 0)
        return NULL;

    aligned_size = ALIGN_SIZE(size);

    /* Check the free list for a good block that fits */
    for (block = *free_list_ptr; block != NULL; block = block->next)
    {
        if (block->size >= aligned_size)
        {
            best_fit = block;
            break;
        }
    }

    if (best_fit != NULL)
    {
        /* Split the block up if it's larger than needed */
        if (best_fit->size > aligned_size + sizeof(Block))
        {
            Block *remaining_block = (Block *)((char *)best_fit + aligned_size + sizeof(Block));
            remaining_block->size = best_fit->size - aligned_size - sizeof(Block);
            remaining_block->next = best_fit->next;
            best_fit->size = aligned_size;
            best_fit->next = remaining_block;
        }
        else
        {
            /* Take out the block from the free list */
            if (prev == NULL)
                *free_list_ptr = best_fit->next;
            else
                prev->next = best_fit->next;
        }
        return (char *)best_fit + sizeof(Block);
    }

    if (heap_end == NULL)
        heap_end = sbrk(0);
    /* Make the heap_end to whatever break is at this time */

    prev_heap_end = heap_end;
    if (sbrk(aligned_size + sizeof(Block)) == (void *)-1)
        return NULL;  /* sbrk didnt work */
    heap_end = (char *)heap_end + aligned_size + sizeof(Block);
    /* Updating the heap_end */

    /* Make the new block */
    block = (Block *)prev_heap_end;
    block->size = aligned_size;
    return (char *)block + sizeof(Block);
}

/* Allocate memory */
void *naive_malloc(size_t size)
{
    static Block *free_list = NULL;  /* Static var for free list */
    return naive_malloc_internal(size, &free_list);
}
