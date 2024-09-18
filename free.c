#include "malloc.h"

/**
 * Frees the allocated memory pointed to by ptr.
 * This function adds the block back to the free list.
 *
 * @param ptr Pointer to the memory block to be freed.
 */
void _free(void *ptr)
{
    if (ptr == NULL)
        return;

    Block *block = (Block *)((char *)ptr - sizeof(Block));
    block->next = free_list;
    free_list = block;
}