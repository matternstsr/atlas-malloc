#ifndef MALLOC_H
#define MALLOC_H
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>

/* start with 4096 page size */
#define PAGE_SIZE 4096

/* Make the "size" to the next size up of PAGE_SIZE */
#define ALIGN_SIZE(size) (((size) + sizeof(Block) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

/* request 10 bytes
aligned_size = size + sizeof(Block);  where size is 10 bytes, sizeof(Block) is, say, 8 bytes
aligned_size = 10 + 8 = 18 bytes  ...............
aligned_size = (18 + 4096 - 1) & ~(4096 - 1);
aligned_size = (4113) & ~(4095);
aligned_size = 4113 & ~4095;
aligned_size = 4113 & 0xFFFFF000;
aligned_size = 4096; */

/* Struct for memory blocks */
typedef struct Block {
    size_t size;
    struct Block *next;
} Block;

/* free_list */
extern Block *free_list;

void *naive_malloc(size_t size);
void *_malloc(size_t size);
void _free(void *ptr);

#endif /* MALLOC_H */



