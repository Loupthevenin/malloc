#include "../includes/free.h"

static t_block	*is_valid_malloc_ptr(void *ptr)
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

// TODO: implement prev in block for more optimization;
static void	merge_adjacent_blocks(t_block *block, t_debug_config *config)
{
	t_block	*current;

	if (block->next && block->next->is_free)
	{
		log_trace_if(config, "[FREE] Merging with next free block");
		block->size += block->next->size;
		block->zone->used_size -= BLOCK_SIZE + block->next->size;
		block->next = block->next->next;
	}
	current = block->zone->blocks;
	while (current && current->next != block)
		current = current->next;
	if (current && current->is_free)
	{
		log_trace_if(config, "[FREE] Merging with previous free block");
		current->size += block->size;
		current->zone->used_size -= BLOCK_SIZE + block->size;
		current->next = block->next;
	}
}

static void	free_block(t_block *block, t_debug_config *config)
{
	block->is_free = 1;
	block->zone->used_size -= block->size + BLOCK_SIZE;
	log_trace_if(config, "[FREE] Marked block as free and updated zone usage");
	merge_adjacent_blocks(block, config);
	if (is_zone_empty(block->zone))
	{
		log_trace_if(config, "[FREE] Zone is empty, removing it");
		remove_zone(block->zone);
	}
}

static void	handle_free(void *ptr, t_debug_config *config)
{
	t_block	*block;

	block = is_valid_malloc_ptr(ptr);
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
