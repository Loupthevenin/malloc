#include "../includes/colors.h"
#include "../includes/libft_malloc.h"

static void	print_zone_header(t_zone *zone)
{
	if (zone->zone_type == TINY)
		ft_putstr_fd("TINY: ", 1);
	else if (zone->zone_type == SMALL)
		ft_putstr_fd("SMALL: ", 1);
	else
		ft_putstr_fd("LARGE: ", 1);
	ft_putptr((void *)zone);
	ft_putendl_fd("", 1);
}

static size_t	print_block_info(t_block *block)
{
	void	*start;
	void	*end;

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
	ft_putsize_fd(block->size, 1);
	ft_putstr_fd(" bytes\n", 1);
	ft_putstr_fd(RESET, 1);
	return (block->size);
}

static size_t	print_zone_blocks(t_zone *zone)
{
	t_block	*block;
	size_t	total;

	total = 0;
	block = zone->blocks;
	while (block)
	{
		if (!block->is_free)
			total += print_block_info(block);
		block = block->next;
	}
	return (total);
}

void	show_alloc_mem(void)
{
	t_zone	*zone;
	size_t	total;

	pthread_mutex_lock(&g_lock);
	zone = g_zone;
	total = 0;
	while (zone)
	{
		if (!zone->blocks)
		{
			zone = zone->next;
			continue ;
		}
		print_zone_header(zone);
		total += print_zone_blocks(zone);
		zone = zone->next;
	}
	ft_putstr_fd("Total : ", 1);
	ft_putsize_fd(total, 1);
	ft_putstr_fd(" bytes\n", 1);
	pthread_mutex_unlock(&g_lock);
}
