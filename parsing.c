/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dehamad <dehamad@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:06:43 by dehamad           #+#    #+#             */
/*   Updated: 2024/06/28 16:38:07 by dehamad          ###   ########.fr       */
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
	if (res.error || (int)res.nbr < 0)
		return (0);
	return (1);
}

static int	is_valid_philo_count(const char *str)
{
	t_atoi	res;

	res = ph_atoi(str);
	if (res.error || (int)res.nbr < 1 || (int)res.nbr > 200)
		return (0);
	return (1);
}

int	parsing(int ac, char **av)
{
	int	i;

	if (ac != 5 && ac != 6)
		return (0);
	i = 1;
	while (i < ac)
	{
		if (i == 1)
		{
			if (!is_valid_philo_count(av[i]))
				return (0);
		}
		else
		{
			if (!is_positive_number(av[i]) || !is_within_range(av[i]))
				return (0);
		}
		i++;
	}
	return (1);
}
