/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcluzan <lcluzan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:45:02 by lcluzan           #+#    #+#             */
/*   Updated: 2025/03/12 15:48:17 by lcluzan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>

typedef struct s_fork
{
	pthread_mutex_t	mutex;
	int				id;
}	t_fork;

typedef struct s_philosopher
{
	int				id;
	int				meals_eaten;
	int				is_eating;
	long long		last_meal_time;
	pthread_t		thread;
	t_fork			*left_fork;
	t_fork			*right_fork;
	struct s_data	*data;
}	t_philosopher;

typedef struct s_data
{
    int             nb_philos;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             nb_must_eat;
    int             simulation_stop;
    long long       start_time;
    t_philosopher   *philosophers;
    t_fork          *forks;
    pthread_mutex_t print_mutex;
    pthread_mutex_t meal_mutex;
    pthread_mutex_t death_mutex;
} t_data;

/* main.c */
int         create_philosophers(t_data *data);
void        join_philosophers(t_data *data);
void        cleanup(t_data *data);

/* init.c */
int         init_data(t_data *data, int argc, char **argv);
int         init_forks(t_data *data);
int         init_mutexes(t_data *data);
void        init_philosophers(t_data *data);
int         init_all(t_data *data, int argc, char **argv);

/* philosopher.c */
void        philo_eat(t_philosopher *philo);
void        philo_sleep(t_philosopher *philo);
void        philo_think(t_philosopher *philo);
int         simulation_should_continue(t_data *data);
void        *philosopher_routine(void *arg);

/* monitor.c */
int         is_philosopher_dead(t_data *data, t_philosopher *philo);
int         all_philosophers_ate_enough(t_data *data);
void        stop_simulation(t_data *data);
void        print_death(t_data *data, int id);
void        *monitor_routine(void *arg);

/* time.c */
long long   get_time(void);
void        precise_sleep(long long ms);

/* utils.c */
void        print_status(t_data *data, int id, char *status);
int         safe_atoi(const char *str);
int         check_args(int argc, char **argv);

#endif
