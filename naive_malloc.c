#include "malloc.h"

/* Initialize the free list */
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

    size_t aligned_size = ALIGN_SIZE(size);
    Block *prev = NULL;
    Block *curr = free_list;

    /* Search for a suitable free block */
    while (curr != NULL) {
        if (curr->size >= aligned_size) {
            /* Remove the block from the free list */
            if (prev == NULL) {
                free_list = curr->next;
            } else {
                prev->next = curr->next;
            }

            /* Return the block */
            return (char *)curr + sizeof(size_t);
        }
        prev = curr;
        curr = curr->next;
    }

    /* No suitable block found, extend the heap */
    void *heap_end = sbrk(0);
    void *prev_heap_end = heap_end;
    if (sbrk(aligned_size) == (void *)-1)
        return NULL;  /* sbrk failed */

    /* Store the block size in the header */
    *(size_t *)prev_heap_end = aligned_size;

    return (char *)prev_heap_end + sizeof(size_t);
}

/**
 * naive_free - Frees previously allocated memory
 * @ptr: pointer to the memory to free
 */
void naive_free(void *ptr)
{
    if (ptr == NULL)
        return;

    /* Get the block header */
    Block *block = (Block *)((char *)ptr - sizeof(size_t));
    block->next = free_list;
    free_list = block;
}
