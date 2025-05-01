#include "../includes/free.h"

// TODO: Fusionner les blocks free adjacents
void	free(void *ptr)
{
	t_block	*block;

	print_custom("FREE");
	if (!ptr)
		return ;
	block = (t_block *)ptr - 1;
	// TODO: Verification que ptr est bien return par malloc (! on doit loop sur toutes la memoire)
	if (block->zone->zone_type == LARGE)
	{
		if (munmap((void *)block->zone, block->zone->zone_size) == -1)
			perror("munmap");
		return ;
	}
	block->is_free = 1;
	// Attention si on fusionne les blocks !
	block->zone->used_size -= block->size + BLOCK_SIZE;
	print_size(block->size);
}
