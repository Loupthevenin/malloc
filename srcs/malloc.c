#include "../includes/malloc.h"

t_zone			*g_zone = NULL;

static void	*get_memory(size_t size)
{
	print_memory(size);
	return (mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1,
			0));
}

// TODO: manage error failed
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
	block = (t_block *)align_ptr(block);
	init_block(tmp, size, &block);
	(*tmp)->blocks = block;
	(*tmp)->used_size = ZONE_SIZE + BLOCK_SIZE + size;
}

static t_block	*create_block_in_zone(t_zone *zone, size_t size)
{
	t_block	*result;

	if (zone->used_size + BLOCK_SIZE + size > zone->zone_size)
		return (NULL);
	result = (t_block *)((char *)zone + zone->used_size);
	result = (t_block *)align_ptr(result);
	init_block(&zone, size, &result);
	add_block_to_zone(zone, result);
	zone->used_size += BLOCK_SIZE + size;
	return (result);
}

// TODO: check size error like 0, overflow, negative;
void	*malloc(size_t size)
{
	t_block	*block;
	t_zone	*zone;
	t_zone	*new_zone;
	int		zone_type;

	print_custom("MALLOC");
	print_size(size);
	block = NULL;
	zone_type = which_zone(size);
	zone = find_zone(zone_type, size);
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
				create_zone(&new_zone, size, zone_type);
				if (!new_zone)
					return (NULL);
				block = new_zone->blocks;
			}
		}
		else
		{
			block->is_free = 0;
			zone->used_size += BLOCK_SIZE + size;
		}
	}
	return ((void *)(block + 1));
}
