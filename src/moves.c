/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:07:22 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/24 13:36:26 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

// Logs and secures each philosopher's action with precise timing
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

// Securely handles the philosopher taking the second fork
int	philo_took_second_fork(t_thread th, struct timeval tv, int previous)
{
	int	fork;

	if (th.data->num_total == 1)
	{
		while (is_out_of_time(th, tv, 0) != -1)
			usleep(100);
		return (pthread_mutex_unlock(th.data->mutex[previous]), EXIT_FAILURE);
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
		return (EXIT_FAILURE);
	}
	return (print_move(th, "has taken a fork"), EXIT_SUCCESS);
}

// Handles taking a fork based on philo's position and fork availability
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
		return (pthread_mutex_unlock(th.data->mutex[fork]), EXIT_FAILURE);
	print_move(th, "has taken a fork");
	return (philo_took_second_fork(th, tv, fork));
}

// Manages philosopher's eating process and resets their cooldown
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
		return (EXIT_FAILURE);
	}
	print_move(thread, "is eating");
	usleep(time);
	pthread_mutex_unlock(thread.data->mutex[thread.num - 1]);
	fork = thread.num - 2;
	if (fork < 0)
		fork = thread.data->num_total - 1;
	pthread_mutex_unlock(thread.data->mutex[fork]);
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
	print_move(thread, "is sleeping");
	usleep(time);
	if (is_out_of_time(thread, tv, 0) == -1)
		return (EXIT_FAILURE);
	print_move(thread, "is thinking");
	return (EXIT_SUCCESS);
}
