/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:09:47 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/16 18:12:11 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static int	start_simulation(t_table *table)
{
	int			i;
	pthread_t	monitor;

	table->start_time = get_time_ms();
	i = 0;
	while (i < table->philo_count)
	{
		if (pthread_create(&table->philos[i].thread, NULL,
				philo_routine, &table->philos[i]))
			return (ERROR);
		i++;
	}
	if (pthread_create(&monitor, NULL, monitor_routine, table))
		return (ERROR);
	i = 0;
	while (i < table->philo_count)
	{
		if (pthread_join(table->philos[i].thread, NULL))
			return (ERROR);
		i++;
	}
	if (pthread_join(monitor, NULL))
		return (ERROR);
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc != 5 && argc != 6)
		return (error_exit("Usage: ./philo number_of_philosophers time_to_die \
time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]"));
	if (init_table(&table, argc, argv))
		return (ERROR);
	if (start_simulation(&table))
	{
		cleanup(&table);
		return (ERROR);
	}
	cleanup(&table);
	return (SUCCESS);
}
