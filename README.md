# Malloc

## 🎯 Objective

For this project, I developed a custom dynamic memory allocator by re-implementing the standard libc functions: `malloc()`, `free()`, and `realloc()`. The goal was to understand how dynamic memory management works at a low level and to create a performant, robust, and thread-safe memory allocator from scratch.

My allocator uses `mmap()` and `munmap()` system calls to manage memory zones and avoids calling the system allocator repeatedly by pre-allocating large memory regions for small and medium allocations. I carefully defined zones for `TINY`, `SMALL`, and `LARGE` allocations to optimize speed and memory usage.

## 🧠 Skills Involved

- Low-level memory management (mmap, munmap)
- Implementing custom allocators
- Memory alignment and fragmentation handling
- Systems programming (POSIX syscalls)
- Thread safety and synchronization (pthread)
- Makefile scripting and environment variable handling
- Debugging and visualization of memory state

## 🚀 Features

### Mandatory

- Reimplementation of standard functions with the same prototypes:
  - `void *malloc(size_t size);`
  - `void free(void *ptr);`
  - `void *realloc(void *ptr, size_t size);`
- Management of memory zones:
  - **TINY** zone for small allocations (1 to _n_ bytes)
  - **SMALL** zone for medium allocations (_n+1_ to _m_ bytes)
  - **LARGE** allocations handled individually outside zones
- Use of `mmap()` and `munmap()` syscalls exclusively for requesting and releasing memory from the OS
- Pre-allocation of zones aligned to page size (`getpagesize()` or `sysconf(_SC_PAGESIZE)`)
- Efficient memory allocation limiting system calls for performance
- Custom memory visualization function `show_alloc_mem()` that displays allocated blocks by zone with addresses and sizes
- Robust error handling to avoid undefined behavior or segmentation faults
- Makefile with automatic symbolic link creation (`libft_malloc.so -> libft_malloc_$HOSTTYPE.so`)
- Usage of environment variable `HOSTTYPE` or fallback to system info for library naming

### Bonus

- Full thread safety ensured using `pthread` mutexes to manage concurrent allocations
- Extended memory visualization with `show_alloc_mem_ex()`, showing detailed allocation history and hex dumps
- Memory defragmentation techniques to reduce fragmentation and optimize reuse of freed blocks
- Support for debug environment variables mimicking or extending system malloc debug options

## 🛠️ Usage

```bash
make                # Compile the malloc library and project

# Optionally set HOSTTYPE environment variable before compilation
export HOSTTYPE=

# Run any program using the custom malloc by preloading the library
LD_PRELOAD=./libft_malloc.so ./your_program
```
