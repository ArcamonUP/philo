/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 09:58:12 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/26 16:01:46 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <unistd.h>
#include <stdlib.h>

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
		usleep(2000);
	return (EXIT_SUCCESS);
}

// Calculates remaining time before a philosopher dies
int	is_out_of_time(t_thread thread, struct timeval tv, int move)
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
	if (current_time + move < max_time)
		return (move * 1000);
	return ((max_time - current_time + 1) * 1000);
}
