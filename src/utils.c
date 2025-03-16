/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:12:29 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/16 18:13:06 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

void	print_status(t_philo *philo, char *status)
{
	long	timestamp;

	pthread_mutex_lock(&philo->table->print_mutex);
	if (get_simulation_status(philo->table))
	{
		timestamp = time_since_start(philo->table);
		printf("%ld %d %s\n", timestamp, philo->id, status);
	}
	pthread_mutex_unlock(&philo->table->print_mutex);
}

int	error_exit(char *msg)
{
	printf("Error: %s\n", msg);
	return (ERROR);
}

bool	get_simulation_status(t_table *table)
{
	bool	status;

	pthread_mutex_lock(&table->running_mutex);
	status = table->simulation_running;
	pthread_mutex_unlock(&table->running_mutex);
	return (status);
}
