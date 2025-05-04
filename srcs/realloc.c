#include "../includes/realloc.h"

static void	*handle_null_ptr(size_t size, t_debug_config *config)
{
	log_trace_if(config,
					"[REALLOC] NULL ptr passed to realloc: fallback to malloc");
	return (malloc(size));
}

static void	*handle_zero_size(void *ptr, t_debug_config *config)
{
	log_trace_if(config, "[REALLOC] Realloc with size 0: freeing pointer");
	free(ptr);
	return (NULL);
}

static void	*realloc_block(void *ptr, size_t size, t_debug_config *config)
{
	t_block	*old_block;
	void	*result;

	old_block = (t_block *)ptr - 1;
	if (!old_block)
	{
		log_trace_if(config, "[REALLOC] Invalid block passed to realloc");
		return (NULL);
	}
	if (size <= old_block->size)
	{
		log_trace_if(config,
						"[REALLOC] Requested size <= current size: no reallocation needed");
		return (ptr);
	}
	log_trace_if(config, "[REALLOC] Allocating new block for realloc");
	result = malloc(size);
	if (!result)
		return (NULL);
	ft_memcpy(result, ptr, old_block->size);
	log_trace_if(config, "[REALLOC] Copied old data to new block,"
							" freeing old block");
	free(ptr);
	return (result);
}

void	*realloc(void *ptr, size_t size)
{
	t_debug_config	*config;

	config = init_debug_env();
	if (config->verbose)
	{
		print_custom("[REALLOC] REALLOC");
		print_size("[REALLOC] ", size);
	}
	if (log_fail_if(config, "[REALLOC] realloc forced to fail"))
		return (NULL);
	if (!ptr)
		return (handle_null_ptr(size, config));
	if (size == 0)
		return (handle_zero_size(ptr, config));
	return (realloc_block(ptr, size, config));
}
