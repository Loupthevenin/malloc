#include "../includes/malloc.h"

void	show_alloc_mem(void)
{
	t_block	*current;

	current = g_head - 1;
	while (current)
	{
		// Block
		ft_putstr_fd("BLOCK at ", 1);
		ft_printf("%p", current);
		// size can't now
		// NEED ZONES
		// etat
		if (current->is_free)
			ft_putstr_fd("FREE\n", 1);
		else
			ft_putstr_fd("ALLOCATED\n", 1);
		current = current->next;
	}
}
