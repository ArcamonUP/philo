/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:07:22 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/23 15:28:44 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

//To secure and print each move one of my philosophers does
void	print_move(t_thread thread, char *str)
{
	struct timeval	tv2;
	long long int	time;

	pthread_mutex_lock(thread.data->writing);
	gettimeofday(&tv2, NULL);
	time = (tv2.tv_sec - thread.data->tv.tv_sec) * 1000000;
	time += tv2.tv_usec - thread.data->tv.tv_usec;
	ft_putnbr(time / 1000);
	write(STDOUT_FILENO, " ", 1);
	ft_putnbr(thread.num);
	write(STDOUT_FILENO, " ", 1);
	write(STDOUT_FILENO, str, ft_strlen(str));
	write(STDOUT_FILENO, "\n", 1);
	pthread_mutex_unlock(thread.data->writing);
}

//To make the philosopher take the second fork (secure)
int	philo_took_second_fork(t_thread th, struct timeval tv, int previous)
{
	int	fork;

	if (th.data->num_total == 1)
	{
		while (is_out_of_time(th, tv, 0) != -1)
			usleep(100);
		return (pthread_mutex_unlock(th.data->mutex[previous]), 1);
	}
	fork = th.num - 2;
	if (fork < 0)
		fork = th.data->num_total - 1;
	else if (th.num % 2 == 0 || th.num == th.data->num_total)
		fork++;
	pthread_mutex_lock(th.data->mutex[fork]);
	if (is_out_of_time(th, tv, 0) == -1)
	{
		pthread_mutex_unlock(th.data->mutex[previous]);
		pthread_mutex_unlock(th.data->mutex[fork]);
		return (1);
	}
	return (print_move(th, "has taken a fork"), 0);
}

//Philosopher take fork: half of philosphers want to take the fork on his left,
//and the other half want to take the fork on his right.
int	philo_took_fork(t_thread th, struct timeval tv)
{
	int				fork;

	fork = th.num - 1;
	if (th.num % 2 == 0 || th.num == th.data->num_total)
		fork--;
	if (th.data->num_total == 1)
		fork = 0;
	pthread_mutex_lock(th.data->mutex[fork]);
	if (is_out_of_time(th, tv, 0) == -1)
		return (pthread_mutex_unlock(th.data->mutex[fork]), 1);
	print_move(th, "has taken a fork");
	return (philo_took_second_fork(th, tv, fork));
}

//Philosopher eat and reset their couldown (before die)
int	philo_eat(t_thread thread, struct timeval tv)
{
	int				fork;
	int				time;

	time = is_out_of_time(thread, tv, thread.data->time_eat);
	if (time == -1)
	{
		pthread_mutex_unlock(thread.data->mutex[thread.num - 1]);
		fork = thread.num - 2;
		if (fork < 0)
			fork = thread.data->num_total - 1;
		pthread_mutex_unlock(thread.data->mutex[fork]);
		return (1);
	}
	print_move(thread, "is eating");
	usleep(time);
	pthread_mutex_unlock(thread.data->mutex[thread.num - 1]);
	fork = thread.num - 2;
	if (fork < 0)
		fork = thread.data->num_total - 1;
	pthread_mutex_unlock(thread.data->mutex[fork]);
	if (is_out_of_time(thread, tv, 0) == -1)
		return (1);
	return (0);
}

//Philosophers sleep and think
int	philo_sleep_think(t_thread thread, struct timeval tv)
{
	int	time;

	time = is_out_of_time(thread, tv, thread.data->time_sleep);
	if (time == -1)
		return (1);
	print_move(thread, "is sleeping");
	usleep(time);
	if (is_out_of_time(thread, tv, 0) == -1)
		return (1);
	print_move(thread, "is thinking");
	return (0);
}
