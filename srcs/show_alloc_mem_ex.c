#include "../includes/colors.h"
#include "../includes/libft_malloc.h"

static void	print_offset(size_t i)
{
	ft_putstr_fd(YELLOW "  ", 1);
	ft_putsize_base_fd(i, "0123456789", 1, 6);
	ft_putstr_fd("  " RESET, 1);
}

static void	print_hex(size_t i, size_t size, unsigned char *data)
{
	size_t			j;
	unsigned char	byte;

	j = 0;
	while (j < 32)
	{
		while (j < 32)
		{
			if (i + j < size)
			{
				byte = data[i + j];
				if (byte == 0x00)
					ft_putstr_fd(DARK_GRAY, 1);
				else if (byte == 0x20 || byte == 0x7F)
					ft_putstr_fd(RED, 1);
				else if (byte >= 0x20 && byte <= 0x7E)
					ft_putstr_fd(GREEN, 1);
				else if (byte >= 0x80 && byte <= 0xBF)
					ft_putstr_fd(CYAN, 1);
				else
					ft_putstr_fd(ORANGE, 1);
				ft_putsize_base_fd(byte, "0123456789abcdef", 1, 2);
				ft_putstr_fd(RESET, 1);
			}
			else
				ft_putstr_fd("  ", 1);
			if (j == 15)
				ft_putstr_fd(" ", 1);
			ft_putstr_fd(" ", 1);
			j++;
		}
	}
}

static void	print_ascii(size_t i, size_t size, unsigned char *data)
{
	size_t			j;
	unsigned char	c;

	j = 0;
	while (j < 32)
	{
		if (i + j < size)
		{
			c = data[i + j];
			if (ft_isprint(c))
			{
				ft_putstr_fd(GREEN, 1);
				ft_putchar_fd(c, 1);
				ft_putstr_fd(RESET, 1);
			}
			else
				ft_putstr_fd(RED "." RESET, 1);
		}
		else
			ft_putstr_fd(" ", 1);
		j++;
	}
}

static void	print_hexdump_full(void *ptr, size_t size)
{
	unsigned char	*data;
	size_t			i;

	data = (unsigned char *)ptr;
	i = 0;
	while (i < size)
	{
		// Offset
		print_offset(i);
		// Hex part
		print_hex(i, size, data);
		ft_putstr_fd(" | ", 1);
		// ASCII part
		print_ascii(i, size, data);
		ft_putstr_fd(" |\n", 1);
		i += 32;
	}
}

static void	print_zone_header(t_zone *zone)
{
	ft_putstr_fd(CYAN "Zone address: " RESET, 1);
	ft_putstr_fd(PURPLE, 1);
	ft_putptr(zone);
	ft_putstr_fd(RESET " | " CYAN "Zone size: " RESET, 1);
	ft_putstr_fd(BLUE, 1);
	ft_putsize_fd(zone->zone_size, 1);
	ft_putstr_fd(RESET " bytes | " CYAN "Type: " RESET, 1);
	if (zone->zone_type == TINY)
		ft_putstr_fd(GREEN "TINY", 1);
	else if (zone->zone_type == SMALL)
		ft_putstr_fd(YELLOW "SMALL", 1);
	else
		ft_putstr_fd(RED "LARGE", 1);
	ft_putendl_fd(RESET, 1);
}

static void	print_block_info(t_block *block)
{
	ft_putstr_fd("  " CYAN "Block address: " RESET, 1);
	ft_putstr_fd(PURPLE, 1);
	ft_putptr((void *)(block + 1));
	ft_putstr_fd(RESET " | " CYAN "Block size: " RESET, 1);
	ft_putstr_fd(BLUE, 1);
	ft_putsize_fd(block->size, 1);
	ft_putstr_fd(RESET " bytes | " CYAN "Status: " RESET, 1);
	if (block->is_free)
		ft_putstr_fd(GREEN "FREE" RESET "\n", 1);
	else
		ft_putstr_fd(BLUE "USED" RESET "\n", 1);
	ft_putstr_fd("  " CYAN "Hexdump of the block:\n" RESET, 1);
	print_hexdump_full((void *)(block + 1), block->size);
}

static void	print_zone_blocks(t_zone *zone)
{
	t_block	*block;

	block = zone->blocks;
	while (block)
	{
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
