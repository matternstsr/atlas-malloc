#include "malloc.h"

Block *free_list = NULL;

void *naive_malloc(size_t size)
{
    if (size == 0)
        return NULL;

    Block *prev = NULL;
    Block *current = free_list;
    size_t aligned_size = ALIGN_SIZE(size);

    // Search the free list for a suitable block
    while (current != NULL)
    {
        if (current->size >= aligned_size)
        {
            if (current->size > aligned_size + sizeof(Block))
            {
                // Split the block
                Block *new_block = (Block *)((char *)current + aligned_size);
                new_block->size = current->size - aligned_size;
                new_block->next = current->next;
                current->size = aligned_size;
                current->next = new_block;
            }
            if (prev)
                prev->next = current->next;
            else
                free_list = current->next;

            return (char *)current + sizeof(size_t);
        }
        prev = current;
        current = current->next;
    }

    // Request more memory from the system
    void *ptr = sbrk(aligned_size);
    if (ptr == (void *)-1)
        return NULL;

    Block *new_block = (Block *)ptr;
    new_block->size = aligned_size;
    new_block->next = NULL;

    return (char *)new_block + sizeof(size_t);
}

void _free(void *ptr)
{
    if (!ptr)
        return;

    Block *block = (Block *)((char *)ptr - sizeof(size_t));
    block->next = free_list;
    free_list = block;
}
