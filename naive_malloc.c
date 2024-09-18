#include "malloc.h"

static n_heap_t heap = {NULL, 0, 0, 0};

/**
 * naive_malloc - Allocates memory in the heap
 * @size: size of memory to allocate
 * Return: returns a pointer to the allocated memory
 */
void *naive_malloc(size_t size) {
    if (size == 0) return NULL;

    // Align the requested size
    size_t aligned_size = (size + sizeof(n_header_t) + 7) & ~7; // Round up to nearest multiple of 8

    // First allocation
    if (heap.first_block == NULL) {
        heap.first_block = sbrk(aligned_size + sizeof(n_header_t));
        if (heap.first_block == (void *)-1) return NULL; // sbrk failed
        heap.first_block->total_bytes = aligned_size;
        heap.heap_size = aligned_size + sizeof(n_header_t);
        heap.heap_free = heap.heap_size - (aligned_size + sizeof(n_header_t));
        heap.total_blocks = 1;
        return (void *)((char *)heap.first_block + sizeof(n_header_t));
    }

    // Ensure enough space in the heap
    while (aligned_size > heap.heap_free) {
        sbrk(getpagesize());
        heap.heap_size += getpagesize();
        heap.heap_free += getpagesize();
    }

    // Traverse the blocks to find a suitable space
    n_header_t *current = heap.first_block;
    for (size_t i = 0; i < heap.total_blocks; i++) {
        size_t total_bytes = current->total_bytes;

        // Check if this block has enough space
        if (total_bytes >= aligned_size) {
            // Adjust the current block's size
            current->total_bytes = total_bytes - aligned_size - sizeof(n_header_t);
            n_header_t *next_block = (n_header_t *)((char *)current + sizeof(n_header_t) + aligned_size);
            next_block->total_bytes = aligned_size;

            return (void *)((char *)next_block + sizeof(n_header_t));
        }

        // Move to the next block
        current = (n_header_t *)((char *)current + sizeof(n_header_t) + total_bytes);
    }

    // No suitable block found, expand the heap
    sbrk(aligned_size + sizeof(n_header_t));
    heap.heap_size += aligned_size + sizeof(n_header_t);
    heap.heap_free -= aligned_size + sizeof(n_header_t);

    // Create the new block
    current = (n_header_t *)((char *)heap.first_block + heap.heap_size - (aligned_size + sizeof(n_header_t)));
    current->total_bytes = aligned_size;

    heap.total_blocks++;
    return (void *)((char *)current + sizeof(n_header_t));
}

/**
 * n_move_block - Function for traversing the header blocks
 * @size: Size user requests plus block header
 * Return: pointer to big enough chunk for size
 */
n_header_t *n_move_block(size_t size) {
    n_header_t *current = heap.first_block;
    for (size_t i = 0; i < heap.total_blocks; i++) {
        if (current->total_bytes >= size) {
            return current;
        }
        current = (n_header_t *)((char *)current + sizeof(n_header_t) + current->total_bytes);
    }
    return NULL; // No suitable block found
}

