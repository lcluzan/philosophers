/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:33:27 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/13 14:04:29 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/* Cette fonction gère l'action de manger d'un philosophe */
void	philo_eat(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&(philo->left_fork->mutex));
	print_status(data, philo->id, "has taken a fork");
	pthread_mutex_lock(&(philo->right_fork->mutex));
	print_status(data, philo->id, "has taken a fork");
	pthread_mutex_lock(&(data->meal_mutex));
	philo->is_eating = 1;
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&(data->meal_mutex));
	print_status(data, philo->id, "is eating");
	precise_sleep(data->time_to_eat);
	philo->meals_eaten++;
	philo->is_eating = 0;
	pthread_mutex_unlock(&(philo->right_fork->mutex));
	pthread_mutex_unlock(&(philo->left_fork->mutex));
}

/* Cette fonction gère l'action de dormir d'un philosophe */
void	philo_sleep(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	print_status(data, philo->id, "is sleeping");
	precise_sleep(data->time_to_sleep);
}

/* Cette fonction gère l'action de penser d'un philosophe */
void	philo_think(t_philosopher *philo)
{
	print_status(philo->data, philo->id, "is thinking");
}

/* Vérifie si la simulation doit continuer */
int	simulation_should_continue(t_data *data)
{
	int	should_continue;

	pthread_mutex_lock(&(data->death_mutex));
	should_continue = !data->simulation_stop;
	pthread_mutex_unlock(&(data->death_mutex));
	return (should_continue);
}

/* La routine principale pour chaque thread philosophe */
void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;
	t_data			*data;

	philo = (t_philosopher *)arg;
	data = philo->data;
	if (philo->id % 2 == 0)
		precise_sleep(data->time_to_eat / 2);
	while (simulation_should_continue(data))
	{
		philo_eat(philo);
		if (data->nb_must_eat > 0 && philo->meals_eaten >= data->nb_must_eat)
			break ;
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}
