/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:12:56 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/16 18:12:57 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_sleep(long ms)
{
	long	start;
	long	current;

	start = get_time_ms();
	while (1)
	{
		current = get_time_ms();
		if (current - start >= ms)
			break ;
		usleep(500);
	}
}

long	time_since_start(t_table *table)
{
	return (get_time_ms() - table->start_time);
}
