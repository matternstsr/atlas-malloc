#ifndef MALLOC_H
#define MALLOC_H
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* start with 4096 page size */
#define PS 4096

/* Make the "size" to the next size up of PS */
#define ALIGN_SIZE(size) (((size) + sizeof(Block) + PS - 1) & ~(PS - 1))


/**
* struct Block - stores metadata for each memory block
* @size: total number of bytes allocated for the block (including header)
* @next: number of bytes used by the user (excluding header)
*/
typedef struct Block
{
	size_t size;          /* Size of the block, including the header */
	struct Block *next;  /* Pointer to the next free block in the free list */
} Block;

/**
* struct Block - stores metadata for each memory block
* @size: total number of bytes allocated for the block (including header)
* @next: number of bytes used by the user (excluding header)
*/
typedef struct Block_n
{
	size_t size;          /* Size of the block, including the header */
} Block_n;


typedef struct n_header_s {
    size_t total_bytes; // Total bytes allocated (including this header)
} n_header_t;

typedef struct n_heap_s {
    n_header_t *first_block; // Pointer to first block of metadata
    size_t heap_size;        // Total size of the heap in bytes
    size_t heap_free;        // Amount of heap free to use in bytes
    size_t total_blocks;     // The total number of blocks in the heap
} n_heap_t;

void *naive_malloc(size_t size);
n_header_t *n_move_block(size_t size);


/* Head of the free list */
extern Block *free_list;

/* Function prototypes */
void *naive_malloc(size_t size);
void *_malloc(size_t size);
void _free(void *ptr);

#endif /* MALLOC_H */




