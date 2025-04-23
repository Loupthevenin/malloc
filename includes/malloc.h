#ifndef MALLOC_H
# define MALLOC_H

# include "../libft/ft_printf/ft_printf.h"
# include "../libft/libft.h"
# include <stdlib.h>

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);

#endif
