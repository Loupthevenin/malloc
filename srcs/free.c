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
		// FIX: revoir logique pour large ptr
		if (munmap((void *)block->zone, block->zone->zone_size) == -1)
			perror("munmap");
		return ;
	}
	block->is_free = 1;
	print_size(block->size);
}
