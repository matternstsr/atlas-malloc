#ifndef MALLOC_H
#define MALLOC_H

#include <unistd.h>
#include <stddef.h>
#include <stdint.h>

/* Align size to 8 bytes */
#define ALIGN_SIZE(size) (((size) + 7) & ~7)

/* Block header size and structure */
#define BLOCK_HEADER_SIZE sizeof(Block)

typedef struct Block {
    size_t size;
    struct Block *next;
    int free;
} Block;

/* Function prototypes */
void *naive_malloc(size_t size);
void naive_free(void *ptr);

#endif /* MALLOC_H */
