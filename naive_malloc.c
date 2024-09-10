#include "malloc.h"

/* Static variable for managing the free list */
static Block *free_list = NULL;

/**
 * naive_malloc - Allocates memory in the heap
 * @size: size of memory to allocate
 * Return: returns a pointer to the allocated memory
 */
void *naive_malloc(size_t size) {
    static void *heap_end = NULL;
    Block *current, *prev = NULL;
    size_t aligned_size;

    if (size == 0) return NULL;

    /* Align size to 8 bytes */
    aligned_size = ALIGN_SIZE(size + sizeof(Block));

    /* Check free list for a suitable block */
    current = free_list;
    while (current) {
        if (current->free && current->size >= aligned_size) {
            current->free = 0; // Mark block as used
            return (char *)current + sizeof(Block);
        }
        prev = current;
        current = current->next;
    }

    /* Initialize heap_end if this is the first call */
    if (heap_end == NULL) {
        heap_end = sbrk(0);
        if (heap_end == (void *)-1) return NULL;
    }

    /* Extend the heap */
    if (sbrk(aligned_size) == (void *)-1) return NULL;
    heap_end = (char *)heap_end + aligned_size;

    /* Initialize the new block */
    Block *new_block = (Block *)((char *)heap_end - aligned_size);
    new_block->size = aligned_size;
    new_block->free = 0;
    new_block->next = NULL;

    /* Update the free list */
    if (prev) {
        prev->next = new_block;
    } else {
        free_list = new_block;
    }

    return (char *)new_block + sizeof(Block);
}

/**
 * naive_free - Frees previously allocated memory
 * @ptr: pointer to the memory to free
 */
void naive_free(void *ptr) {
    if (!ptr) return;

    Block *block_to_free = (Block *)((char *)ptr - sizeof(Block));
    block_to_free->free = 1;

    /* Optionally, coalesce adjacent free blocks here */
}
