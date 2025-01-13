/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:52:30 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/13 14:45:01 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

pthread_t	*init(int ac, char **av, t_philo *data)
{
	pthread_t	*tid;

	tid = malloc(sizeof(pthread_t) * (ft_atoi(av[1]) + 1));
	if (!tid)
		return (NULL);
	gettimeofday(&(*data).tv, NULL);
	(*data).time_die = ft_atoi(av[2]);
	(*data).time_eat = ft_atoi(av[3]);
	(*data).time_sleep = ft_atoi(av[4]);
	(*data).num = 1;
	if (ac == 6)
		(*data).nb_eat = ft_atoi(av[5]);
	return (tid);
}
