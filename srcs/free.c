#include "../includes/free.h"

// TODO: Fusionner les blocks free adjacents
// TODO: Supprimer les zones empty is_free 1;
void	free(void *ptr)
{
	t_block			*block;
	t_debug_config	*config;

	config = init_debug_env();
	if (config->verbose)
		print_custom("[FREE] FREE");
	if (!ptr)
		return ;
	block = (t_block *)ptr - 1;
	// TODO: Verification que ptr est bien return par malloc (! on doit loop sur toutes la memoire) avec env variable ?
	if (config->verbose)
		print_size("[FREE] ", block->size);
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
}
