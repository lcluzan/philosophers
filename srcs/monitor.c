/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:13:45 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/17 18:43:48 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/**
 * @brief Sets the simulation status in a thread-safe manner.
 *
 * @param table Main structure containing simulation data.
 * @param status New simulation status (true = running, false = stopped).
 */
void	set_simulation_status(t_table *table, bool status)
{
	pthread_mutex_lock(&table->running_mutex);
	table->simulation_running = status;
	pthread_mutex_unlock(&table->running_mutex);
}

/**
 * @brief Checks if a philosopher has starved.
 * Compares the time since the last meal with the time-to-die threshold.
 *
 * @param table Main structure containing simulation data.
 * @param i Pointer to the philosopher index being checked.
 * @return true if a philosopher has died, false otherwise.
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
 * @brief Checks if all philosophers have eaten the required number of meals.
 * If all have reached their meal limit, the simulation stops.
 *
 * @param table Main structure containing simulation data.
 * @return true if all have eaten enough, false otherwise.
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
 * @brief Monitor thread routine.
 * Continuously checks if a philosopher dies or if all have eaten enough.
 *
 * @param arg Pointer to the table structure.
 * @return NULL when execution ends.
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
