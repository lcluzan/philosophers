/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:16:43 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/12 16:18:55 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/* Initialise la structure principale */
int	init_data(t_data *data, int argc, char **argv)
{
	data->nb_philos = safe_atoi(argv[1]);
	data->time_to_die = safe_atoi(argv[2]);
	data->time_to_eat = safe_atoi(argv[3]);
	data->time_to_sleep = safe_atoi(argv[4]);
	data->simulation_stop = 0;
	if (argc == 6)
		data->nb_must_eat = safe_atoi(argv[5]);
	else
		data->nb_must_eat = -1;
	data->philosophers = malloc(sizeof(t_philosopher) * data->nb_philos);
	if (!data->philosophers)
		return (0);
	data->forks = malloc(sizeof(t_fork) * data->nb_philos);
	if (!data->forks)
	{
		free(data->philosophers);
		return (0);
	}
	return (1);
}

/* Initialise les fourchettes */
int	init_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_mutex_init(&(data->forks[i].mutex), NULL) != 0)
			return (0);
		data->forks[i].id = i;
		i++;
	}
	return (1);
}

/* Initialise les mutex de synchronisation */
int	init_mutexes(t_data *data)
{
	if (pthread_mutex_init(&(data->print_mutex), NULL) != 0)
		return (0);
	if (pthread_mutex_init(&(data->meal_mutex), NULL) != 0)
		return (0);
	if (pthread_mutex_init(&(data->death_mutex), NULL) != 0)
		return (0);
	return (1);
}

/* Initialise les philosophes */
void	init_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].meals_eaten = 0;
		data->philosophers[i].is_eating = 0;
		data->philosophers[i].last_meal_time = get_time();
		data->philosophers[i].data = data;
		data->philosophers[i].left_fork = &(data->forks[i]);
		if (i == data->nb_philos - 1)
			data->philosophers[i].right_fork = &(data->forks[0]);
		else
			data->philosophers[i].right_fork = &(data->forks[i + 1]);
		i++;
	}
}

/* Fonction principale d'initialisation */
int	init_all(t_data *data, int argc, char **argv)
{
	if (!init_data(data, argc, argv))
		return (0);
	if (!init_forks(data))
		return (0);
	if (!init_mutexes(data))
		return (0);
	init_philosophers(data);
	data->start_time = get_time();
	return (1);
}
