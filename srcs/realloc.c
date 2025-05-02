#include "../includes/realloc.h"

// TODO: reformat + add [REALLOC] in trace and debug
void	*realloc(void *ptr, size_t size)
{
	void			*result;
	t_block			*old_block;
	t_debug_config	*config;

	config = init_debug_env();
	if (config->verbose)
	{
		print_custom("[REALLOC] REALLOC");
		print_size("[REALLOC] ", size);
	}
	if (!ptr)
	{
		if (config->trace)
			print_trace("[REALLOC] NULL ptr passed to realloc: fallback to malloc");
		return (malloc(size));
	}
	if (size == 0)
	{
		if (config->trace)
			print_trace("[REALLOC] Realloc with size 0: freeing pointer");
		free(ptr);
		return (NULL);
	}
	old_block = (t_block *)ptr - 1;
	if (!old_block)
	{
		if (config->trace)
			print_trace("[REALLOC] Invalid block passed to realloc");
		return (NULL);
	}
	if (size <= old_block->size)
	{
		if (config->trace)
			print_trace("[REALLOC] Requested size <= current size: no reallocation needed");
		return (ptr);
	}
	if (config->trace)
		print_trace("[REALLOC] Allocating new block for realloc");
	result = malloc(size);
	if (!result)
		return (NULL);
	ft_memcpy(result, ptr, old_block->size);
	if (config->trace)
		print_trace("[REALLOC] Copied old data to new block,"
					" freeing old block");
	free(ptr);
	return (result);
}
