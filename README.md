# C - malloc
**Project to be done in teams of 2 people**  
**Manual QA review must be done (request it when you are done with the project)**

## Concepts
For this project, you are expected to understand the following concept:

- [`Writing your own dynamic allocation library`](https://intranet.atlasschool.com/concepts/922))

## Resources
Read or watch:

- [Hack the Virtual Memory: malloc, the heap & the program break](https://intranet.atlasschool.com/rltoken/W5Socd-wukfLKNcPi-npNQ)
- [Everything you need to know to write your own malloc](https://intranet.atlasschool.com/rltoken/tiuStPIDj16LrNHL-nXj9A)

man or help:

- [`sbrk`](https://man.freebsd.org/cgi/man.cgi?sbrk(2))
- [`brk`](https://man.freebsd.org/cgi/man.cgi?brk(2))
- [`malloc`](https://linux.die.net/man/3/malloc)

## Learning Objectives
By the end of this project, you should be able to explain:

- What a program break is
- How to manipulate the program break to allocate memory dynamically
- How the glibc malloc and free functions work
- What ASLR (Address Space Layout Randomization) is
- What memory alignment is
- What a memory page is
- How to encapsulate memory management to hide it from the user

## Requirements

### General
- Allowed editors: vi, vim, emacs
- All files will be compiled on Ubuntu 20.04 LTS
- Your C programs and functions will be compiled with `gcc 9.4.0` using the flags `-Wall -Werror -Wextra` and `-pedantic`
- All files should end with a new line
- A `README.md` file at the root of the project folder is mandatory
- Code should follow the Betty style, checked using [betty-style.pl](https://github.com/hs-hq/Betty/blob/master/betty-style.pl) and [betty-doc.pl](https://github.com/hs-hq/Betty/blob/master/betty-doc.pl)
- No more than 5 functions per file
- Prototypes of all functions should be included in your header file called `malloc.h`
- Header files should be include-guarded
- You are allowed to use global variables
- You are allowed to use static variables

### Betty Compliance
- All C source files in your directory and subdirectories must be [Betty-compliant](https://intranet.atlasschool.com/rltoken/abbd1E1RWALmHC6flq9S5w)

### Allowed Functions and System Calls
- Unless specified otherwise, you are allowed to use the C standard library
- You are **not** allowed to use the `malloc` family from the C library

## Tests
It is strongly advised to test your functions with real programs, such as a shell or old projects. To do so:

1. Name your functions `malloc`, `free`, `realloc`, and `calloc` (just like they are in glibc).
2. Compile them into a shared library.
3. Load the library when executing a program using `LD_LIBRARY_PATH` and `LD_PRELOAD`.

Here’s a [tutorial on how to do it](https://intranet.atlasschool.com/rltoken/h2vNP-9d4fVvOB8wh8dhbw).

## More Info

### Important
- Your `_malloc`, `free`, `calloc`, and `realloc` functions do not need to behave exactly like the glibc versions.
- You are free to use any data structure and handle the heap as needed, as long as:
  - Returned pointers are aligned as required.
  - Enough space is available.
  - The program break is extended and reduced appropriately, by multiples of the virtual memory page size.
- Focus on the strength and reliability of your functions, especially for large allocations.

## Tasks

### 0. Naive malloc
**Mandatory**

- Build a naive malloc as described in the concept page.
- **Prototype**: `void *naive_malloc(size_t size);`
- The function must return a pointer to the allocated memory, suitably aligned for any variable.
- It should:
  - Allocate memory to store:
    - A `size_t` as the chunk header
    - The requested size
  - Allocate memory pages only

**Repo**:  
- GitHub repository: [atlas-malloc](https://github.com/matternstsr/atlas-malloc)  
- File: `naive_malloc.c`, `malloc.h`

### 1. malloc
**Mandatory**

- Implement your own malloc function that allocates space in the heap.
- **Prototype**: `void *_malloc(size_t size);`
- The function must return a pointer to the allocated memory, suitably aligned for any variable.

**Repo**:  
- GitHub repository: [atlas-malloc](https://github.com/matternstsr/atlas-malloc)  
- File: `malloc.c`, `malloc.h`

### 2. free
**Mandatory**

- Implement your own free function that frees a memory space.
- **Prototype**: `void _free(void *ptr);`
- The function must handle freeing the memory space pointed to by `ptr`.

**Repo**:  
- GitHub repository: [atlas-malloc](https://github.com/matternstsr/atlas-malloc)  
- File: `free.c`, `malloc.c`, `malloc.h`
