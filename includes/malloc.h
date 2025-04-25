#ifndef MALLOC_H
# define MALLOC_H

# include "../libft/ft_printf/ft_printf.h"
# include "../libft/libft.h"
# include <stdlib.h>
# include <sys/mman.h>

typedef struct s_block
{
	size_t			size;
	int				is_free;
	struct s_block	*next;
}					t_block;

extern t_block		*g_head;

void				free(void *ptr);
void				*malloc(size_t size);
void				*realloc(void *ptr, size_t size);
void				show_alloc_mem(void);

#endif
