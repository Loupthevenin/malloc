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

void	illegal_number(void)
{
	void	*ptr;
	size_t	neg_value;
	size_t	overflow_addition;
	size_t	overflow_multiplication;

	neg_value = (size_t)-10;
	ptr = malloc(neg_value);
	assert_ptr_null(ptr, "negative number");
	ptr = malloc(0);
	assert_ptr_null(ptr, "malloc(0)");
	overflow_addition = SIZE_MAX - BLOCK_SIZE - ZONE_SIZE + 1;
	ptr = malloc(overflow_addition);
	assert_ptr_null(ptr, "overflow addition");
	overflow_multiplication = (SIZE_MAX / MIN_BLOCKS_PER_ZONE) - BLOCK_SIZE + 1;
	ptr = malloc(overflow_multiplication);
	assert_ptr_null(ptr, "overflow multiplication");
	ptr = malloc(2000);
	assert_ptr_not_null(ptr, "valid malloc");
	if (ptr)
		free(ptr);
}

void	first_test(void)
{
	void	*ptr1;
	void	*ptr2;
	void	*ptr3;
	void	*ptr4;
	void	*ptr5;
	void	*ptr6;

	ptr1 = malloc(64);
	ptr2 = malloc(512);
	ptr3 = malloc(2048);
	ptr4 = malloc(4000);
	ptr5 = malloc(8000);
	ptr6 = malloc(5000);
	(void)ptr1;
	(void)ptr2;
	(void)ptr3;
	(void)ptr4;
	(void)ptr5;
	(void)ptr6;
	show_alloc_mem();
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
