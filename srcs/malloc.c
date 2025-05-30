#include "../includes/malloc.h"

t_zone			*g_zone = NULL;
pthread_mutex_t	g_lock = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief get_memory - Alloue un espace mémoire brut de taille donnée : mmap.
 *
 * Cette fonction encapsule l'appel système `mmap` pour obtenir un bloc
 * mémoire anonyme et privé. Elle sert à allouer des zones entières
 * (TINY, SMALL, LARGE)
 *
 * @param size Taille à allouer
 * @return Pointeur vers le début de la mémoire allouée,
 * ou MAP_FAILED en cas d'erreur
 */
static void	*get_memory(size_t size)
{
	return (mmap(
		NULL,                   // Laisser le système choisir l’adresse
		size,                   // Taille totale à allouer
		PROT_READ | PROT_WRITE, // Accès en lecture/écriture
		MAP_ANON | MAP_PRIVATE, // Mémoire anonyme (pas de fichier),
								// privée au processus
		-1,                     // Pas de fichier (puisque MAP_ANON)
		0                       // Offset ignoré (puisque pas de fichier)
		));
}

/**
 * @brief insert_zone_sorted - Insère une zone dans la liste globale g_zone,
	triée par adresse croissante.
 *
 * Ceci garantie un ordre stable des zones mémoire, ce qui facilite le debug
 * et éventuellement certaines stratégies de fusion/optimisation mémoire.
 *
 * @param new_zone La nouvelle zone à insérer dans la liste
 */
static void	insert_zone_sorted(t_zone *new_zone)
{
	t_zone	*current;

	if (!g_zone || (uintptr_t)new_zone < (uintptr_t)g_zone)
	{
		new_zone->next = g_zone;
		g_zone = new_zone;
		return ;
	}
	current = g_zone;
	while (current->next && (uintptr_t)current->next < (uintptr_t)new_zone)
		current = current->next;
	new_zone->next = current->next;
	current->next = new_zone;
}

/**
 * @brief create_zone
	- Alloue une nouvelle zone mémoire et initialise le premier bloc.
 *
 * La zone est allouée via get_memory (=mmap), puis insérée dans la liste triée.
 * On y place un premier bloc immédiatement après l'en-tête de zone.
 *
 * @param size Taille du bloc à allouer
 * @param zone_type Type de la zone (TINY, SMALL, LARGE)
 * @return Pointeur vers la zone allouée, ou NULL en cas d'échec
 */
static t_zone	*create_zone(size_t size, int zone_type)
{
	t_zone	*zone;
	t_block	*block;
	size_t	total_size;

	total_size = get_size(size, zone_type);
	zone = get_memory(total_size);
	if (zone == MAP_FAILED)
	{
		perror("mmap zone");
		return (NULL);
	}
	zone->zone_type = zone_type;
	zone->zone_size = total_size;
	zone->blocks = NULL;
	insert_zone_sorted(zone);
	// on donne de la memoire a block de telle sorte a ce qu'on soit juste apres zone;
	block = (t_block *)((char *)zone + ZONE_SIZE);
	block = (t_block *)align_ptr(block);
	init_block(zone, size, block);
	zone->blocks = block;
	zone->used_size = ZONE_SIZE + BLOCK_SIZE + size;
	return (zone);
}

/**
 * @brief alloc_block_new_zone
	- Alloue une nouvelle zone et retourne le premier bloc alloué.
 *
 * Cette fonction est appelée en dernier recours lorsqu'on ne peut ni réutiliser
 * un bloc libre, ni en ajouter un dans une zone existante. Elle crée une zone
 * entièrement nouvelle et retourne le bloc initial alloué dans celle-ci.
 *
 * @param size Taille du bloc à allouer
 * @param zone_type Type de la zone (TINY, SMALL, LARGE)
 * @param config Configuration de debug
 * @return Pointeur vers le bloc alloué dans la nouvelle zone,
 * ou NULL si échec d'allocation
 */
static t_block	*alloc_block_new_zone(size_t size, int zone_type,
		t_debug_config *config)
{
	t_zone	*zone;

	log_trace_if(config, "[MALLOC] Creating new zone");
	zone = create_zone(size, zone_type);
	if (!zone)
	{
		log_trace_if(config, "[MALLOC] Failed to create new zone");
		return (NULL);
	}
	log_trace_if(config, "[MALLOC] Zone created and first block initialized");
	return (zone->blocks);
}

/**
 * @brief create_block_in_zone
	- Alloue un nouveau bloc à l'intérieur d'une zone existante.
 *

	* Cette fonction tente de créer un bloc à la suite des blocs existants dans la zone,
 * en respectant les contraintes d'alignement et d'espace disponible. Si la zone
 * est pleine, elle retourne NULL.
 *
 * @param zone La zone dans laquelle on veut créer le bloc
 * @param size Taille demandée pour le bloc utilisateur
 * @param config Configuration de debug
 * @return Pointeur vers le bloc créé, ou NULL si échec
 */
