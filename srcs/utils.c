#include "../includes/malloc.h"

void	init_block(size_t size, t_block **block)
{
	(*block)->size = size;
	(*block)->is_free = 0;
	(*block)->next = NULL;
}

int	which_zone(size_t size)
{
	if (size <= TINY_SIZE)
		return (TINY);
	else if (size <= SMALL_SIZE)
		return (SMALL);
	else
		return (LARGE);
}

t_zone	*find_zone(int zone_type)
{
	t_zone	*tmp;

	tmp = g_zone;
	while (tmp)
	{
		if (tmp->zone_type == zone_type)
			break ;
		tmp = tmp->next;
	}
	return (tmp);
}

t_block	*find_free_block(t_zone *zone, size_t size)
{
	t_block	*current;

	current = zone->blocks;
	while (current)
	{
		if (current->is_free && current->size >= size)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	add_block_to_zone(t_zone *zone, t_block *new_block)
{
	t_block	*current;

	current = zone->blocks;
	if (!current)
		zone->blocks = new_block;
	else
	{
		while (current->next)
			current = current->next;
		current->next = new_block;
	}
}

size_t	get_zone_size(size_t max_alloc_size)
{
	size_t	page_size;
	size_t	total_max_alloc;
	size_t	total_zone;

	page_size = get_page_size();
	total_max_alloc = (max_alloc_size + BLOCK_SIZE) * MIN_BLOCKS_PER_ZONE;
	total_zone = total_max_alloc + ZONE_SIZE;
	return (((total_zone + page_size - 1) / page_size) * page_size);
}

size_t	get_block_size(size_t max_alloc_size)
{
	size_t	page_size;
	size_t	total_max_alloc;
	size_t	total_block;

	page_size = get_page_size();
	total_max_alloc = max_alloc_size + BLOCK_SIZE;
	total_block = total_max_alloc * MIN_BLOCKS_PER_ZONE;
	return (((total_block + page_size - 1) / page_size) * page_size);
}

size_t	get_size(size_t size, int zone_type, int is_block)
{
	size_t	result;

	if (zone_type == TINY)
		if (!is_block)
			result = get_zone_size(TINY_SIZE);
		else
			result = get_block_size(TINY_SIZE);
	else if (zone_type == SMALL)
		if (!is_block)
			result = get_zone_size(SMALL_SIZE);
		else
			result = get_block_size(SMALL_SIZE);
	else if (zone_type == LARGE)
		result = size + BLOCK_SIZE;
	return (result);
}

void	print_custom(char *message)
{
	ft_putstr_fd(GREEN "CUSTOM ", 1);
	ft_putendl_fd(message, 1);
	ft_putstr_fd(RESET, 1);
}

void	print_size(size_t size)
{
	ft_putstr_fd(YELLOW "Alloc size: ", 1);
	ft_putnbr_fd(size, 1);
	ft_putstr_fd("\n" RESET, 1);
}

void	print_memory(size_t size)
{
	ft_putstr_fd(BLUE "Calling mmap with size: ", 1);
	ft_putnbr_fd(size, 1);
	ft_putstr_fd("\n" RESET, 1);
}
