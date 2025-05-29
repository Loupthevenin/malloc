#include "../includes/malloc.h"

t_zone			*g_zone = NULL;
pthread_mutex_t	g_lock = PTHREAD_MUTEX_INITIALIZER;

static void	*get_memory(size_t size)
{
	return (mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1,
			0));
}

static void	insert_zone_sorted(t_zone *new_zone)
{
	t_zone	*current;

	if (!g_zone || (uintptr_t)new_zone < (uintptr_t)g_zone)
	{
		new_zone->next = g_zone;
		g_zone = new_zone;
		return ;
	}
	current = g_zone;
	while (current->next && (uintptr_t)current->next < (uintptr_t)new_zone)
		current = current->next;
	new_zone->next = current->next;
	current->next = new_zone;
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
	insert_zone_sorted(zone);
	// on donne de la memoire a block de telle sorte a ce qu'on soit juste apres zone;
	block = (t_block *)((char *)zone + ZONE_SIZE);
	block = (t_block *)align_ptr(block);
	init_block(zone, size, block);
	zone->blocks = block;
	zone->used_size = ZONE_SIZE + BLOCK_SIZE + size;
	return (zone);
}

static t_block	*alloc_block_new_zone(size_t size, int zone_type,
		t_debug_config *config)
{
	t_zone	*zone;

	log_trace_if(config, "[MALLOC] Creating new zone");
	zone = create_zone(size, zone_type);
	if (!zone)
	{
		log_trace_if(config, "[MALLOC] Failed to create new zone");
		return (NULL);
	}
	log_trace_if(config, "[MALLOC] Zone created and first block initialized");
	return (zone->blocks);
}

static t_block	*create_block_in_zone(t_zone *zone, size_t size,
		t_debug_config *config)
{
	t_block	*result;
	t_block	*last;

	if (zone->used_size + BLOCK_SIZE + size > zone->zone_size)
	{
		log_trace_if(config,
						"[MALLOC] Not enough space in zone to create new block");
		return (NULL);
	}
	if (!zone->blocks)
		result = (t_block *)((char *)zone + ZONE_SIZE);
	else
	{
		last = zone->blocks;
		while (last->next)
			last = last->next;
		result = (t_block *)((char *)(last + 1) + last->size);
	}
	result = (t_block *)align_ptr(result);
	init_block(zone, size, result);
	add_block_to_zone(zone, result);
	zone->used_size += BLOCK_SIZE + size;
	log_trace_if(config, "[MALLOC] New block created in existing zone");
	return (result);
}

t_block	*alloc_block_in_existing_zone(t_zone *zone, size_t size, int zone_type,
		t_debug_config *config)
{
	t_block	*block;
	t_block	*new_block;

	block = find_free_block(zone, size);
	if (block)
	{
		if (block->size >= size + BLOCK_SIZE + 64)
		{
			log_trace_if(config,
							"[MALLOC] Splitting and reusing free block");
			new_block = (void *)((char *)(block + 1) + size);
			new_block->size = block->size - size - BLOCK_SIZE;
			new_block->is_free = 1;
			new_block->zone = block->zone;
			new_block->next = block->next;
			new_block->prev = block;
			if (block->next)
				block->next->prev = new_block;
			block->next = new_block;
			block->size = size;
		}
		else
			log_trace_if(config,
							"[MALLOC] Reusing free block without split");
		block->is_free = 0;
		zone->used_size += block->size + BLOCK_SIZE;
		return (block);
	}
	log_trace_if(config, "[MALLOC] No suitable free block,"
							" trying to create new one");
	block = create_block_in_zone(zone, size, config);
	if (!block)
	{
		log_trace_if(config, "[MALLOC] Failed to create block in existing zone,"
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
	void			*result;

	pthread_mutex_lock(&g_lock);
	config = init_debug_env();
	if (config->verbose)
	{
		print_custom("[MALLOC] MALLOC");
		print_size("[MALLOC] ", size);
	}
	if (log_fail_if(config, "[MALLOC] malloc forced to fail"))
	{
		pthread_mutex_unlock(&g_lock);
		return (NULL);
	}
	if (!check_size(size))
	{
		pthread_mutex_unlock(&g_lock);
		return (NULL);
	}
	block = NULL;
	zone_type = which_zone(size);
	zone = find_zone(zone_type, size);
	if (!zone)
		block = alloc_block_new_zone(size, zone_type, config);
	else
		block = alloc_block_in_existing_zone(zone, size, zone_type, config);
	if (!block)
	{
		pthread_mutex_unlock(&g_lock);
		return (NULL);
	}
	result = (void *)(block + 1);
	pthread_mutex_unlock(&g_lock);
	return (result);
}
