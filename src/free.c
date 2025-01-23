/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 11:39:27 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/23 16:16:57 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <unistd.h>

int	fail_process(t_philo data, t_thread **thd, pthread_t *tid, int num)
{
	int	i;

	i = 0;
	pthread_mutex_lock(data.alive.mutex);
	data.alive.value = 0;
	pthread_mutex_unlock(data.alive.mutex);
	while (i < num)
	{
		pthread_detach(tid[i]);
		i++;
	}
	free(tid);
	free_t_data(thd);
	free_mutex(data);
	write(2, "Error.\nFailed to create thread.\n", 32);
	return (1);
}

void	free_mutex(t_philo data)
{
	int	i;

	i = 0;
	while (data.mutex[i])
	{
		pthread_mutex_destroy(data.mutex[i]);
		free(data.mutex[i]);
		i++;
	}
	free(data.mutex);
	pthread_mutex_destroy(data.alive.mutex);
	free(data.alive.mutex);
	pthread_mutex_destroy(data.writing);
	free(data.writing);
}

void	free_t_data(t_thread **t_data)
{
	int	i;

	i = 0;
	while (t_data[i])
	{
		free(t_data[i]);
		i++;
	}
	free(t_data);
}
