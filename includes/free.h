#ifndef FREE_H
# define FREE_H

# include "colors.h"
# include "debug.h"
# include "libft_malloc.h"

// DEBUG
void	print_custom(char *message);
void	print_size(char *message, size_t size);
void	print_memory(size_t size);
void	log_trace_if(t_debug_config *config, char *message);
int		log_fail_if(t_debug_config *config, char *message);
void	print_zone_debug(void);

// UTILS
int		is_zone_empty(t_zone *zone);
void	remove_zone(t_zone *zone_to_remove);
#endif
