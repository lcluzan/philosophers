/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:12:56 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/17 19:20:33 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/**
 * @brief Gets the current time in milliseconds.
 *
 * @return Current time in milliseconds.
 */
long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/**
 * @brief Implements a precise sleep function.
 * Periodically checks if the simulation is still running.
 *
 * @param ms Duration to sleep in milliseconds.
 * @param table Main structure containing simulation data.
 */
void	ft_sleep(long ms, t_table *table)
{
	long	start;
	long	current;

	start = get_time_ms();
	while (get_simulation_status(table))
	{
		current = get_time_ms();
		if (current - start >= ms)
			break ;
		usleep(500);
	}
}

/**
 * @brief Computes elapsed time since the simulation started.
 *
 * @param table Main structure containing simulation data.
 * @return Elapsed time in milliseconds.
 */
long	time_since_start(t_table *table)
{
	return (get_time_ms() - table->start_time);
}
