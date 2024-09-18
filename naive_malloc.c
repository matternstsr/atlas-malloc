#include "malloc.h"

static mem_heap_t mem_heap = {NULL, 0, 0, 0};

/**
 * naive_allocate - Allocates memory in the heap
 * @requested_size: size of memory to allocate
 * Return: returns a pointer to the allocated memory
*/
void *naive_allocate(size_t requested_size)
{
    static int is_heap;
    size_t aligned_size = ((requested_size + 7) / 8) * 8;
    segment_header_t *block_pointer = NULL;  // Change type to segment_header_t*

    if (!is_heap)
    {
        mem_heap.first_segment = sbrk(0);
        while (mem_heap.tot_sz < (aligned_size + sizeof(segment_header_t)))
        {
            sbrk(getpagesize());
            mem_heap.tot_sz += getpagesize();
            mem_heap.free_sz += getpagesize();
        }
        mem_heap.first_segment->total_b = aligned_size;
        mem_heap.first_segment->used_bytes = 0;  // Initialize used bytes
        mem_heap.free_sz = mem_heap.tot_sz - (sizeof(segment_header_t) + aligned_size);
        mem_heap.block_count = 1;
        block_pointer = mem_heap.first_segment + 1;
        is_heap = 1;
        return ((void *)block_pointer);
    }
    while ((aligned_size + sizeof(segment_header_t)) > mem_heap.free_sz)
    {
        sbrk(getpagesize());
        mem_heap.tot_sz += getpagesize();
        mem_heap.free_sz += getpagesize();
    }
    block_pointer = (segment_header_t *)move_memory_block((sizeof(segment_header_t) + aligned_size));
    mem_heap.block_count++;
    mem_heap.free_sz -= (sizeof(segment_header_t) + aligned_size);
    return ((void *)++block_pointer);
}

/**
 * move_memory_block - Function for traversing the header blocks
 * @size: Size user requests plus block header
 * Return: pointer to big enough chunk for size
*/
segment_header_t *move_memory_block(size_t size)
{
    segment_header_t *cur_seg;
    size_t index = 0, total_b = 0;

    cur_seg = mem_heap.first_segment;
    for (; index < mem_heap.block_count; index++)
    {
        total_b = cur_seg->total_b;
        cur_seg = (segment_header_t *)((char *)cur_seg + sizeof(segment_header_t) + total_b);
    }
    cur_seg->total_b = size - sizeof(segment_header_t);
    return cur_seg;
}
