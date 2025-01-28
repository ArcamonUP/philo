/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:06:08 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/28 14:40:21 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdlib.h>
#include <unistd.h>

// Philosopher routine: manages actions (forks, eat, sleep, think) + synchronize
void	*philo(void *arg)
{
	t_thread		*thread;
	struct timeval	tv;
	int				i;

	if (!arg)
		exit(EXIT_FAILURE);
	thread = (t_thread *)arg;
	if (synchronize(thread))
		exit(EXIT_FAILURE);
	write(2, "test\n", 5);
	tv = thread->data->tv;
	i = 0;
	while (1)
	{
		if (philo_took_fork(*thread, tv))
			exit(EXIT_SUCCESS);
		if (philo_eat(*thread, tv))
			exit(EXIT_SUCCESS);
		i++;
		incr_finished(thread, i);
		gettimeofday(&tv, NULL);
		if (philo_sleep_think(*thread, tv))
			exit(EXIT_SUCCESS);
	}
}

//Entry point of the program, responsible for initializing and managing process
int	main(int ac, char **av)
{
	t_thread		**thread_data;
	t_philo			data;
	pid_t			*tid;
	int				i;

	tid = init(ac, av, &data, &thread_data);
	if (!tid)
		return (EXIT_FAILURE);
	i = 0;
	while (i < data.num_total)
	{
		thread_data[i] = cp_data(&data, i);
		tid[i] = fork();
		if (tid[i] == -1)
			return (end_process(&data, thread_data, tid, EXIT_FAILURE));
		if (tid[i] == 0)
			philo(thread_data[i]);
		i++;
	}
	return (end_process(&data, thread_data, tid, EXIT_SUCCESS));
}
