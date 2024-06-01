/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dehamad <dehamad@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 02:20:28 by dehamad           #+#    #+#             */
/*   Updated: 2024/06/01 23:09:07 by dehamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/// @brief Deals with the thinking state of a philosopher
/// @param philo The philosopher
static void	philo_thinking(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->print);
	printf("%ld %d is thinking\n",
		get_current_time() - data->start_time, philo->id);
	pthread_mutex_unlock(&data->print);
}

/// @brief Deals with the eating state of a philosopher
/// @param philo The philosopher
static void	philo_eating(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->print);
	pthread_mutex_lock(&data->forks[philo->left_fork]);
	pthread_mutex_lock(&data->forks[philo->right_fork]);
	printf("%ld %d has taken a fork\n",
		get_current_time() - data->start_time, philo->id);
	printf("%ld %d is eating\n",
		get_current_time() - data->start_time, philo->id);
	philo->last_meal_time = get_current_time();
	philo->meals_eaten++;
	usleep(data->time_to_eat * 1000);
	pthread_mutex_unlock(&data->forks[philo->left_fork]);
	pthread_mutex_unlock(&data->forks[philo->right_fork]);
	pthread_mutex_unlock(&data->print);
}

/// @brief Deals with the sleeping state of a philosopher
/// @param philo The philosopher
static void	philo_sleeping(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->print);
	printf("%ld %d is sleeping.\n",
		get_current_time() - data->start_time, philo->id);
	pthread_mutex_unlock(&data->print);
	usleep(data->time_to_sleep * 1000);
}

/// @brief The routine of a philosopher
/// @param arg The type casted philosopher
/// @return The return value of the thread, Null if not needed
void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	while (1)
	{
		philo_thinking(philo);
		philo_eating(philo);
		philo_sleeping(philo);
	}
	return (NULL);
}

/// @brief Monitors the philosophers
/// @param arg The data struct type casted to void *
/// @return The return value of the thread, Null if not needed
void	*philo_monitor(void *arg)
{
	t_data	*data;
	long	current_time;
	long	death_time;
	int		i;

	data = (t_data *)arg;
	i = 0;
	while (1)
	{
		current_time = get_current_time();
		if (current_time - data->philo[i].last_meal_time > data->time_to_die)
		{
			death_time = current_time - data->start_time;
			pthread_mutex_lock(&data->print);
			printf("%ld %d died\n", death_time, data->philo[i].id);
			pthread_mutex_unlock(&data->print);
			data->dead_philo = 1;
			return (NULL);
		}
		i = (i + 1) % data->nbr_of_philos;
		usleep(1000);
	}
	return (NULL);
}
