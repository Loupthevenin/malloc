#include "../includes/free.h"

// TODO: Fusionner les blocks free adjacents
// TODO: Dans le cas de LARGE munmap le block entier
void	free(void *ptr)
{
	t_block	*block;

	print_custom("FREE");
	if (!ptr)
		return ;
	block = (t_block *)ptr - 1;
	// TODO: Verification que ptr est bien return par malloc (! on doit loop sur toutes la memoire)
	block->is_free = 1;
	print_size(block->size);
}
