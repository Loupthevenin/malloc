#ifndef MALLOC_H
# define MALLOC_H

# include "colors.h"
# include "libft_malloc.h"

// DEBUG
void	print_custom(char *message);
void	print_size(size_t size);
void	print_memory(size_t size);

// UTILS
void	init_block(size_t size, t_block **block);
size_t	get_zone_size(size_t max_alloc_size);

#endif
