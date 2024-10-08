#include "malloc.h"

/**
* naive_malloc - Allocates memory in the heap
* @size: size of memory to allocate
* Return: returns a pointer to the allocated memory
*/
void *naive_malloc(size_t size)
{
	static void *heap_end;
	void *prev_heap_end;
	void *ptr;
	size_t aligned_size;

	/* heap_end = NULL; */
	if (size == 0)
		return (NULL);

	/* Align size to the next page boundary, with the size of the block hdr */
	aligned_size = ALIGN_SIZE(size + sizeof(size_t));

	if (heap_end == NULL)
		heap_end = sbrk(0);  /* Make the heap_end to what break is right now */

	prev_heap_end = heap_end;
	if (sbrk(aligned_size) == (void *)-1)
		return (NULL);  /* sbrk didnt work */
	heap_end = (char *)heap_end + aligned_size;  /* Updating the heap_end */

	/* Store what size is at the beginning of the block */
	*(size_t *)prev_heap_end = aligned_size;

	ptr = (char *)prev_heap_end + sizeof(size_t);
	/* Store what size is at the beginning of the block */
	/* *(size_t *)prev_heap_end = aligned_size; */
	/*  moved  */
	return (ptr);
}
