#include "../includes/malloc.h"

int	check_size(size_t size)
{
	if (size == 0 || size > MAX_ALLOC)
		return (0);
	return (1);
}

void	init_block(t_zone **current_zone, size_t size, t_block **block)
{
	(*block)->size = size;
	(*block)->is_free = 0;
	(*block)->next = NULL;
	(*block)->zone = *current_zone;
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

t_zone	*find_zone(int zone_type, size_t size)
{
	t_zone	*tmp;

	tmp = g_zone;
	while (tmp)
	{
		if (tmp->zone_type == zone_type && tmp->zone_size >= tmp->used_size
			+ size + BLOCK_SIZE)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
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

size_t	get_size(size_t size, int zone_type)
{
	size_t	result;

	if (zone_type == TINY)
		result = get_zone_size(TINY_SIZE);
	else if (zone_type == SMALL)
		result = get_zone_size(SMALL_SIZE);
	else if (zone_type == LARGE)
		result = size + BLOCK_SIZE + ZONE_SIZE;
	return (result);
}

void	*align_ptr(void *ptr)
{
	uintptr_t	p;
	uintptr_t	result;

	p = (uintptr_t)ptr;
	result = (p + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
	return ((void *)result);
}

int	is_zone_empty(t_zone *zone)
{
	t_block	*block;

	block = zone->blocks;
	while (block)
	{
		if (!block->is_free)
			return (0);
		block = block->next;
	}
	return (1);
}

void	remove_zone(t_zone *zone_to_remove)
{
	t_zone	*current;
	t_zone	*prev;

	current = g_zone;
	prev = NULL;
	while (current)
	{
		if (current == zone_to_remove)
		{
			if (prev)
				prev->next = current->next;
			else
				g_zone = current->next;
			if (munmap((void *)current, current->zone_size) == -1)
				perror("munmap");
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	print_custom(char *message)
{
	ft_putstr_fd(GREEN "[DEBUG] ", 1);
	ft_putendl_fd(message, 1);
	ft_putstr_fd(RESET, 1);
}

void	print_size(char *message, size_t size)
{
	ft_putstr_fd(YELLOW "[DEBUG] ", 1);
	ft_putstr_fd(message, 1);
	ft_putstr_fd("Alloc size: ", 1);
	ft_putsize_fd(size, 1);
	ft_putstr_fd("\n" RESET, 1);
}

void	print_memory(size_t size)
{
	ft_putstr_fd(BLUE "[MMAP] Calling mmap with size: ", 1);
	ft_putsize_fd(size, 1);
	ft_putstr_fd("\n" RESET, 1);
}

void	print_trace(char *message)
{
	ft_putstr_fd(PURPLE "[TRACE] ", 1);
	ft_putendl_fd(message, 1);
	ft_putstr_fd(RESET, 1);
}

void	log_trace_if(t_debug_config *config, char *message)
{
	if (config->trace)
		print_trace(message);
}
