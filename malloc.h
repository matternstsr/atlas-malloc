#ifndef MALLOC_H
#define MALLOC_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PS 4096
#define ALIGN_SIZE(size) (((size) + sizeof(Block) + PS - 1) & ~(PS - 1))

typedef struct Block {
    size_t size;
    struct Block *next;
} Block;

typedef struct mem_block_s {
    size_t allocated_bytes;
    size_t used_bytes;
} mem_block_t;

typedef struct mem_heap_s {
    mem_block_t *initial_block;
    size_t total_size;
    size_t free_size;
    size_t block_count;
} mem_heap_t;

void *naive_malloc(size_t size);
mem_block_t *navigate_block(size_t size);  // Ensure this matches

#endif /* MALLOC_H */
