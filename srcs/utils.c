/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:12:29 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/17 19:23:07 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/**
 * @brief Converts a string to an integer.
 * Handles signs and whitespace.
 *
 * @param str String to convert.
 * @return Converted integer.
 */
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

/**
 * @brief Prints a philosopher's status in a thread-safe manner.
 *
 * @param philo Philosopher whose status is printed.
 * @param status Description of the philosopher's action.
 */
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

/**
 * @brief Displays an error message and returns ERROR.
 *
 * @param msg Error message to display.
 * @return Always returns ERROR.
 */
int	error_exit(char *msg)
{
	printf("Error: %s\n", msg);
	return (ERROR);
}

/**
 * @brief Retrieves the current simulation status in a thread-safe manner.
 *
 * @param table Main structure containing simulation data.
 * @return true if simulation is running, false otherwise.
 */
bool	get_simulation_status(t_table *table)
{
	bool	status;

	pthread_mutex_lock(&table->running_mutex);
	status = table->simulation_running;
	pthread_mutex_unlock(&table->running_mutex);
	return (status);
}
