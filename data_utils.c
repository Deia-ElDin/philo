/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dehamad <dehamad@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 22:42:38 by dehamad           #+#    #+#             */
/*   Updated: 2024/06/02 00:26:09 by dehamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	data_mutex(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nbr_of_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		data->philo[i].id = i + 1;
		data->philo[i].left_fork = i;
		data->philo[i].right_fork = (i + 1) % data->nbr_of_philos;
		data->philo[i].meals_eaten = 0;
		data->philo[i].last_meal_time = data->start_time;
		data->philo[i].data = data;
		printf("philo[%d].id: %d\n", i, data->philo[i].id);
		printf("philo[%d].left_fork: %d\n", i, data->philo[i].left_fork);
		printf("philo[%d].right_fork: %d\n", i, data->philo[i].right_fork);
		printf("philo[%d].meals_eaten: %d\n", i, data->philo[i].meals_eaten);
		printf("philo[%d].last_meal_time: %ld\n", i, data->philo[i].last_meal_time);
		printf("philo[%d].data: %p\n", i, data->philo[i].data);
	}
	pthread_mutex_init(&data->print, NULL);
}

/// @brief Initializes the data struct
/// @param av The arguments passed to the program
/// @param data The data struct
void	data_init(char **av, t_data *data)
{
	data->forks = NULL;
	data->threads = NULL;
	data->philo = NULL;
	use_atoi(data, av[1], &data->nbr_of_philos);
	use_atoi(data, av[2], &data->time_to_die);
	use_atoi(data, av[3], &data->time_to_eat);
	use_atoi(data, av[4], &data->time_to_sleep);
	if (av[5])
		use_atoi(data, av[5], &data->must_eat_count);
	else
		data->must_eat_count = -1;
	data->start_time = get_current_time();
	data->full_philo = 0;
	data->dead_philo = 0;
	data->philo = ph_calloc(data->nbr_of_philos, sizeof(t_philo));
	data->forks = ph_calloc(data->nbr_of_philos, sizeof(pthread_mutex_t));
	data->threads = ph_calloc(data->nbr_of_philos, sizeof(pthread_t));
	if (!data->philo || !data->forks || !data->threads)
		exit_error(data);
	data_mutex(data);
	printf("nbr_of_philos: %d\n", data->nbr_of_philos);
	printf("time_to_die: %d\n", data->time_to_die);
	printf("time_to_eat: %d\n", data->time_to_eat);
	printf("time_to_sleep: %d\n", data->time_to_sleep);
	printf("must_eat_count: %d\n", data->must_eat_count);
	printf("start_time: %ld\n", data->start_time);
}

/// @brief Cleans up the data struct
/// @param data The data struct
void	data_cleanup(t_data *data)
{
	int	i;

	if (data->threads)
		free(data->threads);
	if (data->forks)
	{
		i = -1;
		while (++i < data->nbr_of_philos)
			pthread_mutex_destroy(&data->forks[i]);
		free(data->forks);
	}
	if (data->philo)
		free(data->philo);
	pthread_mutex_destroy(&data->print);
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