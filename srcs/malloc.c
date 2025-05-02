#include "../includes/malloc.h"

t_zone			*g_zone = NULL;

static void	*get_memory(size_t size)
{
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

static t_block	*alloc_block_new_zone(size_t size, int zone_type,
		t_debug_config *config)
{
	t_zone	*zone;

	if (config->trace)
		print_trace("[MALLOC] Creating new zone");
	zone = create_zone(size, zone_type);
	if (!zone)
	{
		if (config->trace)
			print_trace("[MALLOC] Failed to create new zone");
		return (NULL);
	}
	if (config->trace)
		print_trace("[MALLOC] Zone created and first block initialized");
	return (zone->blocks);
}

static t_block	*create_block_in_zone(t_zone *zone, size_t size,
		t_debug_config *config)
{
	t_block	*result;

	if (zone->used_size + BLOCK_SIZE + size > zone->zone_size)
	{
		if (config->trace)
			print_trace("[MALLOC] Not enough space in zone to create new block");
		return (NULL);
	}
	result = (t_block *)((char *)zone + zone->used_size);
	result = (t_block *)align_ptr(result);
	init_block(&zone, size, &result);
	add_block_to_zone(zone, result);
	zone->used_size += BLOCK_SIZE + size;
	if (config->trace)
		print_trace("[MALLOC] New block created in existing zone");
	return (result);
}

t_block	*alloc_block_in_existing_zone(t_zone *zone, size_t size, int zone_type,
		t_debug_config *config)
{
	t_block	*block;

	block = find_free_block(zone, size);
	if (block)
	{
		block->is_free = 0;
		zone->used_size += BLOCK_SIZE + size;
		if (config->trace)
			print_trace("[MALLOC] Reusing free block from existing zone");
		return (block);
	}
	if (config->trace)
		print_trace("[MALLOC] No suitable free block,"
					" trying to create new one");
	block = create_block_in_zone(zone, size, config);
	if (!block)
	{
		if (config->trace)
			print_trace("[MALLOC] Failed to create block in existing zone,"
						"fallback to new zone");
		return (alloc_block_new_zone(size, zone_type, config));
	}
	return (block);
}

void	*malloc(size_t size)
{
	t_block			*block;
	t_zone			*zone;
	int				zone_type;
	t_debug_config	*config;

	config = init_debug_env();
	if (config->verbose)
	{
		print_custom("[MALLOC] MALLOC");
		print_size("[MALLOC] ", size);
	}
	if (!check_size(size))
		return (NULL);
	block = NULL;
	zone_type = which_zone(size);
	zone = find_zone(zone_type, size);
	if (!zone)
		block = alloc_block_new_zone(size, zone_type, config);
	else
		block = alloc_block_in_existing_zone(zone, size, zone_type, config);
	if (!block)
		return (NULL);
	return ((void *)(block + 1));
}
