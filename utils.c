/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dehamad <dehamad@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 22:39:59 by dehamad           #+#    #+#             */
/*   Updated: 2024/06/10 21:07:44 by dehamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_atoi	ph_atoi(const char *str)
{
	t_atoi	res;
	int		sign;

	res.nbr = 0;
	res.error = false;
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
			return (res.error = true, res.nbr = 0, res);
		if (res.nbr * sign < INT_MIN && sign < 0)
			return (res.error = true, res.nbr = 0, res);
		str++;
	}
	res.nbr *= sign;
	return (res);
}

void	use_atoi(t_data *data, const char *av, long *counter)
{
	t_atoi	res;

	if (!av || !*av)
		exit_error(data);
	res = ph_atoi(av);
	if (res.error)
		exit_error(data);
	*counter = res.nbr;
}

int		check_death(t_data *data)
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
