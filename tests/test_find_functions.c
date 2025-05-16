#include "test.h"

static t_zone	*create_test_zone(int zone_type, size_t zone_size)
{
	t_zone	*new_zone;

	new_zone = malloc(ZONE_SIZE);
	if (!new_zone)
		return (NULL);
	ft_memset(new_zone, 0, ZONE_SIZE);
	new_zone->zone_type = zone_type;
	new_zone->zone_size = zone_size;
	new_zone->used_size = 0;
	new_zone->blocks = NULL;
	new_zone->next = NULL;
	return (new_zone);
}

static t_block	*create_test_block(size_t size, int is_free, t_zone *zone)
{
	t_block	*new_block;

	new_block = malloc(BLOCK_SIZE);
	if (!new_block)
		return (NULL);
	ft_memset(new_block, 0, BLOCK_SIZE);
	new_block->size = size;
	new_block->is_free = is_free;
	new_block->next = NULL;
	new_block->zone = zone;
	return (new_block);
}

// Test function find_zone();
void	test_find_zone(void)
{
	t_zone	*zone1;
	t_zone	*zone2;
	t_zone	*zone3;
	t_zone	*result;

	ft_putstr_fd("=== Test find_zone ===\n", 1);
	zone1 = create_test_zone(SMALL, 1024);
	zone2 = create_test_zone(TINY, 2048);
	zone3 = create_test_zone(SMALL, 2048);
	zone1->next = zone2;
	zone2->next = zone3;
	g_zone = zone1;
	// match zone
	result = find_zone(SMALL, 512);
	assert_ptr_not_null(result, "Should find zone1");
	// skip zone1;
	zone1->used_size = 1024 - (BLOCK_SIZE - 1); // presque plein
	result = find_zone(SMALL, 64);
	assert_ptr_equal(result, zone3, "Should skip full zone1 and find zone3");
	// no zone
	result = find_zone(LARGE, 512);
	assert_ptr_null(result, "Should return (NULL for unmatched zone type)");
	zone3->used_size = 2048 - 100;
	result = find_zone(SMALL, 200);
	assert_ptr_null(result,
					"Should return (NULL when no zone has enough space)");
	free(zone1);
	free(zone2);
	free(zone3);
	g_zone = NULL;
}

// Test function find_free_block();
void	test_find_free_block(void)
{
	t_zone	*zone;
	t_block	*block1;
	t_block	*block2;
	t_block	*found_block;

	ft_putstr_fd("=== Test find_free_block ===\n", 1);
	zone = create_test_zone(SMALL, 1024);
	block1 = create_test_block(128, 1, zone);
	block2 = create_test_block(256, 0, zone);
	zone->blocks = block1;
	block1->next = block2;
	// find free block
	found_block = find_free_block(zone, 64);
	assert_ptr_not_null(found_block,
						"find_free_block should find the first free block");
	found_block = find_free_block(zone, 512);
	assert_ptr_null(found_block,
					"find_free_block should return (NULL when no block fits");
	free(zone);
	free(block1);
	free(block2);
	g_zone = NULL;
}

// Test function add_block_to_zone();
void	test_add_block_to_zone(void)
{
	t_zone	*zone;
	t_block	*block;
	t_block	*current;

	ft_putstr_fd("=== Test add_block_to_zone ===\n", 1);
	zone = create_test_zone(SMALL, 1024);
	block = create_test_block(256, 1, zone);
	add_block_to_zone(zone, block);
	assert_ptr_not_null(zone->blocks,
						"add_block_to_zone should add the block to the zone");
	current = zone->blocks;
	assert_ptr_not_null(current,
						"Block should be the first element in the zone's blocks");
	assert_ptr_null(current->next, "Next pointer of the block should be NULL");
	free(zone);
	free(block);
	g_zone = NULL;
}

// Test block libre trouvé
void	test_reuse_free_block(void)
{
	t_zone			*zone;
	t_block			*block;
	t_block			*result;
	t_debug_config	config;

	ft_putstr_fd("=== Test reuse free block ===\n", 1);
	zone = create_test_zone(SMALL, SMALL_SIZE);
	block = create_test_block(128, 1, zone);
	zone->blocks = block;
	g_zone = zone;
	config.trace = 0;
	config.fail = 0;
	config.trace = 0;
	result = alloc_block_in_existing_zone(zone, 64, SMALL, &config);
	assert_ptr_not_null(result, "Expected block to be reused");
	assert(result == block);
	ft_putstr_fd("✅ PASS assert\n", 1);
	assert_int_equals(0, result->is_free, "Block should be marked as used");
	free(block);
	free(zone);
	g_zone = NULL;
}
