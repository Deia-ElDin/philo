/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dehamad <dehamad@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 22:39:59 by dehamad           #+#    #+#             */
/*   Updated: 2024/06/02 00:06:03 by dehamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/// @brief Used to convert a string to an integer
/// @param str The string to convert
/// @return A t_atoi struct containing the integer and an error flag
static t_atoi	ph_atoi(const char *str)
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

/// @brief It's a wrapper around the atoi function
/// @param data The data struct
/// @param av Each single argument
/// @param counter The counter to set with in the data struct
void	use_atoi(t_data *data, const char *av, int *counter)
{
	t_atoi	res;

	if (!av || !*av)
		exit_error(data);
	res = ph_atoi(av);
	if (res.error)
		exit_error(data);
	*counter = res.nbr;
}

/// @brief Allocates memory and initializes it to zero
/// @param count The number of elements to allocate
/// @param size The size of each element
/// @return The pointer to the allocated memory
void	*ph_calloc(size_t count, size_t size)
{
	void	*ptr;

	if (size && count > (UINT_MAX / size))
		return (NULL);
	ptr = malloc(size * count);
	if (!ptr)
		return (NULL);
	memset(ptr, 0, size * count);
	return (ptr);
}

/// @brief Get the current time in milliseconds
/// @param void
/// @return The current time in milliseconds
long	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

/// @brief Exit the program with an error message
/// @param data The data struct
void	exit_error(t_data *data)
{
	write(2, "Error\n", 6);
	data_cleanup(data);
	exit(EXIT_FAILURE);
}
