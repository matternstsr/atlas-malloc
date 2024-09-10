#include "malloc.h"

void *naive_malloc(size_t size)
{
    static void *heap_end = NULL;
    void *prev_heap_end;
    Block *block, *prev = NULL;
    size_t aligned_size;

    if (size == 0)
        return NULL;

    /* Align size to the next page boundary, including the size of the block header */
    aligned_size = ALIGN_SIZE(size + sizeof(size_t));

    /* Check if the free list has a good sized usable block */
    for (block = free_list; block != NULL; block = block->next)
    {
        if (block->size >= aligned_size)
        {
            /* Good sized usable block found */
            if (block->size > aligned_size + sizeof(Block))
            {
                /* Split the block if too big */
                Block *remaining_block = (Block *)((char *)block + aligned_size + sizeof(Block));
                remaining_block->size = block->size - aligned_size - sizeof(Block);
                remaining_block->next = block->next;
                block->size = aligned_size;
                block->next = remaining_block;
            }
            else
            {
                /* Remove the block from the free list */
                if (prev == NULL)
                    free_list = block->next;
                else
                    prev->next = block->next;
            }
            return (char *)block + sizeof(Block);
        }
        prev = block;
    }

    /* No suitable block found, make the heap larger */
    if (heap_end == NULL)
        heap_end = sbrk(0);  /* Make the heap_end to whatever break is at this time */

    prev_heap_end = heap_end;
    if (sbrk(aligned_size) == (void *)-1)
        return NULL;  /* sbrk didnt work */
    heap_end = (char *)heap_end + aligned_size;  /* Updating the heap_end */

    /* Create a new block and return its usable portion */
    block = (Block *)prev_heap_end;
    block->size = aligned_size;
    return (char *)block + sizeof(Block);
}

void naive_free(void *ptr)
{
    if (ptr == NULL)
        return;

    Block *block = (Block *)((char *)ptr - sizeof(Block));
    block->next = free_list;
    free_list = block;
}






