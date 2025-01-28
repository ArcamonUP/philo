/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 11:39:27 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/28 14:12:48 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void	monitoring(t_philo *data)
{
	sem_wait(data->alive.semaphore);
	while (data->alive.value == ALIVE)
	{
		sem_post(data->alive.semaphore);
		usleep(1000);
		sem_wait(data->alive.semaphore);
	}
	sem_post(data->alive.semaphore);
	usleep(1000);
	return ;
}

// Process to free + display an error in case of thread creation failure
int	end_process(t_philo *data, t_thread **thd, int *tid, int ret)
{
	int	i;

	if (ret == EXIT_FAILURE)
	{
		sem_wait(data->alive.semaphore);
		data->alive.value = DEAD;
		sem_post(data->alive.semaphore);
	}
	else
	{
		sem_wait(data->alive.semaphore);
		data->alive.value = ALIVE;
		gettimeofday(&data->tv, NULL);
		write(2, "updated\n", 8);
		sem_post(data->alive.semaphore);
		monitoring(data);
	}
	i = 0;
	while (tid[i])
		waitpid(tid[i++], NULL, 0);
	free(tid);
	free_semaphore(*data);
	free_t_data(thd);
	if (ret == EXIT_FAILURE)
		write(2, "Error.\nFailed to create thread.\n", 32);
	return (ret);
}

// Frees all allocated semaphores
void	free_semaphore(t_philo data)
{
	sem_close(data.alive.semaphore);
	sem_unlink("/alive");
	sem_close(data.writing);
	sem_unlink("/writing");
	sem_close(data.forks.semaphore);
	sem_unlink("/forks");
	sem_close(data.finished.semaphore);
	sem_unlink("finished");
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
