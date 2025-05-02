#ifndef DEBUG_H
# define DEBUG_H

// DEBUG MACRO
# define MALLOC_VERBOSE "MALLOC_VERBOSE"
# define MALLOC_TRACE "MALLOC_TRACE"
# define MALLOC_FAIL "MALLOC_FAIL"

typedef struct s_debug_config
{
	int			verbose;
	int			trace;
	int			fail;
}				t_debug_config;

t_debug_config	*init_debug_env(void);
#endif
