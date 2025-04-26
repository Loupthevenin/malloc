#include "../includes/malloc.h"

void	init_block(size_t size, t_block **block)
{
	(*block)->size = size;
	(*block)->is_free = 0;
	(*block)->next = NULL;
}

// update with ZONE_SIZE
size_t	get_zone_size(size_t max_alloc_size)
{
	size_t	page_size;
	size_t	total_max_alloc;
	size_t	total_zone;

	page_size = get_page_size();
	total_max_alloc = max_alloc_size + BLOCK_SIZE;
	total_zone = total_max_alloc * 100;
	return (((total_zone + page_size - 1) / page_size) * page_size);
}

// new get_block_size;

void	print_custom(char *message)
{
	ft_putstr_fd(GREEN "CUSTOM ", 1);
	ft_putendl_fd(message, 1);
	ft_putstr_fd(RESET, 1);
}

void	print_size(size_t size)
{
	ft_putstr_fd(YELLOW "Alloc size: ", 1);
	ft_putnbr_fd(size, 1);
	ft_putstr_fd("\n" RESET, 1);
}

void	print_memory(size_t size)
{
	ft_putstr_fd(BLUE "Calling mmap with size: ", 1);
	ft_putnbr_fd(size, 1);
	ft_putstr_fd("\n" RESET, 1);
}
