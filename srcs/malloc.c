#include "../includes/malloc.h"

t_zone			*g_zone = NULL;

static void	*get_memory(size_t size)
{
	print_memory(size);
	return (mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1,
			0));
}

static t_zone	*create_zone(size_t size, int zone_type)
{
	t_zone	*zone;
	t_block	*block;
	size_t	total_size;

	total_size = get_size(size, zone_type);
	zone = get_memory(total_size);
	if (zone == MAP_FAILED)
	{
		perror("mmap zone");
		return (NULL);
	}
	zone->zone_type = zone_type;
	zone->zone_size = total_size;
	zone->blocks = NULL;
	zone->next = g_zone;
	g_zone = zone;
	// on donne de la memoire a block de telle sorte a ce qu'on soit juste apres zone;
	block = (t_block *)((char *)zone + ZONE_SIZE);
	block = (t_block *)align_ptr(block);
	init_block(&zone, size, &block);
	zone->blocks = block;
	zone->used_size = ZONE_SIZE + BLOCK_SIZE + size;
	return (zone);
}

static t_block	*alloc_block_new_zone(size_t size, int zone_type)
{
	t_zone	*zone;

	zone = create_zone(size, zone_type);
	if (!zone)
		return (NULL);
	return (zone->blocks);
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

t_block	*alloc_block_in_existing_zone(t_zone *zone, size_t size, int zone_type)
{
	t_block	*block;

	block = find_free_block(zone, size);
	if (block)
	{
		block->is_free = 0;
		zone->used_size += BLOCK_SIZE + size;
		return (block);
	}
	block = create_block_in_zone(zone, size);
	if (!block)
		return (alloc_block_new_zone(size, zone_type));
	return (block);
}

// TODO: mettre en place des prints pour chaque passages différents
// TODO: mettre en place un on off pour activer les prints de debug
void	*malloc(size_t size)
{
	t_block	*block;
	t_zone	*zone;
	int		zone_type;

	print_custom("MALLOC");
	print_size(size);
	if (!check_size(size))
		return (NULL);
	block = NULL;
	zone_type = which_zone(size);
	zone = find_zone(zone_type, size);
	if (!zone)
		block = alloc_block_new_zone(size, zone_type);
	else
		block = alloc_block_in_existing_zone(zone, size, zone_type);
	if (!block)
		return (NULL);
	return ((void *)(block + 1));
}
