/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dehamad <dehamad@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 16:50:38 by dehamad           #+#    #+#             */
/*   Updated: 2024/06/10 21:07:20 by dehamad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	exit_error(t_data *data)
{
	data_cleanup(data);
	exit(EXIT_FAILURE);
}

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	main(int ac, char **av)
{
	t_data		data;
	pthread_t	*threads;
	pthread_t	monitor_thread;
	int			i;

	if (!parsing(ac, av))
		return (EXIT_FAILURE);
	data_init(av, &data);
	threads = data.threads;
	i = -1;
	while (++i < data.nbr_of_philos)
		if (pthread_create(&threads[i], NULL, philo_routine, &data.philo[i]))
			exit_error(&data);
	if (pthread_create(&monitor_thread, NULL, philo_monitor, &data))
		exit_error(&data);
	i = -1;
	while (++i < data.nbr_of_philos)
		if (pthread_join(threads[i], NULL))
			exit_error(&data);
	if (pthread_join(monitor_thread, NULL))
		exit_error(&data);
	// check_philo_meals(&data);
	return (data_cleanup(&data), EXIT_SUCCESS);
}

/*
 *	pthread_create:
	- creates a new thread
	- first argument is the thread 
	- second argument is the thread attributes (NULL for default)
	- third argument is the function to be executed by the thread
	- fourth argument is the argument to be passed to the function 
 
 * pthread_join:
	-  Allows one thread to wait for the termination of another thread. 
		When you call pthread_join with a specific thread identifier, 
		the calling thread will block until the specified thread terminates.
	- Can be used to obtain the return value of the terminated thread. 
		This can be useful if the thread performs some computation 
		and you want to retrieve the result.
	- first argument is the thread 
	- second argument is the return value of the thread function 
		(NULL if not needed)

 * main(int ac, char **av)
	- if (ac != 5 && ac != 6)
		. We have 4 at least of 5 at max args to provide to our program
		. 1) nbr_of_philos
		. 2) time_to_die
		. 3) time_to_eat
		. 4) time_to_sleep
		. 5) must_eat_count which is not mandatory to pass
		. Add to that the ./philo, so total is 5 or 6
		. if it's not 5 and not 6 then EXIT_FAILURE

	- data_init(av, &data);
		. Initialize the data struct

	- threads = data.threads;
		. Assign the threads to the data threads

	- Now we need to create all the threads we will use in our program
		and join them together:
		. We first create the threads for our philos
		. Then we create the thread for our monitor function
		. Then we join all the threads together so each thread 
			waits for the other thread to finish
		. If either pthread_create or pthread_join returns a truthy value 
			which any value beside 0, then we exit the program with an error
*/
