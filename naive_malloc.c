#include <stddef.h> // for size_t
#include <sys/mman.h> // for mmap, munmap, PROT_READ, PROT_WRITE, MAP_PRIVATE, MAP_ANONYMOUS, MAP_FAILED
#include <unistd.h> // for sysconf
#include <errno.h> // for errno, EINVAL
#include <stdio.h> // for perror

struct page_header {
    size_t pages;
    size_t page_size;
    char page_data[]; // the rest of the page(s) of memory
};

static unsigned long get_page_size(void) {
    static unsigned long page_size = 0;
    if (page_size == 0) {
        long tmp = sysconf(_SC_PAGESIZE);
        page_size = tmp < 0 ? 4096 : (unsigned long)tmp;
    }
    return page_size;
}

static struct page_header* void_to_page_header(void* ptr) {
    return (struct page_header*)((char*)ptr - sizeof(struct page_header));
}

static void naive_free_internal(struct page_header* header) {
    if (header == NULL) {
        return;
    }
    const size_t size = header->pages * header->page_size;
    int res = munmap(header, size);
    if (res == -1) {
        perror("Error freeing!");
    }
}

void naive_free(void* ptr) {
    if (ptr != NULL) {
        naive_free_internal(void_to_page_header(ptr));
    }
}

static struct page_header* naive_malloc_internal(size_t size) {
    if (size == 0) {
        return NULL;
    }
    size_t page_size = get_page_size();
    size_t size_with_header = size + sizeof(struct page_header);
    size_t pages = size_with_header / page_size + 1;
    size_t mmap_size = pages * page_size;
    void* ptr = mmap(0, mmap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        return NULL;
    }
    struct page_header* page = ptr;
    page->pages = pages;
    page->page_size = page_size;
    return page;
}

void* naive_malloc(size_t size) {
    struct page_header* result = naive_malloc_internal(size);
    return result == NULL ? NULL : result->page_data;
}

static void zero_ptr(char* start, size_t size) {
    for (char* ptr = start; ptr < start + size; ptr++) {
        *ptr = 0;
    }
}

void* naive_calloc(size_t nmemb, size_t size) {
    size_t bytes = nmemb * size;
    if (nmemb == 0 || size == 0) {
        return NULL;
    }
    if (bytes / nmemb != size) { // overflow check
        errno = EINVAL;
        return NULL;
    }
    struct page_header* header = naive_malloc_internal(bytes);
    if (header != NULL) {
        zero_ptr((char*)header, bytes + sizeof(struct page_header));
    }
    return header == NULL ? NULL : header->page_data;
}

void* naive_realloc(void* ptr, size_t size) {
    if (ptr == NULL) {
        return naive_malloc(size);
    }
    struct page_header* page = void_to_page_header(ptr);
    size_t start_size = page->pages * page->page_size;
    if (size < start_size) {
        return ptr;
    }
    struct page_header* new_page = naive_malloc_internal(size);
    if (new_page != NULL) {
        for (size_t i = 0; i < start_size; i++) {
            new_page->page_data[i] = page->page_data[i];
        }
        naive_free(ptr);
    }
    return new_page == NULL ? NULL : new_page->page_data;
}
