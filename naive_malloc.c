#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>
#include "malloc.h"

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

// Array of size classes
static SizeClass size_classes[MAX_SIZE_CLASSES];

// Initialize the allocator
void init_allocator() {
    memset(size_classes, 0, sizeof(size_classes));
}

// Get or create a size class
static SizeClass *get_size_class(size_t size) {
    for (int i = 0; i < MAX_SIZE_CLASSES; i++) {
        if (size_classes[i].size == size) {
            return &size_classes[i];
        } else if (size_classes[i].size == 0) {
            size_classes[i].size = size;
            size_classes[i].free_list = NULL;
            return &size_classes[i];
        }
    }
    return NULL; // No available slot for this size class
}

void *naive_malloc(size_t size) {
    // Initialize allocator only once
    static int initialized = 0;
    if (!initialized) {
        init_allocator();
        initialized = 1;
    }

    size_t total_size = size + METADATA_SIZE;
    size_t aligned_size = ((total_size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;

    SizeClass *size_class = get_size_class(size);
    if (size_class == NULL) {
        return NULL; // No available size class slot
    }

    if (size_class->free_list != NULL) {
        Block *block = size_class->free_list;
        size_class->free_list = block->next;
        return block->memory;
    }

    void *page = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (page == MAP_FAILED) {
        return NULL; // Allocation failed
    }

    Block *new_block = (Block *)page;
    new_block->memory = (char *)page + METADATA_SIZE;
    new_block->next = NULL;
    size_class->free_list = new_block;

    return new_block->memory;
}
