#include "../includes/colors.h"
#include "../includes/libft_malloc.h"

static void	print_zone_header(t_zone *zone)
{
	ft_putstr_fd("Zone address: ", 1);
	ft_putptr(zone);
	ft_putstr_fd(" | Zone size: ", 1);
	ft_putsize_fd(zone->zone_size, 1);
	ft_putstr_fd(" bytes | Type: ", 1);
	if (zone->zone_type == TINY)
	{
		ft_putstr_fd(GREEN "TINY", 1);
	}
	else if (zone->zone_type == SMALL)
	{
		ft_putstr_fd(YELLOW "SMALL", 1);
	}
	else
	{
		ft_putstr_fd(RED "LARGE", 1);
	}
	ft_putendl_fd(RESET, 1);
}

static void	print_hexdump(void *ptr, size_t size)
{
	unsigned char	*data;
	size_t			i;

	data = (unsigned char *)ptr;
	i = 0;
	while (i < size && i < 16)
	{
		if (data[i] < 16)
			ft_putstr_fd("0", 1);
		ft_putsize_base_fd(data[i], "0123456789abcdef", 1);
		ft_putstr_fd(" ", 1);
		i++;
	}
	ft_putstr_fd("\n", 1);
}

static void	print_block_info(t_block *block)
{
	ft_putstr_fd("  Block address: ", 1);
	ft_putptr((void *)(block + 1));
	ft_putstr_fd(" | Block size: ", 1);
	ft_putsize_fd(block->size, 1);
	ft_putstr_fd(" bytes | Status: ", 1);
	if (block->is_free)
		ft_putstr_fd(GREEN "FREE\n" RESET, 1);
	else
		ft_putstr_fd(BLUE "USED\n" RESET, 1);
	ft_putstr_fd("  Hexdump of the block:\n", 1);
	ft_putstr_fd("  ", 1);
	print_hexdump((void *)(block + 1), block->size);
}

static void	print_zone_blocks(t_zone *zone)
{
	t_block	*block;

	block = zone->blocks;
	while (block)
	{
		if (!block->is_free)
			print_block_info(block);
		block = block->next;
	}
}

void	show_alloc_mem_ex(void)
{
	t_zone	*zone;

	zone = g_zone;
	while (zone)
	{
		if (!zone->blocks)
		{
			zone = zone->next;
			continue ;
		}
		print_zone_header(zone);
		print_zone_blocks(zone);
		zone = zone->next;
	}
}
