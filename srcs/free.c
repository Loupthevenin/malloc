#include "../includes/free.h"

static int	is_zone_empty(t_zone *zone)
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

static void	remove_zone(t_zone *zone_to_remove)
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

// TODO: Fusionner les blocks free adjacents
void	free(void *ptr)
{
	t_block			*block;
	t_debug_config	*config;

	config = init_debug_env();
	if (config->verbose)
		print_custom("[FREE] FREE");
	if (!ptr)
	{
		if (config->trace)
			print_trace("[FREE] NULL pointer");
		return ;
	}
	block = is_valid_malloc_ptr(ptr);
	if (!block)
	{
		if (config->trace)
			print_trace("[FREE] Invalid or unknown pointer passed to free");
		return ;
	}
	if (block->is_free)
	{
		if (config->trace)
			print_trace("[FREE] Double free detected");
		return ;
	}
	if (config->trace)
		print_trace("[FREE] Freeing block");
	if (block->zone->zone_type == LARGE)
	{
		remove_zone(block->zone);
		if (config->trace)
			print_trace("[FREE] Unmapping LARGE zone");
		return ;
	}
	block->is_free = 1;
	// Attention si on fusionne les blocks !
	block->zone->used_size -= block->size + BLOCK_SIZE;
	if (config->trace)
		print_trace("[FREE] Marked block as free and updated zone usage");
	if (is_zone_empty(block->zone))
	{
		if (config->trace)
			print_trace("[FREE] Zone is empty, removing it");
		remove_zone(block->zone);
	}
}
