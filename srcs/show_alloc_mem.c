#include "../includes/colors.h"
#include "../includes/libft_malloc.h"

// FIX: int sur le ft_putnbr;
void	show_alloc_mem(void)
{
	t_zone	*zone;
	t_block	*block;
	void	*start;
	void	*end;
	size_t	total;

	zone = g_zone;
	total = 0;
	while (zone)
	{
		if (zone->zone_type == TINY)
			ft_putstr_fd("TINY: ", 1);
		else if (zone->zone_type == SMALL)
			ft_putstr_fd("SMALL: ", 1);
		else
			ft_putstr_fd("LARGE: ", 1);
		ft_putptr((void *)zone);
		ft_putendl_fd("", 1);
		block = zone->blocks;
		while (block)
		{
			if (!block->is_free)
			{
				start = (void *)(block + 1);
				end = (char *)start + block->size;
				if (IS_ALIGNED(start))
					ft_putstr_fd(CYAN, 1);
				else
					ft_putstr_fd(RED, 1);
				ft_putptr(start);
				ft_putstr_fd(" - ", 1);
				ft_putptr(end);
				ft_putstr_fd(" : ", 1);
				ft_putnbr_fd(block->size, 1);
				ft_putstr_fd(" bytes\n", 1);
				ft_putstr_fd(RESET, 1);
				total += block->size;
			}
			block = block->next;
		}
		zone = zone->next;
	}
	ft_putstr_fd("Total : ", 1);
	ft_putnbr_fd(total, 1);
	ft_putstr_fd(" bytes\n", 1);
}
