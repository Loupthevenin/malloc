#include "../includes/libft_malloc.h"

int	main(void)
{
	void	*ptr1;
	void	*ptr2;
	void	*ptr3;

	ptr1 = malloc(64);
	ptr2 = malloc(512);
	ptr3 = malloc(2048);
	(void)ptr1;
	(void)ptr2;
	(void)ptr3;
	ft_printf("ptr1: %p\n", ptr1);
	ft_printf("ptr2: %p\n", ptr2);
	ft_printf("ptr3: %p\n", ptr3);
	return (0);
}
