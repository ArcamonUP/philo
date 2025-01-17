/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:07:22 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/17 11:45:05 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>

void	print_move(t_thread thread, char *str)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	printf("%ld: %d %s\n", tv.tv_usec - thread.data->tv.tv_usec, \
	thread.num, str);
}

int	philo_took_fork(t_thread thread, int fork, struct timeval tv)
{
	int				other_fork;

	if (!thread.data->alive || is_out_of_time(thread, tv))
	{
		if (thread.data->alive)
		{
			thread.data->alive = 0;
			print_move(thread, "died");
		}
		return (1);
	}
	pthread_mutex_lock(thread.data->mutex[fork]);
	print_move(thread, "has taken a fork");
	if (thread.num - 1 == fork)
	{
		other_fork = thread.num - 2;
		if (other_fork < 0)
			other_fork = thread.data->num_total - 1;
		if (philo_took_fork(thread, other_fork, tv))
		{
			pthread_mutex_unlock(thread.data->mutex[fork]);
			return (1);
		}
	}
	return (0);
}

int	philo_eat(t_thread thread, struct timeval tv)
{
	int				fork;

	if (!thread.data->alive || is_out_of_time(thread, tv))
	{
		pthread_mutex_unlock(thread.data->mutex[thread.num - 1]);
		fork = thread.num - 2;
		if (fork < 0)
			fork = thread.data->num_total - 1;
		pthread_mutex_unlock(thread.data->mutex[fork]);
		if (thread.data->alive)
		{
			thread.data->alive = 0;
			print_move(thread, "died");
		}
		return (1);
	}
	print_move(thread, "is eating");
	usleep(thread.data->time_eat);
	pthread_mutex_unlock(thread.data->mutex[thread.num - 1]);
	fork = thread.num - 2;
	if (fork < 0)
		fork = thread.data->num_total - 1;
	return (pthread_mutex_unlock(thread.data->mutex[fork]), 0);
}

int	philo_sleep(t_thread thread, struct timeval tv)
{
	if (!thread.data->alive || is_out_of_time(thread, tv))
	{
		if (thread.data->alive)
		{
			thread.data->alive = 0;
			print_move(thread, "died");
		}
		return (1);
	}
	print_move(thread, "is sleeping");
	usleep(thread.data->time_sleep);
	return (0);
}

int	philo_think(t_thread thread, struct timeval tv)
{
	if (!thread.data->alive || is_out_of_time(thread, tv))
	{
		if (thread.data->alive)
		{
			thread.data->alive = 0;
			print_move(thread, "died");
		}
		return (1);
	}
	print_move(thread, "is thinking");
	return (0);
}
