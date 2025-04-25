#include "../includes/malloc.h"

int	main(void)
{
	void	*ptr1;
	void	*ptr2;

	ptr1 = malloc(10);
	ptr2 = malloc(10);
	(void)ptr1;
	(void)ptr2;
	return (0);
}
