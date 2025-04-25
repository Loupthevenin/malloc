#include "../includes/malloc.h"

t_block			*g_head_tiny = NULL;
t_block			*g_head_small = NULL;
t_block			*g_head_large = NULL;

static void	init_block(size_t size, t_block **block)
{
	(*block)->size = size;
	(*block)->is_free = 0;
	(*block)->next = NULL;
}

static void	add_to_end_of_block(t_block **g_head, t_block *new)
{
	t_block	*tmp;

	if (!*g_head)
		*g_head = new;
	else
	{
		tmp = *g_head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

static void	*get_memory(size_t size)
{
	ft_putstr_fd("Calling mmap with size: ", 1);
	ft_putnbr_fd(size, 1);
	ft_putchar_fd('\n', 1);
	return (mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1,
			0));
}

static size_t	get_zone_size(size_t max_alloc_size)
{
	size_t	page_size;
	size_t	total_max_alloc;
	size_t	total_zone;

	page_size = get_page_size();
	total_max_alloc = max_alloc_size + BLOCK_SIZE;
	total_zone = total_max_alloc * 100;
	return (((total_zone + page_size - 1) / page_size) * page_size);
}

int	handle_region(t_block **block, size_t size, int flags)
{
	size_t	total_size;

	if (flags == TINY_SIZE)
		total_size = get_zone_size(TINY_SIZE);
	else if (flags == SMALL_SIZE)
		total_size = get_zone_size(SMALL_SIZE);
	else if (flags == LARGE_SIZE)
		total_size = size + BLOCK_SIZE;
	else
		return (-1);
	*block = get_memory(total_size);
	if (*block == MAP_FAILED)
	{
		perror("mmap");
		return (-1);
	}
	init_block(size, block);
	if (flags == TINY_SIZE)
		add_to_end_of_block(&g_head_tiny, *block);
	else if (flags == SMALL_SIZE)
		add_to_end_of_block(&g_head_small, *block);
	else if (flags == LARGE_SIZE)
		add_to_end_of_block(&g_head_large, *block);
	else
		return (-1);
	return (0);
}

void	*malloc(size_t size)
{
	t_block	*block;

	ft_putstr_fd("CUSTOM MALLOC\n", 1);
	ft_putstr_fd("Malloc size: ", 1);
	ft_putnbr_fd(size, 1);
	ft_putchar_fd('\n', 1);
	block = NULL;
	if (size <= TINY_SIZE)
	{
		handle_region(&block, size, TINY_SIZE);
	}
	else if (size <= SMALL_SIZE)
	{
		handle_region(&block, size, SMALL_SIZE);
	}
	else
	{
		handle_region(&block, size, LARGE_SIZE);
	}
	return ((void *)(block + 1));
}
