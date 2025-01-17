/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:52:30 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/17 11:47:36 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <unistd.h>

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
			return (NULL);
		}
		i++;
	}
	return (tid);
}

pthread_t	*init(int ac, char **av, t_philo *data)
{
	pthread_t	*tid;

	if (ac != 5)
		return (write(2, "Error.\nWrong parameters.\n", 25), NULL);
	tid = ft_calloc(sizeof(pthread_t), ft_atoi(av[1]) + 1);
	if (!tid)
		return (write(2, "Error.\nMalloc failed.\n", 22), NULL);
	gettimeofday(&(*data).tv, NULL);
	(*data).num_total = ft_atoi(av[1]);
	(*data).time_die = ft_atoi(av[2]);
	(*data).time_eat = ft_atoi(av[3]);
	(*data).time_sleep = ft_atoi(av[4]);
	(*data).mutex = ft_calloc(sizeof(pthread_mutex_t *), data->num_total + 1);
	(*data).alive = 2;
	if (!(*data).mutex)
	{
		free(tid);
		write(2, "Error.\nMalloc failed.\n", 22);
		return (NULL);
	}
	return (init_mutex(data, data->num_total, tid));
}
