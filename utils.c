/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dehamad <dehamad@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 22:39:59 by dehamad           #+#    #+#             */
/*   Updated: 2024/06/28 17:27:15 by dehamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_atoi	ph_atoi(const char *str)
{
	t_atoi	res;
	int		sign;

	res.nbr = 0;
	res.error = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		res.nbr = (res.nbr * 10) + (*str - '0');
		if (res.nbr > INT_MAX && sign > 0)
			return (res.error = 1, res.nbr = 0, res);
		if (res.nbr * sign < INT_MIN && sign < 0)
			return (res.error = 1, res.nbr = 0, res);
		str++;
	}
	res.nbr *= sign;
	return (res);
}

int	use_atoi(const char *av, long *counter)
{
	t_atoi	res;

	if (!av || !*av)
		return (1);
	res = ph_atoi(av);
	if (res.error)
		return (1);
	*counter = res.nbr;
	return (0);
}

int	check_death(t_data *data)
{
	pthread_mutex_lock(&data->dead_mutex);
	if (data->dead_philo)
	{
		pthread_mutex_unlock(&data->dead_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->dead_mutex);
	return (0);
}

int	check_forks(t_philo *philo, int first_fork, int second_fork)
{
	pthread_mutex_lock(&(philo->data->forks[first_fork]));
	if (philo->data->forks_value[first_fork] == philo->id)
	{
		pthread_mutex_unlock(&(philo->data->forks[first_fork]));
		return (1);
	}
	pthread_mutex_lock(&(philo->data->forks[second_fork]));
	if (philo->data->forks_value[second_fork] == philo->id)
	{
		pthread_mutex_unlock(&(philo->data->forks[second_fork]));
		pthread_mutex_unlock(&(philo->data->forks[first_fork]));
		return (1);
	}
	philo_print(philo, FORK);
	philo_print(philo, FORK);
	philo_print(philo, EATING);
	return (0);
}

void	ph_usleep(t_data *data, long time)
{
	long	start;

	start = get_time();
	while ((get_time() - start) < time)
	{
		usleep(500);
		if (check_death(data))
			break ;
	}
}
