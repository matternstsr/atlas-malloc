#include "malloc.h"

static n_heap_t heap = {NULL, 0, 0, 0};

/**
 * naive_malloc - Allocates memory in the heap
 * @size: size of memory to allocate
 * Return: returns a pointer to the allocated memory
*/

void *naive_malloc(size_t size)
{
	static int flag;
	size_t aligned_sz = ((size + 7) / 8) * 8;
	n_header_t *ptr = NULL;

	if (!flag)
	{
		heap.first_block = sbrk(0);
		while (heap.heap_size < (aligned_sz + 8))
		{
			sbrk(getpagesize());
			heap.heap_size += getpagesize(), heap.heap_free += getpagesize();
		}
		heap.first_block->total_bytes = aligned_sz;
		heap.heap_free = heap.heap_size - (8 + aligned_sz);
		heap.total_blocks = 1;
		ptr = heap.first_block + 1;
		flag = 1;
		return ((void *) ptr);
	}
	while ((aligned_sz + 8) > heap.heap_free)
	{
		sbrk(getpagesize());
		heap.heap_size += getpagesize(), heap.heap_free += getpagesize();
	}
	ptr = n_move_block((8 + aligned_sz));
	heap.total_blocks++;
	heap.heap_free -= (8 + aligned_sz);
	return ((void *) ++ptr);
}


/**
 * n_move_block - Function for traversing the header blocks
 * @size: Size user requests plus block header
 * Return: pointer to big enough chunk for size
*/
n_header_t *n_move_block(size_t size)
{
	size_t i = 0, total = 0;
	n_header_t *current;

	current = heap.first_block;
	for (; i < heap.total_blocks; i++)
	{
		total = current->total_bytes;
		current = (n_header_t *)((char *)current + sizeof(n_header_t) + total);
	}
	current->total_bytes = size - 8;
	return (current);
}