#ifndef LIBFT_MALLOC_H
# define LIBFT_MALLOC_H

# include "../libft/ft_printf/ft_printf.h"
# include "../libft/libft.h"
# include <stdint.h>
# include <stdlib.h>
# include <sys/mman.h>

# ifdef __linux__
#  define get_page_size() sysconf(_SC_PAGESIZE)
# else
#  define get_page_size() getpagesize()
# endif

// Each zone must contain at least 100 allocations.
# define MIN_BLOCKS_PER_ZONE 100

// Default values n, m
# define TINY_SIZE 128
# define SMALL_SIZE 1024
# define LARGE_SIZE 1025

# define TINY 1
# define SMALL 2
# define LARGE 3

# define ALIGNMENT 16
# define IS_ALIGNED(p) (((uintptr_t)(p) % ALIGNMENT) == 0)

# define BLOCK_SIZE sizeof(t_block)
# define ZONE_SIZE sizeof(t_zone)

typedef struct	__attribute__((aligned(16))) s_block
{
	size_t size;
	int is_free;
	struct s_block *next;
}				t_block;

typedef struct	__attribute__((aligned(16))) s_zone
{
	int zone_type;
	size_t zone_size;
	size_t used_size;
	t_block *blocks;
	struct s_zone *next;
}				t_zone;

extern t_zone	*g_zone;

void			free(void *ptr);
void			*malloc(size_t size);
void			*realloc(void *ptr, size_t size);
void			show_alloc_mem(void);

#endif
