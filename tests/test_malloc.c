#include "../includes/libft_malloc.h"

static void	print_display_tester(void)
{
	ft_printf("==========================");
	ft_printf("TEST MALLOC");
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

void	second_test(void)
{
	int		i;
	int		max_alloc;
	char	*a;

	i = 0;
	max_alloc = 101;
	while (i < max_alloc)
	{
		a = malloc(TINY_SIZE);
		i++;
	}
	(void)a;
	ft_putstr_fd("total malloc: ", 1);
	ft_putnbr_fd(i, 1);
	ft_putendl_fd("", 1);
}

int	main(void)
{
	print_display_tester();
	/* print_display_test(1); */
	/* first_test(); */
	print_display_test(2);
	print_config();
	second_test();
	return (0);
}
