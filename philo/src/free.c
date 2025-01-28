/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 11:39:27 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/28 12:00:23 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <unistd.h>

// Process to free + display an error in case of thread creation failure
int	end_process(t_philo *data, t_thread **thd, pthread_t *tid, int ret)
{
	int	i;

	if (ret == EXIT_FAILURE)
	{
		pthread_mutex_lock(data->alive.mutex);
		data->alive.value = DEAD;
		pthread_mutex_unlock(data->alive.mutex);
	}
	else
	{
		pthread_mutex_lock(data->alive.mutex);
		data->alive.value = ALIVE;
		gettimeofday(&data->tv, NULL);
		pthread_mutex_unlock(data->alive.mutex);
	}
	i = 0;
	while (tid[i])
		pthread_join(tid[i++], NULL);
	free(tid);
	free_mutex(*data);
	free_t_data(thd);
	if (ret == EXIT_FAILURE)
		write(2, "Error.\nFailed to create thread.\n", 32);
	return (ret);
}

// Frees all allocated mutexes
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
	if (data.nb_eat != -1)
	{
		pthread_mutex_destroy(data.finished.mutex);
		free(data.finished.mutex);
	}
}

// Frees all thread data structures
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
