#include "../includes/malloc.h"

t_zone			*g_zone = NULL;

// TODO probleme d'alignement
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

	total_size = get_size(size, zone_type, 0);
	*tmp = get_memory(total_size);
	if (*tmp == MAP_FAILED)
	{
		perror("mmap zone");
		return ;
	}
	(*tmp)->zone_type = zone_type;
	(*tmp)->zone_size = total_size;
	(*tmp)->blocks = NULL;
	(*tmp)->next = g_zone;
	g_zone = *tmp;
	// on donne de la memoire a block de telle sorte a ce qu'on soit juste apres zone;
	block = (t_block *)((char *)*tmp + ZONE_SIZE);
	init_block(size, &block);
	(*tmp)->blocks = block;
	(*tmp)->used_size = ZONE_SIZE + BLOCK_SIZE + size;
}

static t_block	*create_block_in_zone(t_zone *zone, size_t size)
{
	t_block	*result;

	if (zone->used_size + BLOCK_SIZE + size > zone->zone_size)
		return (NULL);
	result = (t_block *)((char *)zone + zone->used_size);
	init_block(size, &result);
	add_block_to_zone(zone, result);
	zone->used_size += BLOCK_SIZE + size;
	return (result);
}

// TODO manage error failed
static t_block	*allocate_block_zone(t_zone *zone, size_t size, int zone_type)
{
	t_block	*result;
	void	*extra_mem;
	size_t	total_size;

	total_size = get_size(size, zone_type, 1);
	extra_mem = get_memory(total_size);
	if (extra_mem == MAP_FAILED)
	{
		perror("mmap");
		return (NULL);
	}
	result = (t_block *)extra_mem;
	init_block(size, &result);
	add_block_to_zone(zone, result);
	zone->zone_size += total_size;
	zone->used_size += BLOCK_SIZE + size;
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
			{
				block = allocate_block_zone(zone, size, zone_type);
			}
		}
		else
		{
			block->is_free = 0;
			zone->used_size += BLOCK_SIZE + size;
		}
	}
	ft_putstr_fd("used_size: ", 1);
	ft_putnbr_fd(zone->used_size, 1);
	ft_putstr_fd("\n", 1);
	return ((void *)(block + 1));
}
