#include "malloc.h"

/* Initialize the free list head */
Block *free_list = NULL;

/**
 * naive_malloc - Allocates memory in the heap
 * @size: size of memory to allocate
 * Return: returns a pointer to the allocated memory
 */
void *naive_malloc(size_t size)
{
    if (size == 0)
        return NULL;

    Block *current = free_list;
    Block *prev = NULL;
    size_t aligned_size = ALIGN_SIZE(size);

    /* Search for a suitable free block */
    while (current != NULL)
    {
        if (current->size >= aligned_size)
        {
            if (current->size > aligned_size + sizeof(Block))
            {
                /* Split the block */
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

    /* No suitable block found, request more memory from the OS */
    void *ptr = sbrk(aligned_size);
    if (ptr == (void *)-1)
        return NULL;

    Block *new_block = (Block *)ptr;
    new_block->size = aligned_size;
    new_block->next = NULL;

    return (char *)new_block + sizeof(size_t);
}

/**
 * _free - Frees the allocated memory
 * @ptr: pointer to the memory to free
 */
void _free(void *ptr)
{
    if (!ptr)
        return;

    Block *block = (Block *)((char *)ptr - sizeof(size_t));
    block->next = free_list;
    free_list = block;
}
