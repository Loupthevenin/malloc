#ifndef TEST_H
# define TEST_H

# include "../includes/libft_malloc.h"
# include "../includes/malloc.h"

// UTILS
void	assert_ptr_not_null(void *ptr, char *msg);
void	assert_ptr_null(void *ptr, char *msg);
void	assert_ptr_equal(void *a, void *b, char *msg);

// TESTING
void	test_malloc(void);
void	loop_malloc(size_t size);

// Function TEST
void	test_find_zone(void);
void	test_find_free_block(void);
void	test_add_block_to_zone(void);

#endif
