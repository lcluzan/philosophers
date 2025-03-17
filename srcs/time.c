/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:12:56 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/16 18:38:48 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/**
 * @brief Obtient le temps actuel en millisecondes
 * Utilise gettimeofday pour récupérer le temps système
 * et le convertit en millisecondes.
 *
 * @return Temps actuel en millisecondes
 */
long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/**
 * @brief Fonction d'attente précise en millisecondes
 * Implémente une attente active plus précise que usleep
 * pour les durées spécifiées. Vérifie périodiquement si la simulation
 * doit continuer et retourne prématurément si elle doit s'arrêter.
 *
 * @param ms Durée d'attente en millisecondes
 * @param table Structure principale contenant les données de la simulation
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
 * @brief Calcule le temps écoulé depuis le début de la simulation
 *
 * @param table Structure principale contenant les données de la simulation
 * @return Temps écoulé en millisecondes
 */
long	time_since_start(t_table *table)
{
	return (get_time_ms() - table->start_time);
}
