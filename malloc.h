#ifndef MALLOC_H
#define MALLOC_H

#include <unistd.h>
#include <stddef.h>
#include <stdint.h>

/* Align size to the next page boundary */
#define PAGE_SIZE 4096
#define ALIGN_SIZE(size) (((size) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

/* Struct for memory blocks */
typedef struct Block {
    size_t size;         /* Size of the block */
    struct Block *next;  /* Pointer to the next block in the free list */
    int free;            /* Free status (1 if free, 0 if used) */
} Block;

/* Functions for memory management */
void *naive_malloc(size_t size);
void naive_free(void *ptr);

#endif /* MALLOC_H */
