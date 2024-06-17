/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dehamad <dehamad@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:06:43 by dehamad           #+#    #+#             */
/*   Updated: 2024/06/17 18:45:05 by dehamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_positive_number(const char *str)
{
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

static int	is_within_range(const char *str)
{
	t_atoi	res;

	res = ph_atoi(str);
	if (res.error)
		return (0);
	return (1);
}

static int	is_valid_philo_count(const char *str)
{
	t_atoi	res;

	res = ph_atoi(str);
	if (res.error || res.nbr < 1 || res.nbr > 200)
		return (0);
	return (1);
}

int	parsing(int ac, char **av)
{
	int		i;

	if (ac != 5 && ac != 6)
		return (0);
	i = 0;
	while (++i < ac)
	{
		if (i == 1 && !is_valid_philo_count(av[i]))
			return (0);
		else if (!is_positive_number(av[i]) || !is_within_range(av[i]))
			return (0);
	}
	return (1);
}
