#include "../includes/free.h"

t_block	*is_valid_malloc_ptr(void *ptr, t_debug_config *config)
{
	t_zone	*zone;
	t_block	*block;

	zone = g_zone;
	while (zone)
	{
		block = zone->blocks;
		while (block)
		{
			if ((void *)(block + 1) == ptr)
				return (block);
			block = block->next;
		}
		zone = zone->next;
	}
	return (NULL);
}

static t_block	*merge_adjacent_blocks(t_block *block, t_debug_config *config)
{
	t_block	*prev;
	t_block	*next;
	t_zone	*zone;
	size_t	total_freed_size;

	next = block->next;
	prev = block->prev;
	zone = block->zone;
	total_freed_size = BLOCK_SIZE + block->size;
	if (next && next->is_free && next != block)
	{
		log_trace_if(config, "[FREE] Merging with next free block");
		block->size += next->size + BLOCK_SIZE;
		total_freed_size += BLOCK_SIZE;
		block->next = next->next;
		if (block->next)
			block->next->prev = block;
	}
	if (prev && prev->is_free && prev != block)
	{
		log_trace_if(config, "[FREE] Merging with previous free block");
		prev->size += block->size + BLOCK_SIZE;
		/* total_freed_size += BLOCK_SIZE; */
		prev->next = block->next;
		if (block->next)
			block->next->prev = prev;
		block = prev;
	}
	zone->used_size -= total_freed_size;
	return (block);
}

static void	free_block(t_block *block, t_debug_config *config)
{
	t_zone	*zone;

	zone = block->zone;
	block->is_free = 1;
	log_trace_if(config, "[FREE] Marked block as free");
	block = merge_adjacent_blocks(block, config);
	log_trace_if(config, "[FREE] Updated zone used");
	if (is_zone_empty(block->zone))
	{
		log_trace_if(config, "[FREE] Zone is empty, removing it");
		remove_zone(zone);
	}
}

static void	handle_free(void *ptr, t_debug_config *config)
{
	t_block	*block;

	block = is_valid_malloc_ptr(ptr, config);
	if (!block)
		return (log_trace_if(config,
								"[FREE] Invalid or unknown pointer passed to free"));
	if (block->is_free)
		return (log_trace_if(config, "[FREE] Double free detected"));
	log_trace_if(config, "[FREE] Freeing block");
	if (block->zone->zone_type == LARGE)
	{
		remove_zone(block->zone);
		return (log_trace_if(config, "[FREE] Unmapping LARGE zone"));
	}
	free_block(block, config);
}

void	free(void *ptr)
{
	t_debug_config	*config;

	pthread_mutex_lock(&g_lock);
	config = init_debug_env();
	if (config->verbose)
		print_custom("[FREE] FREE");
	if (log_fail_if(config, "[FREE] free forced to fail"))
	{
		pthread_mutex_unlock(&g_lock);
		return ;
	}
	if (!ptr)
	{
		log_trace_if(config, "[FREE] NULL pointer");
		pthread_mutex_unlock(&g_lock);
		return ;
	}
	handle_free(ptr, config);
	pthread_mutex_unlock(&g_lock);
}
