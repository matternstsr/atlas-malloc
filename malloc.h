#ifndef MALLOC_H
#define MALLOC_H
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>

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
 * struct n_header_s - stores header data for each block
 * @total_b: total number of bytes allocated
*/
typedef struct n_header_s
{
	size_t total_b;
} segment_header_t;

/**
 * struct n_heap_s - Struct for storing heap data
 * @first_segment: pointer to first block of metadata
 * @tot_sz: Total size of the heap in bytes
 * @free_sz: Amount of heap free to use in bytes
 * @block_count: The total number of blocks in the heap
*/
typedef struct n_heap_s
{
	segment_header_t *first_segment;
	size_t tot_sz;
	size_t free_sz;
	size_t block_count;
} n_heap_t;

/* Head of the free list */
extern Block *free_list;

/* Function prototypes */
void *naive_malloc(size_t size);
void *_malloc(size_t size);
void _free(void *block_pointer);

void *naive_malloc(size_t size);
segment_header_t *n_move_block(size_t size);

#endif /* MALLOC_H */
