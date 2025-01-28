/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:07:22 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/28 12:28:46 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>

// Logs and secures each philosopher's action with precise timing
void	print_move(t_thread thread, char *str, int num)
{
	struct timeval	tv2;
	long long int	time;

	gettimeofday(&tv2, NULL);
	time = (tv2.tv_sec - thread.data->tv.tv_sec) * 1000000;
	time += tv2.tv_usec - thread.data->tv.tv_usec;
	while (num-- != 0)
		printf("%lld %d %s\n", time / 1000, thread.num, str);
}

// Handles taking a fork based on philo's position and fork availability
int	philo_took_fork(t_thread th, struct timeval tv)
{
	int	fork1;
	int	fork2;

	fork1 = th.num - 1;
	if (th.num % 2 == 0 || th.num == th.data->num_total)
		fork1--;
	if (th.data->num_total == 1)
		fork1 = 0;
	fork2 = th.num - 2;
	if (fork2 < 0)
		fork2 = th.data->num_total - 1;
	else if (th.num % 2 == 0 || th.num == th.data->num_total)
		fork2++;
	pthread_mutex_lock(th.data->mutex[fork1]);
	pthread_mutex_lock(th.data->mutex[fork2]);
	pthread_mutex_lock(th.data->writing);
	if (is_out_of_time(th, tv) == -1)
	{
		pthread_mutex_unlock(th.data->mutex[fork1]);
		pthread_mutex_unlock(th.data->mutex[fork2]);
		return (pthread_mutex_unlock(th.data->writing), EXIT_FAILURE);
	}
	print_move(th, "has taken a fork", 2);
	print_move(th, "is eating", 1);
	return (pthread_mutex_unlock(th.data->writing), EXIT_SUCCESS);
}

// Manages philosopher's eating process and resets their cooldown
int	philo_eat(t_thread thread, struct timeval tv)
{
	int				fork;
	struct timeval	tv2;
	long long int	current_time;
	long long int	max_time;

	gettimeofday(&tv2, NULL);
	max_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000) + thread.data->time_die;
	current_time = (tv2.tv_sec * 1000) + (tv2.tv_usec / 1000);
	if (current_time + thread.data->time_eat < max_time)
		usleep(thread.data->time_eat * 1000);
	else
		usleep((max_time - current_time + 1) * 1000);
	pthread_mutex_unlock(thread.data->mutex[thread.num - 1]);
	fork = thread.num - 2;
	if (fork < 0)
		fork = thread.data->num_total - 1;
	pthread_mutex_unlock(thread.data->mutex[fork]);
	pthread_mutex_lock(thread.data->writing);
	if (is_out_of_time(thread, tv) == -1)
		return (pthread_mutex_unlock(thread.data->writing), EXIT_FAILURE);
	pthread_mutex_unlock(thread.data->writing);
	return (EXIT_SUCCESS);
}

// Manages the philosopher's sleep and think actions
int	philo_sleep_think(t_thread thread, struct timeval tv)
{
	struct timeval	tv2;
	long long int	curr_time;
	long long int	max_time;

	gettimeofday(&tv2, NULL);
	max_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000) + thread.data->time_die;
	curr_time = (tv2.tv_sec * 1000) + (tv2.tv_usec / 1000);
	pthread_mutex_lock(thread.data->writing);
	if (is_out_of_time(thread, tv) == -1)
		return (pthread_mutex_unlock(thread.data->writing), EXIT_FAILURE);
	print_move(thread, "is sleeping", 1);
	pthread_mutex_unlock(thread.data->writing);
	if (curr_time + thread.data->time_sleep < max_time)
		usleep(thread.data->time_sleep * 1000);
	else
		usleep((max_time - curr_time + 1) * 1000);
	pthread_mutex_lock(thread.data->writing);
	if (is_out_of_time(thread, tv) == -1)
		return (pthread_mutex_unlock(thread.data->writing), EXIT_FAILURE);
	print_move(thread, "is thinking", 1);
	pthread_mutex_unlock(thread.data->writing);
	if (thread.data->num_total % 2 != 0)
		usleep(((max_time - (curr_time + thread.data->time_sleep)) / 3) * 1000);
	return (EXIT_SUCCESS);
}
