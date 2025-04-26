#include "../includes/malloc.h"

t_zone			*g_zone = NULL;

static void	*get_memory(size_t size)
{
	print_memory(size);
	return (mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1,
			0));
}

// TODO manage error failed
static void	create_zone(t_zone **tmp, size_t size, int zone_type)
{
	size_t	total_size;
	t_block	*block;

	if (zone_type == TINY)
		total_size = get_zone_size(TINY_SIZE);
	else if (zone_type == SMALL)
		total_size = get_zone_size(SMALL_SIZE);
	else if (zone_type == LARGE)
		total_size = size + BLOCK_SIZE;
	*tmp = get_memory(total_size);
	if (*tmp == MAP_FAILED)
	{
		perror("mmap zone");
		return ;
	}
	(*tmp)->zone_type = zone_type;
	(*tmp)->blocks = NULL;
	(*tmp)->next = g_zone;
	g_zone = *tmp;
	// on donne de la memoire a block de telle sorte a ce qu'on soit juste apres zone;
	block = (t_block *)((char *)*tmp + ZONE_SIZE);
	init_block(size, &block);
	(*tmp)->blocks = block;
}

// TODO verif si encore assez de memoire;
static t_block	*create_block_in_zone(t_zone *zone, size_t size)
{
	t_block	*current;
	t_block	*result;

	current = zone->blocks;
	while (current->next)
		current = current->next;
	result = (t_block *)((char *)current + BLOCK_SIZE + current->size);
	init_block(size, &result);
	current->next = result;
	return (result);
}

void	*malloc(size_t size)
{
	t_block	*block;
	t_zone	*zone;
	int		zone_type;

	print_custom("MALLOC");
	print_size(size);
	block = NULL;
	zone_type = which_zone(size);
	zone = find_zone(zone_type);
	if (!zone)
	{
		create_zone(&zone, size, zone_type);
		if (!zone)
			return (NULL);
		block = zone->blocks;
	}
	else
	{
		block = find_free_block(zone, size);
		if (!block)
		{
			block = create_block_in_zone(zone, size);
			if (!block)
				return (NULL);
		}
		else
			block->is_free = 0;
	}
	return ((void *)(block + 1));
}
