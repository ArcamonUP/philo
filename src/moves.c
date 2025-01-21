/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:07:22 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/21 16:25:40 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>

void	print_move(t_thread thread, char *str)
{
	struct timeval	tv2;
	long long int	time;

	gettimeofday(&tv2, NULL);
	time = (tv2.tv_sec - thread.data->tv.tv_sec) * 1000000;
	time += tv2.tv_usec - thread.data->tv.tv_usec;
	ft_putnbr(time / 1000);
	write(STDOUT_FILENO, ": ", 2);
	ft_putnbr(thread.num);
	write(STDOUT_FILENO, " ", 1);
	write(STDOUT_FILENO, str, ft_strlen(str));
	write(STDOUT_FILENO, "\n", 1);
	pthread_mutex_unlock(thread.data->writing);
}

int	philo_took_second_fork(t_thread th, struct timeval tv, int previous)
{
	int	fork;

	if (th.data->num_total == 1)
	{
		pthread_mutex_lock(th.data->writing);
		while (!is_out_of_time(th, tv))
			usleep(100);
		return (pthread_mutex_unlock(th.data->mutex[previous]), 1);
	}
	fork = th.num - 2;
	if (fork < 0)
		fork = th.data->num_total - 1;
	if (th.num % 2 == 0 || th.num == th.data->num_total)
		fork++;
	pthread_mutex_lock(th.data->mutex[fork]);
	pthread_mutex_lock(th.data->writing);
	if (is_out_of_time(th, tv))
	{
		pthread_mutex_unlock(th.data->mutex[previous]);
		return (pthread_mutex_unlock(th.data->mutex[fork]), 1);
	}
	return (print_move(th, "has taken a fork"), 0);
}

int	philo_took_fork(t_thread th, struct timeval tv)
{
	int				fork;

	if (is_out_of_time(th, tv))
		return (1);
	fork = th.num - 1;
	if (th.num % 2 == 0 || th.num == th.data->num_total)
		fork--;
	if (th.data->num_total == 1)
		fork = 0;
	pthread_mutex_lock(th.data->mutex[fork]);
	pthread_mutex_lock(th.data->writing);
	if (is_out_of_time(th, tv))
		return (pthread_mutex_unlock(th.data->mutex[fork]), 1);
	print_move(th, "has taken a fork");
	return (philo_took_second_fork(th, tv, fork));
}

int	philo_eat(t_thread thread, struct timeval tv)
{
	int				fork;

	pthread_mutex_lock(thread.data->writing);
	if (is_out_of_time(thread, tv))
	{
		pthread_mutex_unlock(thread.data->mutex[thread.num - 1]);
		fork = thread.num - 2;
		if (fork < 0)
			fork = thread.data->num_total - 1;
		pthread_mutex_unlock(thread.data->mutex[fork]);
		return (1);
	}
	print_move(thread, "is eating");
	usleep(thread.data->time_eat * 1000);
	pthread_mutex_unlock(thread.data->mutex[thread.num - 1]);
	fork = thread.num - 2;
	if (fork < 0)
		fork = thread.data->num_total - 1;
	return (pthread_mutex_unlock(thread.data->mutex[fork]), 0);
}

int	philo_sleep(t_thread thread, struct timeval tv)
{
	pthread_mutex_lock(thread.data->writing);
	if (is_out_of_time(thread, tv))
		return (1);
	print_move(thread, "is sleeping");
	usleep(thread.data->time_sleep * 1000);
	pthread_mutex_lock(thread.data->writing);
	if (is_out_of_time(thread, tv))
		return (1);
	print_move(thread, "is thinking");
	return (0);
}
