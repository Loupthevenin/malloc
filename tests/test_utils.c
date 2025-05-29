#include "test.h"

int	assert_ptr_not_null(void *ptr, char *msg)
{
	if (!ptr)
	{
		ft_putstr_fd("❌ FAIL: ", 1);
		ft_putendl_fd(msg, 1);
		return (0);
	}
	else
	{
		ft_putstr_fd("✅ PASS: ", 1);
		ft_putstr_fd(msg, 1);
		ft_putstr_fd("\n", 1);
		ft_putptr(ptr);
		ft_putstr_fd("\n", 1);
		return (1);
	}
}

int	assert_ptr_null(void *ptr, char *msg)
{
	if (!ptr)
	{
		ft_putstr_fd("✅ PASS: ", 1);
		ft_putstr_fd(msg, 1);
		ft_putstr_fd("\n", 1);
		ft_putptr(ptr);
		ft_putstr_fd("\n", 1);
		return (0);
	}
	else
	{
		ft_putstr_fd("❌ FAIL: ", 1);
		ft_putendl_fd(msg, 1);
		return (1);
	}
}

void	assert_ptr_equal(void *a, void *b, char *msg)
{
	if (a != b)
	{
		ft_putstr_fd("❌ FAIL: ", 1);
		ft_putstr_fd(msg, 1);
		ft_putstr_fd(" (got ", 1);
		ft_putptr(a);
		ft_putstr_fd(", expected ", 1);
		ft_putptr(b);
		ft_putstr_fd("\n", 1);
	}
	else
	{
		ft_putstr_fd("✅ PASS: ", 1);
		ft_putstr_fd(msg, 1);
		ft_putstr_fd("\n", 1);
	}
}

void	assert_int_equals(int a, int b, char *msg)
{
	if (a != b)
	{
		ft_putstr_fd("❌ FAIL: ", 1);
		ft_putstr_fd(msg, 1);
		ft_putstr_fd(" (got ", 1);
		ft_putnbr_fd(b, 1);
		ft_putstr_fd(", expected ", 1);
		ft_putnbr_fd(a, 1);
		ft_putstr_fd(")\n", 1);
	}
	else
	{
		ft_putstr_fd("✅ PASS: ", 1);
		ft_putstr_fd(msg, 1);
		ft_putstr_fd("\n", 1);
	}
}

void	assert_true(int cond, char *msg)
{
	if (!cond)
	{
		ft_putstr_fd("❌ FAIL: ", 1);
		ft_putendl_fd(msg, 1);
	}
	else
	{
		ft_putstr_fd("✅ PASS: ", 1);
		ft_putstr_fd(msg, 1);
		ft_putstr_fd("\n", 1);
	}
}
