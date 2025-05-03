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

static void	remove_zone(t_zone *target)
{
	t_zone	*current;
	t_zone	*prev;

	current = g_zone;
	prev = NULL;
	while (current)
	{
		if (current == target)
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

// TODO: Fusionner les blocks free adjacents
void	free(void *ptr)
{
	t_block			*block;
	t_debug_config	*config;

	if (!ptr)
		return ;
	config = init_debug_env();
	block = (t_block *)ptr - 1;
	// TODO: Verification que ptr est bien return par malloc (! on doit loop sur toutes la memoire) avec env variable ?
	if (config->verbose)
	{
		print_custom("[FREE] FREE");
		print_size("[FREE] ", block->size);
	}
	if (config->trace)
		print_trace("[FREE] Freeing block");
	if (block->zone->zone_type == LARGE)
	{
		if (config->trace)
			print_trace("[FREE] Unmapping LARGE zone");
		if (munmap((void *)block->zone, block->zone->zone_size) == -1)
			perror("munmap");
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
