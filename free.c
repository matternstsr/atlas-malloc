#include "malloc.h"

void _free(void *ptr)
{
    if (ptr == NULL)
        return;
    Block *block = (Block *)((char *)ptr - sizeof(Block));
    Block *current = free_list;
    Block *prev = NULL;
    while (current != NULL && current < block)
    {
        prev = current
        current = current->next;
    }
    block->next = current;
    if (prev == NULL)
        free_list = block;
    else
        prev->next = block;
    if (current != NULL && (char *)block + block->size + sizeof(Block) == (char *)current)
    {
        block->size += current->size + sizeof(Block);
        block->next = current->next;
    }
    else
        block->next = current;
    if (prev != NULL && (char *)prev + prev->size + sizeof(Block) == (char *)block)
    {
        prev->size += block->size + sizeof(Block);
        prev->next = block->next;
    }
}
