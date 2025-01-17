/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:06:08 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/17 11:55:04 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void	*philo(void *arg)
{
	t_thread		*thread;
	struct timeval	tv;

	if (!arg)
		return (NULL);
	thread = arg;
	while (thread->data->alive == 2)
	{
		if (thread->num == thread->data->num_total)
			(gettimeofday(&thread->data->tv, NULL), thread->data->alive = 1);
	}
	tv = thread->data->tv;
	while (1)
	{
		if (philo_took_fork(*thread, thread->num - 1, tv))
			return (free(arg), NULL);
		if (philo_eat(*thread, tv))
			return (free(arg), NULL);
		gettimeofday(&tv, NULL);
		if (philo_sleep(*thread, tv))
			return (free(arg), NULL);
		if (philo_think(*thread, tv))
			return (free(arg), NULL);
	}
}

void	join_thread(pthread_t *tid)
{
	int	i;

	i = 0;
	while (tid[i])
	{
		pthread_join(tid[i], NULL);
		i++;
	}
}

int	main(int ac, char **av)
{
	pthread_t		*tid;
	t_philo			data;
	int				i;

	tid = init(ac, av, &data);
	if (!tid)
		return (1);
	i = 0;
	while (i < data.num_total)
	{
		if (pthread_create(&tid[i], NULL, philo, cp_data(&data, i)) != 0)
		{
			(free_thread(tid, i + 1), free_mutex(data));
			return (write(2, "Error.\nFailed to create thread.\n", 32), 1);
		}
		i++;
	}
	join_thread(tid);
	free_thread(tid, data.num_total);
	free_mutex(data);
	return (0);
}
