/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:57:16 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/12 15:59:57 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/* Vérifie si un philosophe est mort */
int	is_philosopher_dead(t_data *data, t_philosopher *philo)
{
	long long	current_time;

	pthread_mutex_lock(&(data->meal_mutex));
	current_time = get_time();
	if (!philo->is_eating && current_time - philo->last_meal_time
		> data->time_to_die)
	{
		pthread_mutex_unlock(&(data->meal_mutex));
		return (1);
	}
	pthread_mutex_unlock(&(data->meal_mutex));
	return (0);
}

/* Vérifie si tous les philosophes ont mangé suffisamment */
int	all_philosophers_ate_enough(t_data *data)
{
	int	i;
	int	finished;

	if (data->nb_must_eat <= 0)
		return (0);
	i = 0;
	finished = 1;
	while (i < data->nb_philos)
	{
		if (data->philosophers[i].meals_eaten < data->nb_must_eat)
		{
			finished = 0;
			break ;
		}
		i++;
	}
	return (finished);
}

/* Stoppe la simulation */
void	stop_simulation(t_data *data)
{
	pthread_mutex_lock(&(data->death_mutex));
	data->simulation_stop = 1;
	pthread_mutex_unlock(&(data->death_mutex));
}

/* Affiche le message de mort d'un philosophe */
void	print_death(t_data *data, int id)
{
	long long	current_time;

	current_time = get_time() - data->start_time;
	pthread_mutex_lock(&(data->print_mutex));
	printf("%lld %d died\n", current_time, id);
	pthread_mutex_unlock(&(data->print_mutex));
}

/* Fonction principale de surveillance */
void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (!data->simulation_stop)
	{
		i = 0;
		while (i < data->nb_philos)
		{
			if (is_philosopher_dead(data, &(data->philosophers[i])))
			{
				print_death(data, data->philosophers[i].id);
				stop_simulation(data);
				return (NULL);
			}
			i++;
		}
		if (all_philosophers_ate_enough(data))
		{
			stop_simulation(data);
			return (NULL);
		}
		precise_sleep(1);
	}
	return (NULL);
}
