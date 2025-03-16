/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:13:18 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/16 18:38:08 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/**
 * @brief Fait prendre les fourchettes à un philosophe
 * Gère la prise des fourchettes dans un ordre différent selon l'ID du philosophe
 * pour éviter les deadlocks.
 *
 * @param philo Philosophe qui prend les fourchettes
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
 * @brief Fait manger un philosophe
 * Met à jour l'heure du dernier repas, incrémente le compteur de repas,
 * et gère la libération des fourchettes après le repas.
 *
 * @param philo Philosophe qui mange
 */
void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->is_eating = true;
	philo->last_meal_time = get_time_ms();
	print_status(philo, "is eating");
	pthread_mutex_unlock(&philo->meal_mutex);
	ft_sleep(philo->table->time_to_eat);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->is_eating = false;
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	pthread_mutex_unlock(&philo->right_fork->mutex);
	pthread_mutex_unlock(&philo->left_fork->mutex);
}

/**
 * @brief Fait dormir puis penser un philosophe
 * Simule les phases de sommeil et de réflexion d'un philosophe
 * en attendant les temps spécifiés.
 *
 * @param philo Philosophe qui dort puis pense
 */
void	sleep_and_think(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_sleep(philo->table->time_to_sleep);
	print_status(philo, "is thinking");
}

/**
 * @brief Routine principale d'un philosophe
 * Gère le cycle de vie d'un philosophe: prendre les fourchettes,
 * manger, dormir et penser, avec un cas spécial pour un seul philosophe.
 *
 * @param arg Pointeur vers la structure du philosophe
 * @return NULL à la fin de l'exécution
 */
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->table->philo_count == 1)
	{
		print_status(philo, "has taken a fork");
		ft_sleep(philo->table->time_to_die);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		ft_sleep(philo->table->time_to_eat / 2);
	while (get_simulation_status(philo->table))
	{
		take_forks(philo);
		eat(philo);
		sleep_and_think(philo);
	}
	return (NULL);
}
