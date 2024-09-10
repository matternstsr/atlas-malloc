#ifndef MALLOC_H
#define MALLOC_H
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>

/* start with 4096 page size */
#define PAGE_SIZE 4096
/* #define PAGE_SIZE 8 */
/* Heap might be extended too freq & could make issue with heap_end not rem const. */

/* Make the "size" to the next size up of PAGE_SIZE */
/* #define ALIGN_SIZE(size) (((size) + sizeof(Block) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1)) */

/* Make the "size" to the next size up of PAGE_SIZE */
#define ALIGN_SIZE(size) (((size) + 8 - 1) & ~(8 - 1))


/* Struct for memory blocks */
typedef struct Block {
    size_t size;
    struct Block *next;
} Block;

/* free_list */
/* extern Block *free_list;
 */
void *naive_malloc(size_t size);
void *_malloc(size_t size);
void _free(void *ptr);

#endif /* MALLOC_H */



