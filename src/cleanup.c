/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:14:42 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/16 18:30:02 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/**
 * @brief Détruit tous les mutex de la table
 * Libère les mutex d'impression, d'état de simulation et pour chaque philosophe,
 * détruit les mutex des fourchettes et des repas.
 *
 * @param table Structure principale contenant les données de la simulation
 */
void	destroy_mutexes(t_table *table)
{
	int	i;

	pthread_mutex_destroy(&table->print_mutex);
	pthread_mutex_destroy(&table->running_mutex);
	i = 0;
	while (i < table->philo_count)
	{
		pthread_mutex_destroy(&table->forks[i].mutex);
		pthread_mutex_destroy(&table->philos[i].meal_mutex);
		i++;
	}
}

/**
 * @brief Nettoie les ressources allouées pour la simulation
 * Détruit tous les mutex et libère la mémoire allouée pour les philosophes
 * et les fourchettes.
 *
 * @param table Structure principale contenant les données de la simulation
 */
void	cleanup(t_table *table)
{
	destroy_mutexes(table);
	if (table->philos)
		free(table->philos);
	if (table->forks)
		free(table->forks);
}
