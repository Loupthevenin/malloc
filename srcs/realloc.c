#include "../includes/realloc.h"

void	*realloc(void *ptr, size_t size)
{
	void			*result;
	t_block			*old_block;
	t_debug_config	*config;

	config = init_debug_env();
	if (config->verbose)
	{
		print_custom("REALLOC");
		print_size(size);
	}
	if (!ptr)
	{
		if (config->trace)
			print_trace("NULL ptr passed to realloc: fallback to malloc");
		return (malloc(size));
	}
	if (size == 0)
	{
		if (config->trace)
			print_trace("Realloc with size 0: freeing pointer");
		free(ptr);
		return (NULL);
	}
	old_block = (t_block *)ptr - 1;
	if (!old_block)
	{
		if (config->trace)
			print_trace("Invalid block passed to realloc");
		return (NULL);
	}
	if (size <= old_block->size)
	{
		if (config->trace)
			print_trace("Requested size <= current size: no reallocation needed");
		return (ptr);
	}
	if (config->trace)
		print_trace("Allocating new block for realloc");
	result = malloc(size);
	if (!result)
		return (NULL);
	ft_memcpy(result, ptr, old_block->size);
	if (config->trace)
		print_trace("Copied old data to new block, freeing old block");
	free(ptr);
	return (result);
}
