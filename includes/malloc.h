#ifndef MALLOC_H
# define MALLOC_H

# include "colors.h"
# include "libft_malloc.h"

// DEBUG
void	print_custom(char *message);
void	print_size(size_t size);
void	print_memory(size_t size);

// MAIN_FUNCTION
t_block	*alloc_block_in_existing_zone(t_zone *zone, size_t size, int zone_type);

// UTILS
int		check_size(size_t size);
void	init_block(t_zone **current_zone, size_t size, t_block **block);
int		which_zone(size_t size);
t_zone	*find_zone(int zone_type, size_t size);
t_block	*find_free_block(t_zone *zone, size_t size);
void	add_block_to_zone(t_zone *zone, t_block *new_block);
size_t	get_block_size(size_t max_alloc_size);
size_t	get_zone_size(size_t max_alloc_size);
size_t	get_size(size_t size, int zone_type);
void	*align_ptr(void *ptr);

#endif
