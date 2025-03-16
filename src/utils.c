/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:12:29 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/16 18:39:34 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/**
 * @brief Convertit une chaîne de caractères en entier
 * Gère les signes, les espaces et convertit les chiffres en valeur numérique.
 *
 * @param str Chaîne de caractères à convertir
 * @return Entier résultant de la conversion
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
 * @brief Affiche le statut d'un philosophe
 * Imprime de manière thread-safe un message avec timestamp
 * indiquant l'action en cours du philosophe.
 *
 * @param philo Philosophe concerné
 * @param status Message décrivant l'action du philosophe
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
 * @brief Affiche un message d'erreur et retourne ERROR
 * Utilitaire pour gérer les erreurs de manière uniforme.
 *
 * @param msg Message d'erreur à afficher
 * @return Toujours ERROR
 */
int	error_exit(char *msg)
{
	printf("Error: %s\n", msg);
	return (ERROR);
}

/**
 * @brief Récupère l'état actuel de la simulation de manière thread-safe
 *
 * @param table Structure principale contenant les données de la simulation
 * @return true si la simulation est en cours, false sinon
 */
bool	get_simulation_status(t_table *table)
{
	bool	status;

	pthread_mutex_lock(&table->running_mutex);
	status = table->simulation_running;
	pthread_mutex_unlock(&table->running_mutex);
	return (status);
}
