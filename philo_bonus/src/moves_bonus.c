/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:07:22 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/28 14:53:57 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
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

// Handles printing the move of taking forks and eating
void	take_and_eat(t_thread th, struct timeval tv)
{
	if (th.data->forks.value > 1)
	{
		th.data->forks.value -= 2;
		print_move(th, "has taken a fork", 2);
		print_move(th, "is eating", 1);
		sem_post(th.data->forks.semaphore);
		sem_post(th.data->writing);
	}
	else
	{
		sem_post(th.data->forks.semaphore);
		sem_post(th.data->writing);
		usleep(1000);
		philo_took_fork(th, tv);
	}
	return ;
}

// Handles taking a fork based on fork availability
int	philo_took_fork(t_thread th, struct timeval tv)
{
	sem_wait(th.data->forks.semaphore);
	sem_wait(th.data->writing);
	if (is_out_of_time(th, tv) == -1)
	{
		(sem_post(th.data->forks.semaphore), sem_post(th.data->writing));
		return (EXIT_FAILURE);
	}
	if (th.data->num_total == 1)
	{
		th.data->forks.value--;
		print_move(th, "has taken a fork", 1);
		while (is_out_of_time(th, tv) != -1)
			usleep(100);
		(sem_post(th.data->forks.semaphore), sem_post(th.data->writing));
		return (EXIT_FAILURE);
	}
	take_and_eat(th, tv);
	return (EXIT_SUCCESS);
}

// Manages philosopher's eating process and resets their cooldown
int	philo_eat(t_thread thread, struct timeval tv)
{
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
	sem_wait(thread.data->forks.semaphore);
	thread.data->forks.value += 2;
	sem_post(thread.data->forks.semaphore);
	sem_wait(thread.data->writing);
	if (is_out_of_time(thread, tv) == -1)
		return (sem_post(thread.data->writing), EXIT_FAILURE);
	sem_post(thread.data->writing);
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
	sem_wait(thread.data->writing);
	if (is_out_of_time(thread, tv) == -1)
		return (sem_post(thread.data->writing), EXIT_FAILURE);
	print_move(thread, "is sleeping", 1);
	sem_post(thread.data->writing);
	if (curr_time + thread.data->time_sleep < max_time)
		usleep(thread.data->time_sleep * 1000);
	else
		usleep((max_time - curr_time + 1) * 1000);
	sem_wait(thread.data->writing);
	if (is_out_of_time(thread, tv) == -1)
		return (sem_post(thread.data->writing), EXIT_FAILURE);
	print_move(thread, "is thinking", 1);
	sem_post(thread.data->writing);
	if (thread.data->num_total % 2 != 0)
		usleep(((max_time - (curr_time + thread.data->time_sleep)) / 3) * 1000);
	return (EXIT_SUCCESS);
}
