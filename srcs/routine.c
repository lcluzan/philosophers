/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:13:18 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/17 19:19:26 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/**
 * @brief Makes a philosopher take forks.
 * Handles fork-taking order to prevent deadlocks.
 *
 * @param philo Philosopher taking forks.
 */
void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->right_fork->mutex);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork->mutex);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->left_fork->mutex);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->right_fork->mutex);
		print_status(philo, "has taken a fork");
	}
}

/**
 * @brief Simulates a philosopher sleeping and thinking.
 *
 * @param philo Philosopher sleeping and thinking.
 */
void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->is_eating = true;
	philo->last_meal_time = get_time_ms();
	pthread_mutex_unlock(&philo->meal_mutex);
	print_status(philo, "is eating");
	ft_sleep(philo->table->time_to_eat, philo->table);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->is_eating = false;
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	pthread_mutex_unlock(&philo->right_fork->mutex);
	pthread_mutex_unlock(&philo->left_fork->mutex);
}

/**
 * @brief Prints a philosopher's status in a thread-safe manner.
 *
 * @param philo Philosopher whose status is printed.
 * @param status Description of the philosopher's action.
 */
void	sleep_and_think(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_sleep(philo->table->time_to_sleep, philo->table);
	print_status(philo, "is thinking");
}

/**
 * @brief Main routine for philosopher threads.
 * Handles the philosopher's cycle of taking forks, eating, sleeping, and
 * thinking.
 *
 * @param arg Pointer to philosopher structure.
 * @return NULL when execution ends.
 */
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->table->philo_count == 1)
	{
		print_status(philo, "has taken a fork");
		ft_sleep(philo->table->time_to_die, philo->table);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		ft_sleep(philo->table->time_to_eat / 2, philo->table);
	while (get_simulation_status(philo->table))
	{
		take_forks(philo);
		eat(philo);
		sleep_and_think(philo);
	}
	return (NULL);
}
