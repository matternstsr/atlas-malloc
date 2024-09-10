#include "malloc.h"

static Block *free_list = NULL;  The free list for managing free blocks

void *naive_malloc(size_t size)
{
    static void *heap_end = NULL;
    void *prev_heap_end;
    Block *block, *prev = NULL;
    size_t aligned_size;

    if (size == 0)
        return NULL;

    aligned_size = ALIGN_SIZE(size);

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
                /* Take out the block from the free list */
                if (prev == NULL)
                    free_list = block->next;
                else
                    prev->next = block->next;
            }
            return (char *)block + sizeof(Block);
        }
        prev = block;
    }

    /* No good block found, make the heap larger */
    if (heap_end == NULL)
        heap_end = sbrk(0);  /* Set if first use */

    prev_heap_end = heap_end;
    if (sbrk(aligned_size + sizeof(Block)) == (void *)-1)
        return NULL;  /* sbrk failed */
    heap_end = (char *)heap_end + aligned_size + sizeof(Block);

    /* Make a new block and use the good part of it */
    block = (Block *)prev_heap_end;
    block->size = aligned_size;
    return (char *)block + sizeof(Block);
}
