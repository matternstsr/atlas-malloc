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

    // Traverse the blocks to find a suitable space
    n_header_t *current = heap.first_block;
    for (size_t i = 0; i < heap.total_blocks; i++) {
        size_t total_bytes = current->total_bytes;

        // Check if this block has enough space
        if (total_bytes >= aligned_size) {
            // If enough space, split the block if there's leftover space
            if (total_bytes >= aligned_size + sizeof(n_header_t) + 8) { // Ensure we can split
                n_header_t *next_block = (n_header_t *)((char *)current + sizeof(n_header_t) + aligned_size);
                next_block->total_bytes = total_bytes - aligned_size - sizeof(n_header_t);
                current->total_bytes = aligned_size;
            }
            return (void *)((char *)current + sizeof(n_header_t));
        }

        // Move to the next block
        current = (n_header_t *)((char *)current + sizeof(n_header_t) + total_bytes);
    }

    // No suitable block found, expand the heap
    void *new_block = sbrk(aligned_size + sizeof(n_header_t));
    if (new_block == (void *)-1) return NULL; // sbrk failed

    // Update heap metadata
    heap.heap_size += aligned_size + sizeof(n_header_t);
    heap.heap_free -= aligned_size + sizeof(n_header_t);

    // Create the new block
    current = (n_header_t *)new_block;
    current->total_bytes = aligned_size;

    heap.total_blocks++;
    return (void *)((char *)current + sizeof(n_header_t));
}
