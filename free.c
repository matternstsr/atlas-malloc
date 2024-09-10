#include "malloc.h"

/* Freeing memory */
static void naive_free_internal(void *ptr, Block **free_list_ptr)
{
    if (ptr == NULL)
        return;

    Block *block = (Block *)((char *)ptr - sizeof(Block));
    block->next = *free_list_ptr;
    *free_list_ptr = block;
}

/* Free memory */
void _free(void *ptr)
{
    static Block *free_list = NULL;  /* Static var for free list */
    naive_free_internal(ptr, &free_list);
}
