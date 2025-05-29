#include "test.h"

///////////////////////////////////////////
static void	print_display_tester(void)
{
	ft_printf("==========================");
	ft_printf("TESTER MALLOC");
	ft_printf("==========================\n");
}

static void	print_config(void)
{
	ft_putstr_fd("BLOCK_SIZE: ", 1);
	ft_putnbr_fd(BLOCK_SIZE, 1);
	ft_putstr_fd(", ZONE_SIZE: ", 1);
	ft_putnbr_fd(ZONE_SIZE, 1);
	ft_putendl_fd("", 1);
}
///////////////////////////////////////////
void	tester_malloc(void)
{
	test_malloc();
	test_malloc_structures();
}

void	tester_free(void)
{
	test_free_simple();
	test_free_null();
	test_free_all_zones();
	test_zone_removed_when_empty();
}

void	tester_realloc(void)
{
	test_realloc_increase_size();
	test_realloc_decrease_size();
	test_realloc_zero_size();
	test_realloc_memory_copy();
}

void	tester_function(void)
{
	test_find_zone();
	test_find_free_block();
	test_add_block_to_zone();
	test_reuse_free_block();
}

// TODO: loop_random size malloc
void	tester_stress(void)
{
	loop_malloc(100, 0);
}

// UP
void	tester_illegal(void)
{
	// MALLOC
	illegal_number();
	// REALLOC
	test_realloc_null_pointer();
	test_realloc_null_pointer_zero_size();
	// FREE
	test_double_free();
	test_no_malloc();
	test_free_and_reuse();
}

void	tester_threads(void)
{
	test_concurrent_malloc();
	test_concurrent_realloc();
	test_concurrent_show_alloc();
	test_concurrent_double_free();
}

void	tester_colors(void)
{
	test_ex();
	test_free_defragment();
}

int	main(void)
{
	print_display_tester();
	print_config();
	tester_malloc();
	tester_free();
	tester_realloc();
	return (0);
}
