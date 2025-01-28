/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 09:58:12 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/28 12:38:09 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
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
	pthread_mutex_lock(th->data->finished.mutex);
	th->data->finished.value++;
	if (th->data->finished.value == th->data->num_total)
	{
		gettimeofday(&tv, NULL);
		time = (tv.tv_sec - th->data->tv.tv_sec) * 1000000;
		time += tv.tv_usec - th->data->tv.tv_usec;
		pthread_mutex_unlock(th->data->finished.mutex);
		pthread_mutex_lock(th->data->writing);
		pthread_mutex_lock(th->data->alive.mutex);
		th->data->alive.value = DEAD;
		printf("%lld All philosophers are satisfied.\n", time / 1000);
		pthread_mutex_unlock(th->data->writing);
		pthread_mutex_unlock(th->data->alive.mutex);
	}
	else
		pthread_mutex_unlock(th->data->finished.mutex);
	return ;
}

// Synchronizes philosopher start times and staggers even-numbered philosophers
int	synchronize(t_thread *thread)
{
	pthread_mutex_lock(thread->data->alive.mutex);
	while (thread->data->alive.value == WAIT)
	{
		pthread_mutex_unlock(thread->data->alive.mutex);
		usleep(1000);
		pthread_mutex_lock(thread->data->alive.mutex);
	}
	if (thread->data->alive.value == DEAD)
		return (pthread_mutex_unlock(thread->data->alive.mutex), EXIT_FAILURE);
	pthread_mutex_unlock(thread->data->alive.mutex);
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
	pthread_mutex_lock(thread.data->alive.mutex);
	if (thread.data->alive.value == DEAD || current_time > max_time)
	{
		if (thread.data->alive.value == ALIVE)
		{
			thread.data->alive.value = DEAD;
			print_move(thread, "died", 1);
		}
		pthread_mutex_unlock(thread.data->alive.mutex);
		return (-1);
	}
	pthread_mutex_unlock(thread.data->alive.mutex);
	return (0);
}
