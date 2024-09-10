#ifndef MALLOC_H
#define MALLOC_H

#include <unistd.h>
#include <stddef.h>

/* Define page size */
#define PAGE_SIZE 4096

/* Align size to be a multiple of PAGE_SIZE, plus the header size */
#define ALIGN_SIZE(size) (((size) + sizeof(size_t) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

/* Function prototypes */
void *naive_malloc(size_t size);

#endif /* MALLOC_H */
