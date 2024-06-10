/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dehamad <dehamad@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 02:20:28 by dehamad           #+#    #+#             */
/*   Updated: 2024/06/10 21:10:49 by dehamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_print(t_philo *philo, int status)
{
	t_data	*data;
	long	start_time;
	int		id;

	data = philo->data;
	if (check_death(data))
		return ;
	pthread_mutex_lock(&philo->meal_lock);
	start_time = philo->start_time;
	pthread_mutex_unlock(&philo->meal_lock);
	id = philo->id;
	pthread_mutex_lock(&data->print);
	if (status == THINKING)
		printf("%ld %d is thinking.\n", get_time() - start_time, id);
	else if (status == FORK)
		printf("%ld %d has taken a fork.\n", get_time() - start_time, id);
	else if (status == EATING)
		printf("%ld %d is eating.\n", get_time() - start_time, id);
	else if (status == SLEEPING)
		printf("%ld %d is sleeping.\n", get_time() - start_time, id);
	else if (status == DEAD)
		printf("%ld %d died\n", get_time() - start_time, id);
	pthread_mutex_unlock(&data->print);
}

static void	philo_life(t_philo *philo)
{
	int	first_fork;
	int	second_fork;

	first_fork = philo->first_fork;
	second_fork = philo->second_fork;
	if (check_forks(philo, first_fork, second_fork))
		return ;
	pthread_mutex_lock(&philo->eat_counter);
	philo->last_meal_time = get_time();
	philo->must_eat_count--;
	pthread_mutex_unlock(&philo->eat_counter);
	ph_usleep(philo->data, philo->time_to_eat);
	philo->data->forks_value[first_fork] = philo->id;
	philo->data->forks_value[second_fork] = philo->id;
	if (check_death(philo->data))
	{
		pthread_mutex_unlock(&(philo->data->forks[second_fork]));
		pthread_mutex_unlock(&(philo->data->forks[first_fork]));
		return ;
	}
	pthread_mutex_unlock(&(philo->data->forks[second_fork]));
	pthread_mutex_unlock(&philo->data->forks[first_fork]);
	philo_print(philo, SLEEPING);
	ph_usleep(philo->data, philo->time_to_sleep);
	philo_print(philo, THINKING);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	pthread_mutex_lock(&philo->meal_lock);
	philo->start_time = get_time();
	pthread_mutex_unlock(&philo->meal_lock);
	usleep(5);
	if (philo->id % 2 == 0)
		ph_usleep(philo->data, philo->time_to_eat / 2);
	while (philo->must_eat_count)
	{
		if (check_death(data))
			break;
		philo_life(philo);
	}
	return (NULL);
}

void	*philo_monitor(void *arg)
{
	t_data	*data;
	int		i;
	int		count;

	data = (t_data *)arg;
	count = 0;
	while (1)
	{
		i = -1;
		while (++i < data->nbr_of_philos)
		{
			pthread_mutex_lock(&data->philo[i].eat_counter);
			if (get_time() - data->philo[i].last_meal_time
				> data->philo[i].time_to_die
				&& data->philo[i].must_eat_count != 0)
			{
				philo_print(&data->philo[i], DEAD);
				pthread_mutex_lock(&data->dead_mutex);
				data->dead_philo = true;
				pthread_mutex_unlock(&data->dead_mutex);
				pthread_mutex_unlock(&data->philo[i].eat_counter);
				return (NULL);
			}
			if (data->philo[i].must_eat_count == 0)
				count++;
			pthread_mutex_unlock(&data->philo[i].eat_counter);
		}
		usleep(1);
		if (count >= data->nbr_of_philos)
			return (NULL);
	}
	return (NULL);
}

