/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dehamad <dehamad@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 22:42:38 by dehamad           #+#    #+#             */
/*   Updated: 2024/06/17 18:36:10 by dehamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	data_values(t_data *data, int i)
{
	data->philo[i].id = i + 1;
	if (data->philo[i].id % 2 == 0)
	{
		data->philo[i].first_fork = i;
		data->philo[i].second_fork = (i + 1) % data->nbr_of_philos;
	}
	else
	{
		data->philo[i].first_fork = (i + 1) % data->nbr_of_philos;
		data->philo[i].second_fork = i;
	}
	data->forks_value[i] = -1;
	data->philo[i].meals_eaten = 0;
	data->philo[i].last_meal_time = get_time();
	data->philo[i].start_time = get_time();
	data->philo[i].nbr_of_philos = data->nbr_of_philos;
	data->philo[i].time_to_die = data->time_to_die;
	data->philo[i].time_to_eat = data->time_to_eat;
	data->philo[i].time_to_sleep = data->time_to_sleep;
	data->philo[i].must_eat_count = data->must_eat_count;
	data->philo[i].data = data;
}

static void	data_mutex(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nbr_of_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_mutex_init(&data->philo[i].meal_lock, NULL);
		pthread_mutex_init(&data->philo[i].eat_counter, NULL);
		data_values(data, i);
	}
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->dead_mutex, NULL);
}

int	data_init(char **av, t_data *data)
{
	int	size;

	data->forks = NULL;
	data->threads = NULL;
	data->philo = NULL;
	data->forks_value = NULL;
	data->dead_philo = 0;
	if (use_atoi(av[1], &data->nbr_of_philos)
		|| use_atoi(av[2], &data->time_to_die)
		|| use_atoi(av[3], &data->time_to_eat)
		|| use_atoi(av[4], &data->time_to_sleep))
		return (1);
	if (av[5] && use_atoi(av[5], &data->must_eat_count))
		return (1);
	else if (!av[5])
		data->must_eat_count = -1;
	size = data->nbr_of_philos;
	data->philo = malloc(size * sizeof(t_philo));
	data->forks = malloc(size * sizeof(pthread_mutex_t));
	data->threads = malloc(size * sizeof(pthread_t));
	data->forks_value = malloc(size * sizeof(int));
	if (!data->philo || !data->forks || !data->threads || !data->forks_value)
		return (1);
	data_mutex(data);
	return (0);
}

void	data_cleanup(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = -1;
		while (++i < data->nbr_of_philos)
			pthread_mutex_destroy(&data->forks[i]);
		free(data->forks);
	}
	if (data->philo)
	{
		i = -1;
		while (++i < data->nbr_of_philos)
		{
			pthread_mutex_destroy(&data->philo[i].meal_lock);
			pthread_mutex_destroy(&data->philo[i].eat_counter);
		}
		free(data->philo);
	}
	if (data->threads)
		free(data->threads);
	if (data->forks_value)
		free(data->forks_value);
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->dead_mutex);
}

/*
*	pthread_mutex_init:
	- The pthread_mutex_init() function initializes the mutex
	referenced by mutex with attributes specified by attr.
	- Is a synchronization primitive used to prevent multiple threads 
	from simultaneously accessing a shared resource, 
	which can lead to race conditions and inconsistent data.

*	pthread_mutex_init(&data->print, NULL);
	- The print mutex is used to ensure that output to the console is done
	in a thread-safe manner. Without this mutex, multiple threads (philosophers) 
	could attempt to print messages at the same time, 
	resulting in garbled or interleaved output.

*	pthread_mutex_init(&data->forks[i], NULL);
	- The forks mutex array is used to synchronize access to the forks 
	on the table. Each fork is represented by a mutex, and philosophers 
	need to lock the appropriate mutexes before they can pick up the forks.
	- If two philosophers attempt to pick up the same fork at the same time, 
	they could both succeed, Leading to a deadlock situation.

* pthread_mutex_lock:
 	- locks a mutex
	- the argument is the mutex to lock
*/