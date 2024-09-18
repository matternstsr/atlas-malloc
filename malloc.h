#ifndef _MALLOC_H_
#define _MALLOC_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BLOCK_SIZE sizeof(segment_header_t)

/**
 * struct mem_header_s - stores header data for each block
 * @total_b: total number of bytes allocated
*/
typedef struct mem_header_s
{
	size_t total_b;
} mem_hdr_t;

/**
 * struct segment_header_s - stores header data for each block
 * @total_b: total number of bytes allocated
 * @used_bytes: number of bytes used
*/
typedef struct segment_header_s
{
	size_t total_b;
	size_t used_bytes;
} segment_header_t;

/**
 * struct mem_heap_s - Struct for storing heap data
 * @first_segment: pointer to first block of metadata
 * @tot_sz: Total size of the heap in bytes
 * @free_sz: Amount of heap free to use in bytes
 * @block_count: The total number of blocks in the heap
*/
typedef struct mem_heap_s
{
	segment_header_t *first_segment;
	size_t tot_sz;
	size_t free_sz;
	size_t block_count;
} mem_heap_t;

/**
 * struct n_mem_heap_s - Struct for storing heap data
 * @first_segment: pointer to first block of metadata
 * @tot_sz: Total size of the heap in bytes
 * @free_sz: Amount of heap free to use in bytes
 * @block_count: The total number of blocks in the heap
*/
typedef struct n_mem_heap_s
{
	mem_hdr_t *first_segment;
	size_t tot_sz;
	size_t free_sz;
	size_t block_count;
} n_mem_heap_t;

void *naive_malloc(size_t size);
segment_header_t *move_memory_block(size_t size);

void *_malloc(size_t size);
segment_header_t *move_block(size_t size);

void _free(void *ptr);

#endif
