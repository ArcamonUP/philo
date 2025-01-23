/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 09:58:12 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/23 16:08:51 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdlib.h>

//Copy the data of t_philo struct, to give it to each philosopher.
//This way, every philosopher have his own struct.
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

//To synchronize the time beetween each philospher + make philosophers who start
//with even numbers begin late
int	synchronize(t_thread *thread)
{
	pthread_mutex_lock(thread->data->alive.mutex);
	while (thread->data->alive.value == 2)
	{
		pthread_mutex_unlock(thread->data->alive.mutex);
		usleep(100);
		pthread_mutex_lock(thread->data->alive.mutex);
	}
	if (thread->data->alive.value == 0)
		return (1);
	pthread_mutex_unlock(thread->data->alive.mutex);
	if (thread->num % 2 == 0)
		usleep(2000);
	return (0);
}

//This function is used to know how much time I have left before my philo die
int	is_out_of_time(t_thread thread, struct timeval tv, int move)
{
	struct timeval	tv2;
	long long int	current_time;
	long long int	max_time;

	gettimeofday(&tv2, NULL);
	max_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000) + thread.data->time_die;
	current_time = (tv2.tv_sec * 1000) + (tv2.tv_usec / 1000);
	pthread_mutex_lock(thread.data->alive.mutex);
	if (!thread.data->alive.value || current_time > max_time)
	{
		if (thread.data->alive.value)
		{
			thread.data->alive.value = 0;
			print_move(thread, "died");
		}
		pthread_mutex_unlock(thread.data->alive.mutex);
		return (-1);
	}
	pthread_mutex_unlock(thread.data->alive.mutex);
	if (current_time + move < max_time)
		return (move * 1000);
	return ((max_time - current_time + 1) * 1000);
}
