#include "test.h"

static void	*thread_malloc(void *arg)
{
	int		i;
	void	*ptr;

	i = 0;
	(void)arg;
	while (i < 100)
	{
		ptr = malloc(TINY_SIZE);
		if (!ptr)
			return (NULL);
		free(ptr);
		i++;
	}
	return (NULL);
}

static void	*thread_realloc(void *arg)
{
	void	*ptr;
	int		i;
	size_t	new_size;

	i = 0;
	(void)arg;
	ptr = malloc(64);
	while (i < 100)
	{
		new_size = 128;
		ptr = realloc(ptr, new_size);
		ft_memset(ptr, i, new_size);
		i++;
	}
	free(ptr);
	return (NULL);
}

static void	*thread_with_show(void *arg)
{
	void	*ptr;
	int		i;

	(void)arg;
	for (i = 0; i < 50; i++)
	{
		ptr = malloc(32);
		ft_memset(ptr, i, 32);
		if (i % 10 == 0)
			show_alloc_mem();
		free(ptr);
	}
	return (NULL);
}

static void	*thread_double_free(void *arg)
{
	void	**ptr;

	ptr = (void **)arg;
	usleep(rand() % 1000);
	free(*ptr);
	return (NULL);
}

void	test_concurrent_malloc(void)
{
	pthread_t	threads[10];
	int			i;

	ft_putstr_fd("=== Test concurrent malloc ===\n", 1);
	i = 0;
	while (i < 10)
	{
		usleep(200);
		pthread_create(&threads[i], NULL, thread_malloc, NULL);
		i++;
	}
	i = 0;
	while (i < 10)
		pthread_join(threads[i++], NULL);
	ft_putstr_fd("✅ Concurrent malloc test passed\n", 1);
}

void	test_concurrent_realloc(void)
{
	pthread_t		threads[10];
	pthread_mutex_t	print_mutex;
	int				i;
	t_thread_arg	args[10];

	ft_putstr_fd("=== Test concurrent realloc ===\n", 1);
	pthread_mutex_init(&print_mutex, NULL);
	i = 0;
	while (i < 10)
	{
		usleep(200);
		args[i].id = i;
		args[i].mutex = &print_mutex;
		pthread_create(&threads[i], NULL, thread_realloc, &args[i]);
		i++;
	}
	i = 0;
	while (i < 10)
		pthread_join(threads[i++], NULL);
	pthread_mutex_destroy(&print_mutex);
	ft_putstr_fd("✅ Concurrent realloc test passed\n", 1);
}

void	test_concurrent_show_alloc(void)
{
	pthread_t		threads[10];
	t_thread_arg	args[10];
	int				i;

	ft_putstr_fd("=== Test concurrent show_alloc_mem ===\n", 1);
	i = 0;
	while (i < 10)
	{
		usleep(200);
		args[i].id = i;
		args[i].mutex = NULL;
		pthread_create(&threads[i], NULL, thread_with_show, &args[i]);
		i++;
	}
	i = 0;
	while (i < 10)
		pthread_join(threads[i++], NULL);
	ft_putstr_fd("✅ Concurrent show_alloc_mem test passed\n", 1);
}

void	test_concurrent_double_free(void)
{
	void	*ptr;

	pthread_t t1, t2;
	ft_putstr_fd("=== Test concurrent double free ===\n", 1);
	ptr = malloc(64);
	pthread_create(&t1, NULL, thread_double_free, &ptr);
	pthread_create(&t2, NULL, thread_double_free, &ptr);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	ft_putstr_fd("✅ Concurrent double free test done\n",
					1);
}
