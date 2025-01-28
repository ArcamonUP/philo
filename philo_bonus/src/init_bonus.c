/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:52:30 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/28 15:11:44 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

// Initializes shared data:finished if necessary
t_philo	*init_finished(t_philo *data)
{
	data->finished.semaphore = sem_open("/finished", O_CREAT, 0644, 1);
	if (data->finished.semaphore == SEM_FAILED)
	{
		(sem_close(data->alive.semaphore), sem_unlink("/alive"));
		(sem_close(data->forks.semaphore), sem_unlink("/forks"));
		(sem_close(data->writing), sem_unlink("/writing"));
		return (NULL);
	}
	data->finished.value = 0;
	return (data);
}

// Initializes shared data for philosopher threads
t_philo	*init_shared(t_philo *data)
{
	data->alive.semaphore = sem_open("/alive", O_CREAT, 0644, 1);
	if (data->alive.semaphore == SEM_FAILED)
		return (NULL);
	data->alive.value = WAIT;
	data->forks.semaphore = sem_open("/forks", O_CREAT, 0644, 1);
	if (data->forks.semaphore == SEM_FAILED)
	{
		(sem_close(data->alive.semaphore), sem_unlink("/alive"));
		return (NULL);
	}
	data->forks.value = data->num_total;
	data->writing = sem_open("/writing", O_CREAT, 0644, 1);
	if (data->writing == SEM_FAILED)
	{
		(sem_close(data->alive.semaphore), sem_unlink("/alive"));
		(sem_close(data->forks.semaphore), sem_unlink("/forks"));
		return (NULL);
	}
	return (init_finished(data));
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
	{
		(free(tid), free(*t_data));
		return (write(2, "Error.\nShared values init.\n", 22), NULL);
	}
	return (tid);
}
