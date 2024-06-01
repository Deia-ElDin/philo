/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dehamad <dehamad@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 16:49:52 by dehamad           #+#    #+#             */
/*   Updated: 2024/06/02 00:23:00 by dehamad          ###   ########.fr       */
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

typedef struct s_atoi
{
	long	nbr;
	bool	error;
}	t_atoi;

typedef struct s_philo
{
	int				id;
	int				left_fork;
	int				right_fork;
	int				meals_eaten;
	long			last_meal_time;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	int				nbr_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	int				full_philo;
	int				dead_philo;
	long			start_time;
	pthread_mutex_t	print;
	pthread_mutex_t	*forks;
	pthread_t		*threads;
	t_philo			*philo;
}	t_data;

// ************ DATA ************ //
void	data_init(char **av, t_data *data);
void	data_cleanup(t_data *data);

// ************ PHILO ************ //
void	*philo_routine(void *arg);
void	*philo_monitor(void *arg);

// ************ UTILS ************ //
void	use_atoi(t_data *data, const char *av, int *counter);
void	*ph_calloc(size_t count, size_t size);
long	get_current_time(void);
void	exit_error(t_data *data);

#endif