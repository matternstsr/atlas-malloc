#ifndef MALLOC_H
#define MALLOC_H
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>

/* start with 4096 page size */
#define PS 4096

/* Make the "size" to the next size up of PS */
/* #define ALIGN_SIZE(size) (((size) + sizeof(Block) + PS - 1) & ~(PS - 1))
 */
#define ALIGN_SIZED(size) (((size) + 7) & ~7)  // Align to 8 bytes

/* #define ALIGN_SIZE(size) (((size) + 7) & ~7)  // Align to 8 bytes
#define BUFFER_SIZE 4096  // Total size of the buffer */
static char buffer[BUFFER_SIZE]; // Static buffer for allocation
/* static size_t allocated_size = 0; // Track allocated size */

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

/* Head of the free list */
extern Block *free_list;

/* Function prototypes */
void *naive_malloc(size_t size);
void *_malloc(size_t size);
void _free(void *ptr);

#endif /* MALLOC_H */
