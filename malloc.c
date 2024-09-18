#include "malloc.h"

/* Free list */
Block *free_list = NULL;

void *_malloc(size_t size)
{
	Block *block, *prev = NULL, *best_fit = NULL;
	size_t aligned_size = ALIGN_SIZE(size);

	if (size == 0)
		return (NULL);

	/* go through the free list until find a good block */
	for (block = free_list; block != NULL; block = block->next)
	{
		if (block->size >= aligned_size && (best_fit == NULL || block->size < best_fit->size))
			best_fit = block;
	}

	if (best_fit != NULL) {
		/* Use the block that fits */
		if (best_fit->size > aligned_size + sizeof(Block))
		{
			Block *remaining_block = (Block *)((char *)best_fit + aligned_size + sizeof(Block));
			remaining_block->size = best_fit->size - aligned_size - sizeof(Block);
			remaining_block->next = best_fit->next;
			best_fit->size = aligned_size;
			best_fit->next = remaining_block;
		}
		else
		{
			/* Take the block out of the free list */
			if (prev == NULL)
				free_list = best_fit->next;
			else
				prev->next = best_fit->next;
		}
		return ((char *)best_fit + sizeof(Block));
	}

	/* No good block was found, Make the block larger */
	block = sbrk(aligned_size + sizeof(Block));
	if (block == (void *)-1)
		return (NULL);  /* sbrk didnt work */
	block->size = aligned_size;
	return ((char *)block + sizeof(Block));
}
