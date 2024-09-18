#include "malloc.h"

static n_heap_t heap = {NULL, 0, 0, 0};

/**
 * naive_malloc - Allocates memory in the heap
 * @size: size of memory to allocate
 * Return: returns a pointer to the allocated memory
*/

void *naive_malloc(size_t size)
{
	static int is_heap;
	size_t aligned_sz = ((size + 7) / 8) * 8;
	segment_header_t *block_pointer = NULL;

	if (!is_heap)
	{
		heap.first_segment = sbrk(0);
		while (heap.tot_sz < (aligned_sz + 8))
		{
			sbrk(getpagesize());
			heap.tot_sz += getpagesize();
			heap.free_sz += getpagesize();
		}
		heap.first_segment->total_b = aligned_sz;
		heap.free_sz = heap.tot_sz - (8 + aligned_sz);
		heap.block_count = 1;
		block_pointer = heap.first_segment + 1;
		is_heap = 1;
		return ((void *) block_pointer);
	}
	while ((aligned_sz + 8) > heap.free_sz)
	{
		sbrk(getpagesize());
		heap.tot_sz += getpagesize(), heap.free_sz += getpagesize();
	}
	block_pointer = n_move_block((8 + aligned_sz));
	heap.block_count++;
	heap.free_sz -= (8 + aligned_sz);
	return ((void *) ++block_pointer);
}


/**
 * n_move_block - Function for traversing the header blocks
 * @size: Size user requests plus block header
 * Return: pointer to big enough chunk for size
*/
segment_header_t *n_move_block(size_t size)
{
    segment_header_t *cur_seg;
    size_t index = 0, total_b = 0;

	cur_seg = heap.first_segment;
	for (; i < heap.block_count; i++)
	{
		total_b = cur_seg->total_b;
		cur_seg = (segment_header_t *)((char *)cur_seg + sizeof(segment_header_t) + total_b);
	}
	cur_seg->total_b = size - 8;
	return (cur_seg);
}