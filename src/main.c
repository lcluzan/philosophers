/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:20:41 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/12 16:21:58 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/* Crée les threads des philosophes */
int	create_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_create(&(data->philosophers[i].thread), NULL,
				philosopher_routine, &(data->philosophers[i])) != 0)
			return (0);
		i++;
	}
	return (1);
}

/* Attend la fin des threads */
void	join_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		pthread_join(data->philosophers[i].thread, NULL);
		i++;
	}
}

/* Libère les ressources allouées */
void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_destroy(&(data->forks[i].mutex));
		i++;
	}
	pthread_mutex_destroy(&(data->print_mutex));
	pthread_mutex_destroy(&(data->meal_mutex));
	pthread_mutex_destroy(&(data->death_mutex));
	free(data->philosophers);
	free(data->forks);
}

/* Fonction principale */
int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	monitor;

	if (!check_args(argc, argv))
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}
	if (!init_all(&data, argc, argv))
	{
		printf("Error: Initialization failed\n");
		return (1);
	}
	if (!create_philosophers(&data))
	{
		printf("Error: Thread creation failed\n");
		cleanup(&data);
		return (1);
	}
	if (pthread_create(&monitor, NULL, monitor_routine, &data) != 0)
	{
		printf("Error: Monitor thread creation failed\n");
		cleanup(&data);
		return (1);
	}
	join_philosophers(&data);
	pthread_join(monitor, NULL);
	cleanup(&data);
	return (0);
}
