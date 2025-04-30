#include "test.h"

void	test_malloc(void)
{
	void	*ptr;
	char	*mem;

	ptr = malloc(TINY_SIZE);
	assert_ptr_not_null(ptr, "malloc(TINY_SIZE)");
	ptr = malloc(0);
	assert_ptr_not_null(ptr,
						"malloc(0) should still return (non-null per POSIX");
	mem = malloc(64);
	assert_ptr_not_null(ptr, "malloc(64)");
	ft_memset(mem, 'A', 64);
}

// negative_number flag -Werror
/* void	negative_number(void) */
/* { */
/* 	void	*ptr; */
/**/
/* 	ptr = malloc(-1); */
/* } */

void	first_test(void)
{
	void	*ptr1;
	void	*ptr2;
	void	*ptr3;

	ptr1 = malloc(64);
	ptr2 = malloc(512);
	ptr3 = malloc(2048);
	ft_printf("ZONE TINY: ");
	ft_printf("ptr1: %p\n", ptr1);
	ft_printf("ZONE SMALL: ");
	ft_printf("ptr2: %p\n", ptr2);
	ft_printf("ZONE LARGE: ");
	ft_printf("ptr3: %p\n", ptr3);
}

void	loop_malloc(size_t size)
{
	int		i;
	int		max_alloc;
	char	*a;

	i = 0;
	max_alloc = 500;
	while (i < max_alloc)
	{
		a = malloc(size);
		i++;
	}
	(void)a;
	ft_putstr_fd("total malloc: ", 1);
	ft_putnbr_fd(i, 1);
	ft_putendl_fd("", 1);
}
