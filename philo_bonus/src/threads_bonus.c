/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 09:58:12 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/28 15:14:22 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Copies philosopher data to give each philosopher their own struct
t_thread	*cp_data(t_philo *data, int i)
{
	t_thread	*result;

	result = (t_thread *)malloc(sizeof(t_thread));
	if (!result)
		return (NULL);
	result->data = data;
	result->num = i + 1;
	return (result);
}

void	incr_finished(t_thread *th, int i)
{
	struct timeval	tv;
	long long int	time;

	if (th->data->nb_eat == -1 || i != th->data->nb_eat)
		return ;
	sem_wait(th->data->finished.semaphore);
	th->data->finished.value++;
	if (th->data->finished.value == th->data->num_total)
	{
		gettimeofday(&tv, NULL);
		time = (tv.tv_sec - th->data->tv.tv_sec) * 1000000;
		time += tv.tv_usec - th->data->tv.tv_usec;
		sem_post(th->data->finished.semaphore);
		sem_wait(th->data->writing);
		sem_wait(th->data->alive.semaphore);
		th->data->alive.value = DEAD;
		printf("%lld All philosophers are satisfied.\n", time / 1000);
		sem_post(th->data->writing);
		sem_post(th->data->alive.semaphore);
	}
	else
		sem_post(th->data->finished.semaphore);
	return ;
}

// Synchronizes philosopher start times and staggers even-numbered philosophers
int	synchronize(t_thread *thread)
{
	sem_wait(thread->data->alive.semaphore);
	while (thread->data->alive.value == WAIT)
	{
		sem_post(thread->data->alive.semaphore);
		usleep(1000);
		sem_wait(thread->data->alive.semaphore);
	}
	if (thread->data->alive.value == DEAD)
		return (sem_post(thread->data->alive.semaphore), EXIT_FAILURE);
	sem_post(thread->data->alive.semaphore);
	if (thread->num % 2 == 0)
		usleep(3000);
	else if ((thread->num / 2) % 2 == 0)
		usleep(1000);
	return (EXIT_SUCCESS);
}

// Calculates remaining time before a philosopher dies
int	is_out_of_time(t_thread thread, struct timeval tv)
{
	struct timeval	tv2;
	long long int	current_time;
	long long int	max_time;

	gettimeofday(&tv2, NULL);
	max_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000) + thread.data->time_die;
	current_time = (tv2.tv_sec * 1000) + (tv2.tv_usec / 1000);
	sem_wait(thread.data->alive.semaphore);
	if (thread.data->alive.value == DEAD || current_time > max_time)
	{
		if (thread.data->alive.value == ALIVE)
		{
			thread.data->alive.value = DEAD;
			print_move(thread, "died", 1);
		}
		sem_post(thread.data->alive.semaphore);
		return (-1);
	}
	sem_post(thread.data->alive.semaphore);
	return (0);
}
