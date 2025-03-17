/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:13:45 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/17 17:06:18 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/**
 * @brief Modifie l'état de la simulation
 * Met à jour de manière thread-safe la variable qui indique
 * si la simulation est toujours en cours.
 *
 * @param table Structure principale contenant les données de la simulation
 * @param status Nouvel état de la simulation (true=en cours, false=terminée)
 */
void	set_simulation_status(t_table *table, bool status)
{
	pthread_mutex_lock(&table->running_mutex);
	table->simulation_running = status;
	pthread_mutex_unlock(&table->running_mutex);
}

/**
 * @brief Vérifie si un philosophe est mort de faim
 * Compare le temps écoulé depuis le dernier repas avec le temps limite,
 * et arrête la simulation si un philosophe est mort.
 *
 * @param table Structure principale contenant les données de la simulation
 * @param i Pointeur vers l'index du philosophe à vérifier
 * @return true si un philosophe est mort, false sinon
 */
bool	check_death(t_table *table, int *i)
{
	long	time_now;
	long	time_last_meal;

	pthread_mutex_lock(&table->philos[*i].meal_mutex);
	time_now = get_time_ms();
	time_last_meal = table->philos[*i].last_meal_time;
	if (!table->philos[*i].is_eating && time_last_meal > 0
		&& time_now - time_last_meal > table->time_to_die)
	{
		pthread_mutex_unlock(&table->philos[*i].meal_mutex);
		pthread_mutex_lock(&table->print_mutex);
		printf("%ld %d died\n", time_since_start(table), table->philos[*i].id);
		pthread_mutex_unlock(&table->print_mutex);
		set_simulation_status(table, false);
		return (true);
	}
	pthread_mutex_unlock(&table->philos[*i].meal_mutex);
	*i = (*i + 1) % table->philo_count;
	return (false);
}

/**
 * @brief Vérifie si tous les philosophes ont mangé suffisamment
 * Compte combien de philosophes ont atteint le nombre de repas requis,
 * et arrête la simulation si tous l'ont atteint.
 *
 * @param table Structure principale contenant les données de la simulation
 * @return true si tous les philosophes ont mangé suffisamment, false sinon
 */
bool	check_all_ate(t_table *table)
{
	int	i;
	int	finished;

	if (table->must_eat_count < 0)
		return (false);
	i = 0;
	finished = 0;
	while (i < table->philo_count)
	{
		pthread_mutex_lock(&table->philos[i].meal_mutex);
		if (table->philos[i].meals_eaten >= table->must_eat_count)
			finished++;
		pthread_mutex_unlock(&table->philos[i].meal_mutex);
		i++;
	}
	if (finished == table->philo_count)
	{
		set_simulation_status(table, false);
		return (true);
	}
	return (false);
}

/**
 * @brief Routine du thread moniteur
 * Surveille continuellement les philosophes pour détecter
 * si l'un d'eux meurt ou si tous ont mangé suffisamment.
 *
 * @param arg Pointeur vers la structure table
 * @return NULL à la fin de l'exécution
 */
void	*monitor_routine(void *arg)
{
	t_table	*table;
	int		i;

	table = (t_table *)arg;
	i = 0;
	while (get_simulation_status(table))
	{
		if (check_death(table, &i))
			break ;
		if (check_all_ate(table))
			break ;
		usleep(500);
	}
	return (NULL);
}
