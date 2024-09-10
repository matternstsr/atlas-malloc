#ifndef MALLOC_H
#define MALLOC_H

#include <unistd.h>
#include <stddef.h>

/* Define page size */
#define PAGE_SIZE 4096

/* Align size to be a multiple of PAGE_SIZE, plus the header size */
#define ALIGN_SIZE(size) (((size) + sizeof(size_t) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

/* Struct for memory blocks */
typedef struct Block {
    size_t size;
    struct Block *next;
} Block;

/* Free list head */
extern Block *free_list;

void *naive_malloc(size_t size);
void naive_free(void *ptr);

#endif /* MALLOC_H */
