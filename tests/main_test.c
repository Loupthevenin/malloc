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
void	free_all_zones(void)
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

void	test_threads(void)
{
	test_concurrent_malloc();
	test_concurrent_realloc();
	test_concurrent_show_alloc();
	test_concurrent_double_free();
}

// TODO: loop_random size malloc
int	main(void)
{
	print_display_tester();
	print_config();
	test_threads();
	return (0);
}
