#include "../includes/malloc.h"

t_block	*g_head = NULL;

void	init_block(size_t size, t_block **block)
{
	(*block)->size = size;
	(*block)->is_free = 0;
	(*block)->next = NULL;
}

void	*malloc(size_t size)
{
	size_t	total_size;
	t_block	*block;
	t_block	*tmp;

	ft_putstr_fd("CUSTOM MALLOC\n", 1);
	total_size = size + sizeof(t_block);
	block = mmap(NULL, total_size, PROT_READ | PROT_WRITE,
			MAP_ANON | MAP_PRIVATE, -1, 0);
	if (block == MAP_FAILED)
	{
		perror("mmap");
		return (NULL);
	}
	init_block(size, &block);
	if (!g_head)
		g_head = block;
	else
	{
		tmp = g_head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = block;
	}
	return ((void *)(block + 1));
}
