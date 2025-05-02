#include "test.h"

void	test_realloc_increase_size(void)
{
	void	*ptr;

	ft_putstr_fd("=== Test realloc increase size ===\n", 1);
	ptr = malloc(64);
	assert_ptr_not_null(ptr, "malloc(64)");
	ft_memset(ptr, 'A', 64);
	ptr = realloc(ptr, 128);
	assert_ptr_not_null(ptr, "realloc(128)");
	ft_memset(ptr, 'B', 128);
	free(ptr);
}

void	test_realloc_decrease_size(void)
{
	void	*ptr;

	ft_putstr_fd("=== Test realloc decrease size ===\n", 1);
	ptr = malloc(128);
	assert_ptr_not_null(ptr, "malloc(128)");
	ft_memset(ptr, 'A', 128);
	ptr = realloc(ptr, 64);
	assert_ptr_not_null(ptr, "realloc(64)");
	free(ptr);
}

void	test_realloc_null_pointer(void)
{
	void	*ptr;

	ft_putstr_fd("=== Test realloc null pointer ===\n", 1);
	ptr = realloc(NULL, 64);
	assert_ptr_not_null(ptr, "realloc(NULL, 64)");
	ft_memset(ptr, 'C', 64);
	free(ptr);
}

void	test_realloc_zero_size(void)
{
	void	*ptr;

	ft_putstr_fd("=== Test realloc 0 size ===\n", 1);
	ptr = malloc(64);
	assert_ptr_not_null(ptr, "malloc(64)");
	ptr = realloc(ptr, 0);
	assert_ptr_null(ptr, "realloc(0)");
}

void	test_realloc_null_pointer_zero_size(void)
{
	void	*ptr;

	ft_putstr_fd("=== Test realloc null pointer and 0 size ===\n", 1);
	ptr = realloc(NULL, 0);
	assert_ptr_null(ptr, "realloc(NULL, 0)");
}

void	test_realloc_memory_copy(void)
{
	char	*ptr;
	int		i;

	ft_putstr_fd("=== Test realloc memory copy ===\n", 1);
	ptr = malloc(64);
	assert_ptr_not_null(ptr, "malloc(64)");
	ft_memset(ptr, 'A', 64);
	ptr = realloc(ptr, 128);
	assert_ptr_not_null(ptr, "realloc(ptr, 128)");
	i = 0;
	while (i < 64)
	{
		assert_int_equals(ptr[i], 'A', "Memory A copied");
		i++;
	}
	ft_memset(ptr + 64, 'B', 64);
	free(ptr);
}
