/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dehamad <dehamad@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 16:49:52 by dehamad           #+#    #+#             */
/*   Updated: 2024/06/16 17:02:32 by dehamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <limits.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

enum
{
	THINKING,
	FORK,
	EATING,
	SLEEPING,
	DEAD,
};

typedef struct s_atoi
{
	long	nbr;
	bool	error;
}	t_atoi;

typedef struct s_philo
{
	int				id;
	int				first_fork;
	int				second_fork;
	int				meals_eaten;
	long			nbr_of_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			last_meal_time;
	long			must_eat_count;
	long			start_time;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	eat_counter;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	long			nbr_of_philos;
	long			must_eat_count;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	bool			dead_philo;
	pthread_mutex_t	print;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	*forks;
	int				*forks_value;
	pthread_t		*threads;
	t_philo			*philo;
}	t_data;

// ************ PARSING ************ //
bool	parsing(int ac, char **av);

// ************ DATA ************ //
int		data_init(char **av, t_data *data);
void	data_cleanup(t_data *data);

// ************ PHILO ************ //
void	philo_print(t_philo *philo, int status);
void	*philo_routine(void *arg);
void	*philo_monitor(void *arg);

// ************ UTILS ************ //
t_atoi	ph_atoi(const char *str);
int		use_atoi(const char *av, long *counter);
long	get_time(void);
int		check_death(t_data *data);
int		check_forks(t_philo *philo, int first_fork, int second_fork);
void	ph_usleep(t_data *data, long time);

#endif