/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:06:08 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/26 16:09:19 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdlib.h>

// Philosopher routine: manages actions (forks, eat, sleep, think) + synchronize
void	*philo(void *arg)
{
	t_thread		*thread;
	struct timeval	tv;
	int				i;

	if (!arg)
		return (NULL);
	thread = (t_thread *)arg;
	if (synchronize(thread))
		return (NULL);
	tv = thread->data->tv;
	i = 0;
	while (1)
	{
		if (philo_took_fork(*thread, tv))
			return (NULL);
		if (philo_eat(*thread, tv))
			return (NULL);
		i++;
		if (i == thread->data->nb_eat)
			return (NULL);
		gettimeofday(&tv, NULL);
		if (philo_sleep_think(*thread, tv))
			return (NULL);
	}
}

//Entry point of the program, responsible for initializing and managing threads
int	main(int ac, char **av)
{
	pthread_t		*tid;
	t_thread		**thread_data;
	t_philo			data;
	int				i;

	tid = init(ac, av, &data, &thread_data);
	if (!tid)
		return (EXIT_FAILURE);
	i = 0;
	while (i < data.num_total)
	{
		thread_data[i] = cp_data(&data, i);
		if (pthread_create(&tid[i], NULL, philo, thread_data[i]) != 0)
			return (end_process(&data, thread_data, tid, EXIT_FAILURE));
		i++;
	}
	return (end_process(&data, thread_data, tid, EXIT_SUCCESS));
}
