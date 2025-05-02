#include "test.h"

void	test_malloc(void)
{
	void	*ptr;
	char	*mem;
	int		*small;
	char	*large;

	ptr = malloc(TINY_SIZE);
	assert_ptr_not_null(ptr, "malloc(TINY_SIZE)");
	mem = malloc(64);
	assert_ptr_not_null(mem, "malloc(64)");
	ft_memset(mem, 'A', 64);
	small = malloc(600);
	assert_ptr_not_null(small, "malloc(600)");
	ft_memset(small, 5, 600);
	large = malloc(60000);
	assert_ptr_not_null(large, "malloc(60000)");
	ft_memset(large, 'B', 60000);
	show_alloc_mem();
	if (ptr)
		free(ptr);
	if (mem)
		free(mem);
	if (small)
		free(small);
	if (large)
		free(large);
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
