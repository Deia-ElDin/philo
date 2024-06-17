/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dehamad <dehamad@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 02:20:28 by dehamad           #+#    #+#             */
/*   Updated: 2024/06/17 18:57:44 by dehamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_print(t_philo *philo, int status)
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
		printf("%ld %d died.\n", get_time() - start_time, id);
	pthread_mutex_unlock(&data->print);
}

static void	philo_life(t_philo *philo)
{
	int	first_fork;
	int	second_fork;

	first_fork = philo->first_fork;
	second_fork = philo->second_fork;
	if (check_death(philo->data))
		return ;
	if (check_forks(philo, first_fork, second_fork))
		return ;
	pthread_mutex_lock(&philo->eat_counter);
	philo->last_meal_time = get_time();
	philo->must_eat_count--;
	pthread_mutex_unlock(&philo->eat_counter);
	ph_usleep(philo->data, philo->time_to_eat);
	philo->data->forks_value[first_fork] = philo->id;
	philo->data->forks_value[second_fork] = philo->id;
	pthread_mutex_unlock(&(philo->data->forks[second_fork]));
	pthread_mutex_unlock(&(philo->data->forks[first_fork]));
	if (check_death(philo->data))
		return ;
	philo_print(philo, SLEEPING);
	ph_usleep(philo->data, philo->time_to_sleep);
	if (check_death(philo->data))
		return ;
	philo_print(philo, THINKING);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->data->nbr_of_philos == 1)
	{
		philo_print(philo, FORK);
		ph_usleep(philo->data, philo->time_to_die);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		ph_usleep(philo->data, philo->time_to_eat / 2);
	while (philo->must_eat_count)
	{
		if (check_death(data))
			break ;
		philo_life(philo);
	}
	return (NULL);
}

int	philo_still_alive(t_data *data, int *count)
{
	int	i;

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
			data->dead_philo = 1;
			pthread_mutex_unlock(&data->dead_mutex);
			pthread_mutex_unlock(&data->philo[i].eat_counter);
			return (0);
		}
		if (data->philo[i].must_eat_count == 0)
			(*count)++;
		pthread_mutex_unlock(&data->philo[i].eat_counter);
	}
	return (1);
}

void	*philo_monitor(void *arg)
{
	t_data	*data;
	int		count;

	data = (t_data *)arg;
	count = 0;
	while (1)
	{
		if (!philo_still_alive(data, &count))
			return (NULL);
		if (count >= data->nbr_of_philos)
			return (NULL);
	}
	return (NULL);
}
