#include "malloc.h"

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

    void *page = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (page == MAP_FAILED) {
        return NULL; // Allocation failed
    }

    Block *new_block = (Block *)page;
    new_block->memory = (char *)page + METADATA_SIZE;
    new_block->next = NULL;
    size_class->free_list = new_block;

    return new_block->memory;
}
