#include "test.h"

void	test_free_simple(void)
{
	void	*ptr;

	ft_putstr_fd("=== Test free simple ===\n", 1);
	ptr = malloc(64);
	assert_ptr_not_null(ptr, "malloc(64)");
	free(ptr);
	show_alloc_mem();
}

static void	free_wrapper(void *ptr)
{
	free(ptr);
}

void	test_double_free(void)
{
	void	*ptr;

	ft_putstr_fd("=== Test double free ===\n", 1);
	ptr = malloc(128);
	assert_ptr_not_null(ptr, "malloc(128)");
	free_wrapper(ptr);
	free_wrapper(ptr);
	show_alloc_mem();
}

void	test_no_malloc(void)
{
	void	*ptr;

	ft_putstr_fd("=== Test no malloc ===\n", 1);
	ptr = "test";
	free_wrapper(ptr);
	show_alloc_mem();
}

void	test_free_null(void)
{
	ft_putstr_fd("=== Test free NULL ===\n", 1);
	free(NULL);
	show_alloc_mem();
}

void	test_free_all_zones(void)
{
	void	*tiny;
	void	*small;
	void	*large;

	ft_putstr_fd("=== Test free all zones ===\n", 1);
	tiny = malloc(TINY_SIZE);
	assert_ptr_not_null(tiny, "malloc(TINY_SIZE)");
	small = malloc(SMALL_SIZE);
	assert_ptr_not_null(small, "malloc(SMALL_SIZE)");
	large = malloc(100000);
	assert_ptr_not_null(large, "malloc(10000)");
	free(tiny);
	free(small);
	free(large);
	show_alloc_mem();
}

void	test_zone_removed_when_empty(void)
{
	void	*a;
	void	*b;

	ft_putstr_fd("=== Test zone removed when empty ===\n", 1);
	a = malloc(64);
	b = malloc(64);
	free(a);
	free(b);
	show_alloc_mem();
	a = malloc(SMALL_SIZE);
	b = malloc(SMALL_SIZE);
	free(a);
	free(b);
	show_alloc_mem();
	a = malloc(100000);
	free(a);
	show_alloc_mem();
}

void	test_free_and_reuse(void)
{
	void	*ptr1;
	void	*ptr2;

	ft_putstr_fd("=== Test free and reuse ===\n", 1);
	ptr1 = malloc(64);
	assert_ptr_not_null(ptr1, "malloc(64)");
	free(ptr1);
	ptr2 = malloc(64);
	assert_ptr_not_null(ptr2, "malloc(64)");
	assert(ptr1 == ptr2);
	free(ptr2);
}
