/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:52:30 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/28 12:44:07 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdlib.h>

// Initializes all philosopher forks with mutexes
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

// Initializes shared data:finished if necessary
t_philo	*init_finished(t_philo *data)
{
	if (data->nb_eat == -1)
		return (data);
	data->finished.mutex = malloc(sizeof(pthread_mutex_t) * 1);
	if (!data->finished.mutex || \
	pthread_mutex_init(data->finished.mutex, NULL) != 0)
	{
		if (data->finished.mutex)
			free(data->finished.mutex);
		return (NULL);
	}
	data->finished.value = 0;
	return (data);
}

// Initializes shared data for philosopher threads
t_philo	*init_shared(t_philo *data)
{
	data->alive.mutex = malloc(sizeof(pthread_mutex_t) * 1);
	if (!data->alive.mutex || pthread_mutex_init(data->alive.mutex, NULL) != 0)
	{
		if (data->alive.mutex)
			free(data->alive.mutex);
		return (NULL);
	}
	data->alive.value = WAIT;
	data->writing = malloc(sizeof(pthread_mutex_t) * 1);
	if (!data->writing || pthread_mutex_init(data->writing, NULL) != 0)
	{
		pthread_mutex_destroy(data->alive.mutex);
		free(data->alive.mutex);
		if (data->writing)
			free(data->writing);
		return (NULL);
	}
	data = init_finished(data);
	if (!data)
	{
		(pthread_mutex_destroy(data->writing), free(data->writing));
		(pthread_mutex_destroy(data->alive.mutex), free(data->alive.mutex));
		return (NULL);
	}
	return (data);
}

// Validates input and initializes basic philosopher data
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
	data->time_eat < 1 || data->time_sleep < 1 || (ac == 6 && data->nb_eat < 1))
		return (NULL);
	return (data);
}

// General initialization for philosophers, forks, and shared data
pthread_t	*init(int ac, char **av, t_philo *data, t_thread ***t_data)
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
	*t_data = ft_calloc(sizeof(t_thread *), data->num_total + 1);
	if (!*t_data)
		return (write(2, "Error.\nMalloc failed.\n", 22), free(tid), NULL);
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
