/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dehamad <dehamad@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 09:06:43 by dehamad           #+#    #+#             */
/*   Updated: 2024/06/10 13:54:04 by dehamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	is_positive_number(const char *str)
{
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (false);
		str++;
	}
	return (true);
}

static bool	is_within_range(const char *str)
{
	t_atoi	res;

	res = ph_atoi(str);
	if (res.error)
		return (false);
	return (true);
}

bool	parsing(int ac, char **av)
{
	int		i;

	if (ac != 5 && ac != 6)
		return (false);
	i = 0;
	while (++i < ac)
		if (!is_positive_number(av[i]) || !is_within_range(av[i]))
			return (false);
	return (true);
}
