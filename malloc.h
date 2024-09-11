#ifndef MALLOC_NEW_H
#define MALLOC_NEW_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h> // For size_t

// Initialize the allocator
void init_allocator();

// Allocate memory of a given size
void *naive_malloc(size_t size);

// Page size
#define PAGE_SIZE 4096

// Metadata size (size_t header)
#define METADATA_SIZE sizeof(size_t)

// Define a struct to manage allocated blocks of the same size
typedef struct Block {
    struct Block *next;
    void *memory;
} Block;

// Define a struct for each size class
typedef struct SizeClass {
    Block *free_list;
    size_t size;
} SizeClass;

// Maximum number of size classes we will handle
#define MAX_SIZE_CLASSES 128

#endif // MALLOC_NEW_H
