/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:13:40 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/13 14:26:18 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/* Affiche un message d'état pour un philosophe */
void	print_status(t_data *data, int id, char *status)
{
	long long	current_time;

	pthread_mutex_lock(&(data->death_mutex));
	if (data->simulation_stop)
	{
		pthread_mutex_unlock(&(data->death_mutex));
		return ;
	}
	pthread_mutex_unlock(&(data->death_mutex));
	current_time = get_time() - data->start_time;
	pthread_mutex_lock(&(data->print_mutex));
	printf("%lld %d %s\n", current_time, id, status);
	pthread_mutex_unlock(&(data->print_mutex));
}

/* Version sécurisée de atoi */
int	safe_atoi(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		if (result * sign > INT_MAX || result * sign < INT_MIN)
			return (0);
		str++;
	}
	return (result * sign);
}

/* Vérifie si tous les arguments sont valides */
int	check_args(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc < 5 || argc > 6)
		return (0);
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (0);
			j++;
		}
		if (safe_atoi(argv[i]) <= 0)
			return (0);
		i++;
	}
	return (1);
}