static t_block	*create_block_in_zone(t_zone *zone, size_t size,
		t_debug_config *config)
{
	t_block	*result;
	t_block	*last;

	if (zone->used_size + BLOCK_SIZE + size > zone->zone_size)
	{
		log_trace_if(config,
						"[MALLOC] Not enough space in zone to create new block");
		return (NULL);
	}
	// Si aucun bloc dans la zone, on place le premier jsute après
	if (!zone->blocks)
		result = (t_block *)((char *)zone + ZONE_SIZE);
	else
	{
		// Sinon on va à la fin de la liste chaînée pour trouver le dernier bloc
		last = zone->blocks;
		while (last->next)
			last = last->next;
		result = (t_block *)((char *)(last + 1) + last->size);
	}
	result = (t_block *)align_ptr(result);
	init_block(zone, size, result);
	add_block_to_zone(zone, result);
	zone->used_size += BLOCK_SIZE + size;
	log_trace_if(config, "[MALLOC] New block created in existing zone");
	return (result);
}

/**
 * @brief alloc_block_in_existing_zone
	- Tente d'allouer un bloc dans une zone existante.
 *
 * Cette fonction cherche un bloc libre suffisament grand dans la zone donnée.
 * Si un bloc libre est trouvé, il peut être soit utilisé tel quel, soit splitté
 * en deux si sa taille le permet (fragmentation). Sinon,
	un nouveau bloc est créé dans cette zone.
 * En dernier recours, une nouvelle zone est allouée.
 *
 * @param zone Zone mémoire existante où l'on tente d'allouer
 * @param size Taille demandée par l'utilisateur
 * @param zone_type Type de zone (TINY, SMALL, LARGE)
 * @param config Configuration de debug
 * @return Pointeur vers le bloc alloué, ou NULL en cas d'échec
 */
t_block	*alloc_block_in_existing_zone(t_zone *zone, size_t size, int zone_type,
		t_debug_config *config)
{
	t_block	*block;
	t_block	*new_block;

	block = find_free_block(zone, size);
	if (block)
	{
		// Si le bloc est assez grand pour être splitté (padding minimum)
		if (block->size >= size + BLOCK_SIZE + 64)
		{
			log_trace_if(config,
							"[MALLOC] Splitting and reusing free block");
			new_block = (void *)((char *)(block + 1) + size);
			new_block = (t_block *)align_ptr(new_block);
			new_block->size = block->size - size - BLOCK_SIZE;
			new_block->is_free = 1;
			new_block->zone = block->zone;
			new_block->next = block->next;
			new_block->prev = block;
			if (block->next)
				block->next->prev = new_block;
			block->next = new_block;
			block->size = size;
		}
		else
			log_trace_if(config,
							"[MALLOC] Reusing free block without split");
		block->is_free = 0;
		zone->used_size += block->size + BLOCK_SIZE;
		return (block);
	}
	log_trace_if(config, "[MALLOC] No suitable free block,"
							" trying to create new one");
	block = create_block_in_zone(zone, size, config);
	if (!block)
	{
		log_trace_if(config, "[MALLOC] Failed to create block in existing zone,"
								"fallback to new zone");
		return (alloc_block_new_zone(size, zone_type, config));
	}
	return (block);
}

/**
 * @brief malloc - Point d'entrée de l'allocateur mémoire.
 *
 * Cette fonction est appelée par l'utilisateur pour demander une zone mémoire
 * de taille `size`. Elle choisit une zone adapté au besoin (TINY, SMALL,
		LARGE),
 * y alloue un bloc, et retourne un pointeur vers la mémoire utilisable.
 * Elle est thread-safe grâce à un mutex global.
 *
 * @param size: taille demandée (en octets)
 * @return pointeur vers la mémoire allouée, ou NULL en cas d'échec
 */
void	*malloc(size_t size)
{
	t_block			*block;
	t_zone			*zone;
	int				zone_type;
	t_debug_config	*config;
	void			*result;

	pthread_mutex_lock(&g_lock);
	// Lecture des variables d'environnement
	config = init_debug_env();
	if (config->verbose)
	{
		print_custom("[MALLOC] MALLOC");
		print_size("[MALLOC] ", size);
	}
	if (log_fail_if(config, "[MALLOC] malloc forced to fail"))
	{
		pthread_mutex_unlock(&g_lock);
		return (NULL);
	}
	// Tailles invalides
	if (!check_size(size))
	{
		pthread_mutex_unlock(&g_lock);
		return (NULL);
	}
	block = NULL;
	// Choix de la zone (TINY, SMALL, LARGE)
	zone_type = which_zone(size);
	// Recherche de la zone existante pouvant contenir ce bloc
	zone = find_zone(zone_type, size);
	// Allocation du bloc dans une nouvelle ou une zone existante
	if (!zone)
		block = alloc_block_new_zone(size, zone_type, config);
	else
		block = alloc_block_in_existing_zone(zone, size, zone_type, config);
	if (!block)
	{
		pthread_mutex_unlock(&g_lock);
		return (NULL);
	}
	result = (void *)(block + 1);
	pthread_mutex_unlock(&g_lock);
	return (result);
}
