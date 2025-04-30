#include "test.h"

///////////////////////////////////////////
static void	print_display_tester(void)
{
	ft_printf("==========================");
	ft_printf("TESTER MALLOC");
	ft_printf("==========================\n");
}

static void	print_display_test(int i)
{
	ft_putstr_fd("***TEST*** ", 1);
	ft_putnbr_fd(i, 1);
	ft_putstr_fd("\n", 1);
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

static void	free_all_zones(void)
{
	t_zone	*zone;
	t_zone	*next;

	zone = g_zone;
	while (zone)
	{
		next = zone->next;
		free(zone);
		zone = next;
	}
	g_zone = NULL;
}

// TODO: loop_random size malloc
int	main(void)
{
	print_display_tester();
	print_display_test(1);
	print_config();
	/* test_malloc(); */
	// functions
	test_find_zone();
	test_find_free_block();
	test_add_block_to_zone();
	/* show_alloc_mem(); */
	free_all_zones();
	return (0);
}
