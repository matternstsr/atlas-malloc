#include "malloc.h"

static void *heap_end = NULL;


void *naive_malloc(size_t size)
{
    void *prev_heap_end;
    void *ptr;
    size_t aligned_size;

    if (size == 0)
        return NULL;

    /* Align size to the next page boundary, including the size of the block header */
    aligned_size = ALIGN_SIZE(size + sizeof(Block));

    /* Initialize the heap if it is the first call */
    if (heap_end == NULL)
    {
        heap_end = sbrk(0);
        if (heap_end == (void *)-1)
            return NULL; /* sbrk failed */

        Add initial heap space
        if (sbrk(PAGE_SIZE) == (void *)-1)
            return NULL; /* sbrk failed */

        heap_end = sbrk(0);
    }

    prev_heap_end = heap_end;
    /* Extend heap if there is not enough space */
    if (sbrk(aligned_size) == (void *)-1)
        return NULL; /* sbrk failed
 */
    heap_end = (char *)heap_end + aligned_size; /* Update heap_end */

    /* Initialize the new block */
    Block *new_block = (Block *)prev_heap_end;
    new_block->size = size;
    new_block->next = NULL;

    /* Return the memory after the block header */
    ptr = (char *)new_block + sizeof(Block);
    return ptr;
}

void _free(void *ptr)
{
    if (ptr == NULL)
        return;

    /* Get the block header */
    Block *block = (Block *)((char *)ptr - sizeof(Block));

    /* Add the block to the free list */
    block->next = free_list;
    free_list = block;
}
