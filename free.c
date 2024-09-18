#include "malloc.h"

/**
* _free - Frees the allocated memory pointed to by ptr.
*
* This function adds the block back to the free list.
*
* @ptr: Pointer to the memory block to be freed.
*/
void _free(void *ptr)
{
	free_list;
	Block *block;

	if (ptr == NULL)
		return;

	block = (Block *)((char *)ptr - sizeof(Block));
	block->next = free_list;
	free_list = block;
}
