#include "malloc.h"

void _free(void *ptr)
{
    if (ptr == NULL)
        return;
    Block *block = (Block *)((char *)ptr - sizeof(Block));
    block->next = free_list;
    free_list = block;
}
