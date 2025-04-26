#include "../includes/malloc.h"

t_zone		*g_zone = NULL;

// TODO Need one time get_memory not multiple calling
static void	*get_memory(size_t size)
{
	print_memory(size);
	return (mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1,
			0));
}

// TODO manage error failed
static void	create_zone(t_zone **tmp, int zone_type)
{
	*tmp = get_memory(ZONE_SIZE);
	if (*tmp == MAP_FAILED)
	{
		perror("mmap zone");
		return ;
	}
	(*tmp)->zone_type = zone_type;
	(*tmp)->blocks = NULL;
	(*tmp)->next = g_zone;
	g_zone = *tmp;
}

static void	add_block_to_zone(t_block *new, int zone_type)
{
	t_zone	*tmp;
	t_block	*current;

	tmp = g_zone;
	while (tmp)
	{
		if (tmp->zone_type == zone_type)
			break ;
		tmp = tmp->next;
	}
	if (!tmp)
		create_zone(&tmp, zone_type);
	if (!tmp->blocks)
		tmp->blocks = new;
	else
	{
		current = tmp->blocks;
		while (current->next)
			current = current->next;
		current->next = new;
	}
}

int	handle_region(t_block **block, size_t size, int flags)
{
	size_t	total_size;

	if (flags == TINY)
		total_size = get_zone_size(TINY_SIZE);
	else if (flags == SMALL)
		total_size = get_zone_size(SMALL_SIZE);
	else if (flags == LARGE)
		total_size = size + BLOCK_SIZE;
	else
		return (-1);
	*block = get_memory(total_size);
	if (*block == MAP_FAILED)
	{
		perror("mmap");
		return (-1);
	}
	init_block(size, block);
	add_block_to_zone(*block, flags);
	return (0);
}

void	*malloc(size_t size)
{
	t_block	*block;

	print_custom("MALLOC");
	print_size(size);
	block = NULL;
	if (size <= TINY_SIZE)
	{
		handle_region(&block, size, TINY);
	}
	else if (size <= SMALL_SIZE)
	{
		handle_region(&block, size, SMALL);
	}
	else
	{
		handle_region(&block, size, LARGE);
	}
	return ((void *)(block + 1));
}
