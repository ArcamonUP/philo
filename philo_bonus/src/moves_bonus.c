/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:07:22 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/26 16:03:40 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <unistd.h>

// Logs and secures each philosopher's action with precise timing
void	print_move(t_thread thread, char *str, int i)
{
	struct timeval	tv2;
	long long int	time;

	sem_wait(thread.data->writing);
	gettimeofday(&tv2, NULL);
	time = (tv2.tv_sec - thread.data->tv.tv_sec) * 1000000;
	time += tv2.tv_usec - thread.data->tv.tv_usec;
	while (i != 0)
	{
		ft_putnbr(time / 1000);
		write(STDOUT_FILENO, " ", 1);
		ft_putnbr(thread.num);
		write(STDOUT_FILENO, " ", 1);
		write(STDOUT_FILENO, str, ft_strlen(str));
		write(STDOUT_FILENO, "\n", 1);
		i--;
	}
	sem_post(thread.data->writing);
}

// Handles taking a fork based on fork availability
int	philo_took_fork(t_thread th, struct timeval tv)
{
	sem_wait(th.data->forks.semaphore);
	if (is_out_of_time(th, tv, 0) == -1)
		return (sem_post(th.data->forks.semaphore), EXIT_FAILURE);
	if (th.data->num_total == 1)
	{
		th.data->forks.value -= 1;
		print_move(th, "has taken a fork", 1);
		while (is_out_of_time(th, tv, 0) != -1)
			usleep(100);
		return (sem_post(th.data->forks.semaphore), EXIT_FAILURE);
	}
	if (th.data->forks.value > 1)
	{
		th.data->forks.value -= 2;
		print_move(th, "has taken a fork", 2);
		sem_post(th.data->forks.semaphore);
	}
	else
	{
		sem_post(th.data->forks.semaphore);
		usleep(1000);
		philo_took_fork(th, tv);
	}
	return (EXIT_SUCCESS);
}

// Manages philosopher's eating process and resets their cooldown
int	philo_eat(t_thread thread, struct timeval tv)
{
	int				time;

	time = is_out_of_time(thread, tv, thread.data->time_eat);
	if (time == -1)
		return (EXIT_FAILURE);
	print_move(thread, "is eating", 1);
	usleep(time);
	sem_wait(thread.data->forks.semaphore);
	thread.data->forks.value += 2;
	sem_post(thread.data->forks.semaphore);
	if (is_out_of_time(thread, tv, 0) == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

// Manages the philosopher's sleep and think actions
int	philo_sleep_think(t_thread thread, struct timeval tv)
{
	int	time;

	time = is_out_of_time(thread, tv, thread.data->time_sleep);
	if (time == -1)
		return (EXIT_FAILURE);
	print_move(thread, "is sleeping", 1);
	usleep(time);
	if (is_out_of_time(thread, tv, 0) == -1)
		return (EXIT_FAILURE);
	print_move(thread, "is thinking", 1);
	return (EXIT_SUCCESS);
}
