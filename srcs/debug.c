#include "../includes/debug.h"
#include "../includes/libft_malloc.h"

t_debug_config	*init_debug_env(void)
{
	static t_debug_config	config;
	static int				init = 0;
	char					*env;

	if (!init)
	{
		env = getenv(MALLOC_VERBOSE);
		if (env && ft_strncmp(env, "1", 1) == 0)
			config.verbose = 1;
		env = getenv(MALLOC_TRACE);
		if (env && ft_strncmp(env, "1", 1) == 0)
			config.trace = 1;
		env = getenv(MALLOC_FAIL);
		if (env && ft_strncmp(env, "1", 1) == 0)
			config.fail = 1;
		init = 1;
	}
	return (&config);
}
