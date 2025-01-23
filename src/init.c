/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:52:30 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/23 11:47:21 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <unistd.h>

void	synchronize(t_thread *thread)
{
	pthread_mutex_lock(thread->data->alive.mutex);
	while (thread->data->alive.value == 2)
	{
		pthread_mutex_unlock(thread->data->alive.mutex);
		usleep(100);
		pthread_mutex_lock(thread->data->alive.mutex);
	}
	pthread_mutex_unlock(thread->data->alive.mutex);
	if (thread->num % 2 == 0)
		usleep(2000);
}

pthread_t	*init_mutex(t_philo *data, int num, pthread_t *tid)
{
	int	i;

	i = 0;
	while (i < num)
	{
		data->mutex[i] = malloc(sizeof(pthread_mutex_t) * 1);
		if (!data->mutex[i] || pthread_mutex_init(data->mutex[i], NULL) != 0)
		{
			write(2, "Error.\nFailed to init mutex.\n", 29);
			while (i != 0)
			{
				pthread_mutex_destroy(data->mutex[i]);
				free(data->mutex[i]);
				i--;
			}
			free(tid);
			free(data->mutex);
			pthread_mutex_destroy(data->alive.mutex);
			pthread_mutex_destroy(data->writing);
			return (free(data->writing), free(data->alive.mutex), NULL);
		}
		i++;
	}
	return (tid);
}

t_philo	*init_shared(t_philo *data)
{
	data->alive.mutex = malloc(sizeof(pthread_mutex_t) * 1);
	if (!data->alive.mutex || pthread_mutex_init(data->alive.mutex, NULL) != 0)
	{
		if (data->alive.mutex)
			free(data->alive.mutex);
		return (NULL);
	}
	data->alive.value = 2;
	data->writing = malloc(sizeof(pthread_mutex_t) * 1);
	if (!data->writing || pthread_mutex_init(data->writing, NULL) != 0)
	{
		pthread_mutex_destroy(data->alive.mutex);
		free(data->alive.mutex);
		if (data->writing)
			free(data->writing);
		return (NULL);
	}
	return (data);
}

t_philo	*init_basics(int ac, char **av, t_philo *data)
{
	int	i;
	int	j;

	i = 0;
	while (av[++i])
	{
		j = -1;
		while (av[i][++j])
		{
			if ('0' > av[i][j] || '9' < av[i][j])
				return (NULL);
		}
	}
	data->num_total = ft_atoi(av[1]);
	data->time_die = ft_atoi(av[2]);
	data->time_eat = ft_atoi(av[3]);
	data->time_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->nb_eat = ft_atoi(av[5]);
	else
		data->nb_eat = -1;
	if (data->num_total < 1 || data->time_die < 1 || \
	data->time_eat < 1 || data->time_sleep < 1)
		return (NULL);
	return (data);
}

pthread_t	*init(int ac, char **av, t_philo *data)
{
	pthread_t	*tid;

	if (ac < 5 || ac > 6)
		return (write(2, "Error.\nWrong parameters.\n", 25), NULL);
	data = init_basics(ac, av, data);
	if (!data)
		return (write(2, "Error.\nInvalid parameters.\n", 27), NULL);
	tid = ft_calloc(sizeof(pthread_t), ft_atoi(av[1]) + 1);
	if (!tid)
		return (write(2, "Error.\nMalloc failed.\n", 22), NULL);
	gettimeofday(&(*data).tv, NULL);
	data = init_shared(data);
	if (!data)
		return (free(tid), write(2, "Error.\nShared values init.\n", 22), NULL);
	data->mutex = ft_calloc(sizeof(pthread_mutex_t *), data->num_total + 1);
	if (!(*data).mutex)
	{
		free(tid);
		(pthread_mutex_destroy(data->alive.mutex), free(data->alive.mutex));
		(pthread_mutex_destroy(data->writing), free(data->writing));
		return (write(2, "Error.\nMalloc failed.\n", 22), NULL);
	}
	return (init_mutex(data, data->num_total, tid));
}
