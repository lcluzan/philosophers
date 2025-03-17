/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:14:42 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/17 18:35:04 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/**
 * @brief Destroys all mutexes used in the simulation.
 * This includes the print mutex, simulation state mutex, and each philosopher's
 * fork and meal mutexes.
 *
 * @param table Main structure containing simulation data.
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
 * @brief Cleans up allocated resources for the simulation.
 * Destroys mutexes and frees allocated memory for philosophers and forks.
 *
 * @param table Main structure containing simulation data.
 */
void	cleanup(t_table *table)
{
	destroy_mutexes(table);
	if (table->philos)
		free(table->philos);
	if (table->forks)
		free(table->forks);
}
