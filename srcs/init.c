/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:11:03 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/16 18:36:08 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/**
 * @brief Initialise tous les mutex nécessaires pour la simulation
 * Crée les mutex pour l'impression, l'état de la simulation, les fourchettes
 * et les repas de chaque philosophe.
 *
 * @param table Structure principale contenant les données de la simulation
 * @return SUCCESS si l'initialisation réussit, ERROR sinon
 */
int	init_mutexes(t_table *table)
{
	int	i;

	if (pthread_mutex_init(&table->print_mutex, NULL))
		return (ERROR);
	if (pthread_mutex_init(&table->running_mutex, NULL))
	{
		pthread_mutex_destroy(&table->print_mutex);
		return (ERROR);
	}
	i = -1;
	while (++i < table->philo_count)
	{
		if (pthread_mutex_init(&table->forks[i].mutex, NULL))
			return (ERROR);
		if (pthread_mutex_init(&table->philos[i].meal_mutex, NULL))
			return (ERROR);
	}
	return (SUCCESS);
}

/**
 * @brief Initialise les fourchettes pour la simulation
 * Alloue de la mémoire pour les fourchettes et initialise leurs propriétés.
 *
 * @param table Structure principale contenant les données de la simulation
 * @return SUCCESS si l'initialisation réussit, ERROR sinon
 */
int	init_forks(t_table *table)
{
	int		i;
	t_fork	*forks;

	forks = (t_fork *)malloc(sizeof(t_fork) * table->philo_count);
	if (!forks)
		return (ERROR);
	table->forks = forks;
	i = -1;
	while (++i < table->philo_count)
	{
		table->forks[i].id = i;
		table->forks[i].is_taken = false;
	}
	return (SUCCESS);
}

/**
 * @brief Initialise les philosophes pour la simulation
 * Alloue de la mémoire pour les philosophes, initialise leurs propriétés
 * et attribue les fourchettes gauche et droite à chaque philosophe.
 *
 * @param table Structure principale contenant les données de la simulation
 * @return SUCCESS si l'initialisation réussit, ERROR sinon
 */
int	init_philos(t_table *table)
{
	int		i;
	int		next;
	t_philo	*philos;

	philos = (t_philo *)malloc(sizeof(t_philo) * table->philo_count);
	if (!philos)
		return (ERROR);
	table->philos = philos;
	i = -1;
	while (++i < table->philo_count)
	{
		next = (i + 1) % table->philo_count;
		table->philos[i].id = i + 1;
		table->philos[i].meals_eaten = 0;
		table->philos[i].last_meal_time = get_time_ms();
		table->philos[i].is_eating = false;
		table->philos[i].table = table;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[next];
	}
	return (SUCCESS);
}

/**
 * @brief Vérifie la validité des arguments fournis
 * S'assure que le nombre de philosophes, les temps et le nombre de repas
 * sont des valeurs positives et logiques.
 *
 * @param table Structure principale contenant les données de la simulation
 * @param argc Nombre d'arguments fournis au programme
 * @return SUCCESS si les arguments sont valides, ERROR sinon
 */
int	check_args(t_table *table, int argc)
{
	if (table->philo_count <= 0 || table->time_to_die <= 0
		|| table->time_to_eat <= 0 || table->time_to_sleep <= 0
		|| (argc == 6 && table->must_eat_count <= 0))
		return (error_exit("Invalid arguments"));
	return (SUCCESS);
}

/**
 * @brief Initialise la structure principale de la simulation
 * Parse les arguments, vérifie leur validité et initialise les fourchettes,
 * les philosophes et les mutex nécessaires.
 *
 * @param table Structure principale à initialiser
 * @param argc Nombre d'arguments fournis au programme
 * @param argv Tableau des arguments
 * @return SUCCESS si l'initialisation réussit, ERROR sinon
 */
int	init_table(t_table *table, int argc, char **argv)
{
	table->philo_count = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->simulation_running = true;
	table->must_eat_count = -1;
	if (argc == 6)
		table->must_eat_count = ft_atoi(argv[5]);
	if (check_args(table, argc))
		return (ERROR);
	if (init_forks(table))
		return (error_exit("Failed to initialize forks"));
	if (init_philos(table))
	{
		free(table->forks);
		return (error_exit("Failed to initialize philosophers"));
	}
	if (init_mutexes(table))
	{
		free(table->forks);
		free(table->philos);
		return (error_exit("Failed to initialize mutexes"));
	}
	return (SUCCESS);
}
