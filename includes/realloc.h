#ifndef REALLOC_H
# define REALLOC_H

# include "colors.h"
# include "debug.h"
# include "libft_malloc.h"

// DEBUG
void	print_custom(char *message);
void	print_size(char *message, size_t size);
void	print_memory(size_t size);
void	log_trace_if(t_debug_config *config, char *message);

#endif
