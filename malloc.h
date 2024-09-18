#ifndef MALLOC_H
#define MALLOC_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* start with 4096 page size */
#define PS 4096

/* Make the "size" to the next size up of PS */
#define ALIGN_SIZE(size) (((size) + sizeof(Block) + PS - 1) & ~(PS - 1))

#define BLOCK_SIZE sizeof(mem_block_t)


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


/**
 * struct mem_header_s - stores header data for each block
 * @allocated_bytes: total number of bytes allocated
*/
typedef struct mem_header_s
{
    size_t allocated_bytes;
} mem_header_t;

/**
 * struct mem_block_s - stores header data for each block
 * @allocated_bytes: total number of bytes allocated
 * @used_bytes: number of bytes used
*/
typedef struct mem_block_s
{
    size_t allocated_bytes;
    size_t used_bytes;
} mem_block_t;

/**
 * struct mem_heap_s - Struct for storing heap data
 * @initial_block: pointer to first block of metadata
 * @total_size: Total size of the heap in bytes
 * @free_size: Amount of heap free to use in bytes
 * @block_count: The total number of blocks in the heap
*/
typedef struct mem_heap_s
{
    mem_block_t *initial_block;
    size_t total_size;
    size_t free_size;
    size_t block_count;
} mem_heap_t;

void *simple_malloc(size_t size);
mem_header_t *navigate_block(size_t size);

void *_my_malloc(size_t size);
mem_block_t *move_block(size_t size);

void _my_free(void *ptr);

/* Head of the free list */
extern Block *free_list;

/* Function prototypes */
void *naive_malloc(size_t size);
void *_malloc(size_t size);
void _free(void *ptr);
void *naive_malloc(size_t size);
mem_header_t *navigate_block(size_t size);

#endif /* MALLOC_H */




