#include "../includes/free.h"

/**
 * @brief is_valid_malloc_ptr - Vérifie si un pointeur a été alloué par malloc.
 *
 * Cette fonction parcourt toutes les zones et leurs blocs pour déterminer
 * si le pointeur donné correspond à un bloc alloué connu.
 *
 * @param ptr Le pointeur utilisateur
 * @return Un pointeur vers le bloc correspondant si trouvé, NULL sinon
 */
t_block	*is_valid_malloc_ptr(void *ptr)
{
	t_zone	*zone;
	t_block	*block;

	zone = g_zone;
	while (zone)
	{
		block = zone->blocks;
		while (block)
		{
			if ((void *)(block + 1) == ptr)
				return (block);
			block = block->next;
		}
		zone = zone->next;
	}
	return (NULL);
}

/**
 * @brief merge_adjacent_blocks
	- Fusionne les blocs libres voisins pour réduire la fragmentation.
 *
 * Si les blocs adjacents (précédent ou suivant) sont libres,
	cette fonction fusionne

	* avec le bloc actuel pour créer un bloc plus grand. Cela réduit la fragmentation
 * de la zone et permet une meilleure réutilisation.
 *
 * @param block Le bloc récement libéré
 * @param config Configuration debug pour les traces
 * @return Le bloc si fusionné
 */
static t_block	*merge_adjacent_blocks(t_block *block, t_debug_config *config)
{
	t_block	*prev;
	t_block	*next;
	t_zone	*zone;
	size_t	total_freed_size;

	next = block->next;
	prev = block->prev;
	zone = block->zone;
	total_freed_size = BLOCK_SIZE + block->size;
	if (next && next->is_free && next != block)
	{
		log_trace_if(config, "[FREE] Merging with next free block");
		block->size += next->size + BLOCK_SIZE;
		total_freed_size += BLOCK_SIZE;
		block->next = next->next;
		if (block->next)
			block->next->prev = block;
	}
	if (prev && prev->is_free && prev != block)
	{
		log_trace_if(config, "[FREE] Merging with previous free block");
		prev->size += block->size + BLOCK_SIZE;
		/* total_freed_size += BLOCK_SIZE; */
		prev->next = block->next;
		if (block->next)
			block->next->prev = prev;
		block = prev;
	}
	zone->used_size -= total_freed_size;
	return (block);
}

/**
 * @brief free_block
	- Marque un bloc comme libre et tente de nettoyer la zone si possible.
 *
 * Cette fonction est utilisée uniquement pour les zones de type TINY/SMALL.
 * - Marque le bloc comme libre.
 * - Fusionne les blocs adjacents libres pour éviter la fragmentation.
 * - Met à jour la taille utilisée de la zone.
 * - Détruit la zone si elle devient totalement vide.
 *
 * @param block Pointeur vers le bloc à libérer
 * @param config Configuration debug
 */
static void	free_block(t_block *block, t_debug_config *config)
{
	t_zone	*zone;

	zone = block->zone;
	block->is_free = 1;
	log_trace_if(config, "[FREE] Marked block as free");
	block = merge_adjacent_blocks(block, config);
	log_trace_if(config, "[FREE] Updated zone used");
	if (is_zone_empty(block->zone))
	{
		log_trace_if(config, "[FREE] Zone is empty, removing it");
		remove_zone(zone);
	}
}

/**
 * @brief handle_free - Gère la libération réelle d'un bloc mémoire.
 *
 * Cette fonction est appelée par free après les vérifications de base.
 * - Vérifie que le pointeur correspond bien à un bloc connu.
 * - Ignore les doubles-libérations.
 * - Unmap les zones LARGE (une zone = un bloc dans ce cas).
 * - Libère les blocs standard : free_block().
 *
 * @param ptr Le pointeur utilisateur à libérer
 * @param config Structure de configuration debug
 */
static void	handle_free(void *ptr, t_debug_config *config)
{
	t_block	*block;

	block = is_valid_malloc_ptr(ptr);
	if (!block)
		return (log_trace_if(config,
								"[FREE] Invalid or unknown pointer passed to free"));
	if (block->is_free)
		return (log_trace_if(config, "[FREE] Double free detected"));
	log_trace_if(config, "[FREE] Freeing block");
	if (block->zone->zone_type == LARGE)
	{
		remove_zone(block->zone);
		return (log_trace_if(config, "[FREE] Unmapping LARGE zone"));
	}
	free_block(block, config);
}

/**
 * @brief free - Libère un bloc mémoire précédemment alloué avec malloc/realloc
 *
 * Cette fonction encapsule la libération d'un bloc en s'assurant que :
 * - La mémoire est bien non-nulle.
 * - Un environnement de debug est correctement initialisé.
 * - Des conditions de test (MALLOC_FAIL) sont respectées.
 *
 * g_lock garantit une sécurité thread-safe lors de l'accès aux zones.
 *
 * @param ptr Pointeur vers le bloc à libérer
 */
void	free(void *ptr)
{
	t_debug_config	*config;

	pthread_mutex_lock(&g_lock);
	config = init_debug_env();
	if (config->verbose)
		print_custom("[FREE] FREE");
	if (log_fail_if(config, "[FREE] free forced to fail"))
	{
		pthread_mutex_unlock(&g_lock);
		return ;
	}
	if (!ptr)
	{
		log_trace_if(config, "[FREE] NULL pointer");
		pthread_mutex_unlock(&g_lock);
		return ;
	}
	handle_free(ptr, config);
	pthread_mutex_unlock(&g_lock);
}
