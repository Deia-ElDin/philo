/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dehamad <dehamad@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 22:42:38 by dehamad           #+#    #+#             */
/*   Updated: 2024/06/10 20:36:22 by dehamad          ###   ########.fr       */
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
		data->philo[i].nbr_of_philos = data->nbr_of_philos;
		data->philo[i].time_to_die = data->time_to_die;
		data->philo[i].time_to_eat = data->time_to_eat;
		data->philo[i].time_to_sleep = data->time_to_sleep;
		data->philo[i].must_eat_count = data->must_eat_count;
		data->philo[i].data = data;
		pthread_mutex_init(&data->philo[i].meal_lock, NULL);
		pthread_mutex_init(&data->philo[i].eat_counter, NULL);
	}
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->dead_mutex, NULL);
}

void	data_init(char **av, t_data *data)
{
    int size;
    
	data->forks = NULL;
	data->threads = NULL;
	data->philo = NULL;
	data->dead_philo = false;
	use_atoi(data, av[1], &data->nbr_of_philos);
	use_atoi(data, av[2], &data->time_to_die);
	use_atoi(data, av[3], &data->time_to_eat);
	use_atoi(data, av[4], &data->time_to_sleep);
	if (av[5])
		use_atoi(data, av[5], &data->must_eat_count);
	else
		data->must_eat_count = -1;
    size = data->nbr_of_philos;
	data->philo = malloc(size * sizeof(t_philo));
	data->forks = malloc(size * sizeof(pthread_mutex_t));
	data->threads = malloc(size * sizeof(pthread_t));
	data->forks_value = malloc(size * sizeof(int));
	if (!data->philo || !data->forks || !data->threads)
		exit_error(data);
	data_mutex(data);
}

void	data_cleanup(t_data *data)
{
	int	i;

	if (data->threads)
		free(data->threads);
	if (data->forks)
	{
		i = -1;
		while (++i < data->nbr_of_philos)
		{
			pthread_mutex_destroy(&data->forks[i]);
			pthread_mutex_destroy(&data->philo[i].meal_lock);
			pthread_mutex_destroy(&data->philo[i].eat_counter);
		}
		free(data->forks);
	}
	if (data->philo)
		free(data->philo);
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

// printf("philo[%d].id: %d\n", i, data->philo[i].id);
// printf("philo[%d].left_fork: %d\n", i, data->philo[i].left_fork);
// printf("philo[%d].right_fork: %d\n", i, data->philo[i].right_fork);
// printf("philo[%d].meals_eaten: %d\n", i, data->philo[i].meals_eaten);
// printf("philo[%d].last_meal_time: %ld\n", i, data->philo[i].last_meal_time);
// printf("philo[%d].data: %p\n", i, data->philo[i].data);

// printf("nbr_of_philos: %d\n", data->nbr_of_philos);
// printf("time_to_die: %d\n", data->time_to_die);
// printf("time_to_eat: %d\n", data->time_to_eat);
// printf("time_to_sleep: %d\n", data->time_to_sleep);
// printf("must_eat_count: %d\n", data->must_eat_count);
// printf("start_time: %ld\n", data->start_time);