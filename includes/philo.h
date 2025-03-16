/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:45:02 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/16 16:09:26 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>

# define SUCCESS 0
# define ERROR 1

typedef struct s_fork
{
	pthread_mutex_t	mutex;
	int				id;
	bool			is_taken;
}	t_fork;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	t_fork			*left_fork;
	t_fork			*right_fork;
	long			last_meal_time;
	int				meals_eaten;
	bool			is_eating;
	pthread_mutex_t	meal_mutex;
	struct s_table	*table;
}	t_philo;

typedef struct s_table
{
	int				philo_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	bool			simulation_running;
	long			start_time;
	t_philo			*philos;
	t_fork			*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	running_mutex;
}	t_table;

/* init.c */
int		init_table(t_table *table, int argc, char **argv);
int		init_philos(t_table *table);
int		init_forks(t_table *table);
int		init_mutexes(t_table *table);

/* routine.c */
void	*philo_routine(void *arg);
void	take_forks(t_philo *philo);
void	eat(t_philo *philo);
void	sleep_and_think(t_philo *philo);

/* monitor.c */
void	*monitor_routine(void *arg);
bool	check_death(t_table *table, int *i);
bool	check_all_ate(t_table *table);
bool	simulation_should_stop(t_table *table);
void	set_simulation_status(t_table *table, bool status);

/* time.c */
long	get_time_ms(void);
void	ft_sleep(long ms);
long	time_since_start(t_table *table);

/* utils.c */
int		ft_atoi(const char *str);
void	print_status(t_philo *philo, char *status);
int		error_exit(char *msg);
bool	get_simulation_status(t_table *table);

/* cleanup.c */
void	cleanup(t_table *table);
void	destroy_mutexes(t_table *table);

#endif
