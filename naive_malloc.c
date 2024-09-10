#include "malloc.h"

static void *heap_end = NULL;

/**
 * naive_malloc - Allocates memory in the heap
 * @size: size of memory to allocate
 * Return: returns a pointer to the allocated memory
 */
void *naive_malloc(size_t size)
{
    void *prev_heap_end;
    void *ptr;
    size_t aligned_size;

    if (size == 0)
        return NULL;
    aligned_size = ALIGN_SIZE(size + sizeof(size_t));
    if (heap_end == NULL)
    {
        heap_end = sbrk(0);
        if (heap_end == (void *)-1)
            return NULL;
        if (sbrk(PAGE_SIZE) == (void *)-1)
            return NULL;
        heap_end = sbrk(0);
    }
    prev_heap_end = heap_end;
    if (sbrk(aligned_size) == (void *)-1)
        return NULL;
    heap_end = (char *)heap_end + aligned_size;
    *(size_t *)prev_heap_end = size;
    ptr = (char *)prev_heap_end + sizeof(size_t);
    return ptr;
}
