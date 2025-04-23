#include "../includes/malloc.h"

int	main(void)
{
	void	*ptr1;
	void	*ptr2;

	ptr1 = malloc(10);
	ptr2 = malloc(10);
	ft_printf("Allocated ptr1: %p\n", ptr1);
	ft_printf("Allocated ptr2: %p\n", ptr2);
	return (0);
}
