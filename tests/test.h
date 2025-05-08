#ifndef TEST_H
# define TEST_H

# include "../includes/libft_malloc.h"
# include "../includes/malloc.h"
# include <assert.h>
# include <pthread.h>

// UTILS
void				assert_ptr_not_null(void *ptr, char *msg);
void				assert_ptr_null(void *ptr, char *msg);
void				assert_ptr_equal(void *a, void *b, char *msg);
void				assert_int_equals(int a, int b, char *msg);
void				assert_true(int cond, char *msg);

typedef struct s_thread_arg
{
	int				id;
	void			*ptrs[100];
	pthread_mutex_t	*mutex;
}					t_thread_arg;

// *** TESTING *** //
///////// MALLOC /////////
void				illegal_number(void);
void				test_malloc(void);
void				loop_malloc(size_t size, int show_alloc);
void				test_malloc_structures(void);
void				test_ex(void);

///////// FREE /////////
void				test_free_simple(void);
void				test_double_free(void);
void				test_no_malloc(void);
void				test_free_null(void);
void				test_free_all_zones(void);
void				test_zone_removed_when_empty(void);
void				test_free_and_reuse(void);
void				test_free_defragment(void);

///////// REALLOC /////////
void				test_realloc_increase_size(void);
void				test_realloc_decrease_size(void);
void				test_realloc_null_pointer(void);
void				test_realloc_zero_size(void);
void				test_realloc_null_pointer_zero_size(void);
void				test_realloc_memory_copy(void);

// *** Function TEST *** //
///////// MALLOC /////////
void				test_find_zone(void);
void				test_find_free_block(void);
void				test_add_block_to_zone(void);
void				test_reuse_free_block(void);

///////// THREADS /////////
void				test_concurrent_malloc(void);
void				test_concurrent_realloc(void);
void				test_concurrent_show_alloc(void);
void				test_concurrent_double_free(void);

#endif
