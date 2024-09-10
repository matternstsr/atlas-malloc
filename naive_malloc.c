#include "malloc.h"

/* Static variables for heap management */
static Block *free_list = NULL;
static void *heap_end = NULL;

/**
 * naive_malloc - Allocates memory in the heap
 * @size: size of memory to allocate
 * Return: returns a pointer to the allocated memory
 */
void *naive_malloc(size_t size) {
    if (size == 0) return NULL;

    size_t aligned_size = ALIGN_SIZE(size + BLOCK_HEADER_SIZE);
    Block *current = free_list;

    /* Search free list for a suitable block */
    while (current) {
        if (current->free && current->size >= aligned_size) {
            /* Found a suitable block */
            if (current->size > aligned_size + BLOCK_HEADER_SIZE) {
                /* Split the block if it's larger than needed */
                Block *new_block = (Block *)((char *)current + aligned_size);
                new_block->size = current->size - aligned_size;
                new_block->next = current->next;
                new_block->free = 1;
                current->size = aligned_size;
                current->next = new_block;
            }
            current->free = 0;
            return (char *)current + BLOCK_HEADER_SIZE;
        }
        prev = current;
        current = current->next;
    }

    /* No suitable block found, extend the heap */
    size_t page_size = getpagesize();
    size_t total_size = aligned_size > page_size ? aligned_size : page_size;
    void *new_heap_end = sbrk(total_size);

    if (new_heap_end == (void *)-1) return NULL;

    if (heap_end == NULL) {
        heap_end = new_heap_end;
    }

    /* Initialize the new block */
    Block *new_block = (Block *)heap_end;
    new_block->size = total_size;
    new_block->next = NULL;
    new_block->free = 0;

    heap_end = (char *)heap_end + total_size;

    return (char *)new_block + BLOCK_HEADER_SIZE;
}

/**
 * naive_free - Frees previously allocated memory
 * @ptr: pointer to the memory to free
 */
void naive_free(void *ptr) {
    if (!ptr) return;

    Block *block_to_free = (Block *)((char *)ptr - BLOCK_HEADER_SIZE);
    block_to_free->free = 1;

    /* Coalesce adjacent free blocks */
    Block *current = free_list;
    Block *prev = NULL;

    while (current && current < block_to_free) {
        prev = current;
        current = current->next;
    }

    if (prev) {
        prev->next = block_to_free;
    } else {
        free_list = block_to_free;
    }

    block_to_free->next = current;

    /* Attempt to coalesce free blocks */
    if (block_to_free->next && block_to_free->next->free) {
        block_to_free->size += BLOCK_HEADER_SIZE + block_to_free->next->size;
        block_to_free->next = block_to_free->next->next;
    }
}
