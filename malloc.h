#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h>

/* Define a common page size value (e.g., 4096 bytes) */
#define PAGE_SIZE 4096

/* Struct for memory blocks */
typedef struct memory_block {
    size_t size;
    struct memory_block *next;
} memory_block;

/* Struct for heap management */
typedef struct heap_management {
    memory_block *first_block;
    size_t total_size;
    size_t free_size;
    size_t used_size;
    size_t block_count;
} heap_management;

/* Prototypes */
void *simple_malloc(size_t size);
void simple_free(void *ptr);

#endif /* MALLOC_H */
