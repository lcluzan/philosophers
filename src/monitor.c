/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:13:45 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/16 18:12:34 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	set_simulation_status(t_table *table, bool status)
{
	pthread_mutex_lock(&table->running_mutex);
	table->simulation_running = status;
	pthread_mutex_unlock(&table->running_mutex);
}

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
		usleep(1000);
	}
	return (NULL);
}
