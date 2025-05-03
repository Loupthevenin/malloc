#include "test.h"

void	test_malloc(void)
{
	void	*ptr;
	char	*mem;
	int		*small;
	char	*large;

	ft_putstr_fd("=== simple test malloc ===\n", 1);
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

	ft_putstr_fd("=== illegal number ===\n", 1);
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
	void	*ptrs[500];

	ft_putstr_fd("=== loop malloc ===\n", 1);
	i = 0;
	max_alloc = 500;
	while (i < max_alloc)
	{
		ptrs[i] = malloc(size);
		assert_ptr_not_null(ptrs[i], "loop_malloc()");
		i++;
	}
	i = 0;
	show_alloc_mem();
	while (i < max_alloc)
		free(ptrs[i++]);
}

void	test_malloc_structures(void)
{
	void	*ptr;
	t_zone	*z;
	t_block	*b;
	void	*expected_block_start;
	void	*expected_ptr;

	ft_putstr_fd("=== STRUCTURAL TEST START ===\n", 1);
	ptr = malloc(TINY_SIZE); // Déclenche creation d'une TINY zone
	assert_ptr_not_null(ptr, "malloc(TINY_SIZE)");
	// Vérifie zone globale
	z = g_zone;
	assert_ptr_not_null(z, "g_zone exists");
	assert_true(z->zone_type == TINY, "zone type == TINY");
	assert_true(z->blocks != NULL, "zone->blocks initialized");
	b = z->blocks;
	assert_ptr_not_null(b, "block created");
	assert_true(b->is_free == 0, "block not free");
	assert_true(b->size >= TINY_SIZE, "block size >= requested");
	// Vérifie l’alignement
	assert_true((uintptr_t)b % ALIGNMENT == 0, "block is aligned");
	// Vérifie l’emplacement du bloc dans la mémoire
	expected_block_start = align_ptr((char *)z + ZONE_SIZE);
	assert_true((void *)b == expected_block_start,
				"block is just after zone header");
	// Vérifie que malloc retourne bien l’adresse après le block header
	expected_ptr = (void *)(b + 1);
	assert_true(ptr == expected_ptr, "malloc returned payload ptr");
	ft_putstr_fd("✅ STRUCTURAL TEST OK\n", 1);
	free(ptr);
}
