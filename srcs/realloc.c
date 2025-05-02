#include "../includes/libft_malloc.h"

void	*realloc(void *ptr, size_t size)
{
	void	*result;
	t_block	*old_block;

	if (!ptr)
		return (malloc(size));
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	old_block = (t_block *)ptr - 1;
	if (!old_block)
		return (NULL);
	if (size <= old_block->size)
		return (ptr);
	result = malloc(size);
	if (!result)
		return (NULL);
	ft_memcpy(result, ptr, old_block->size);
	free(ptr);
	return (result);
}
