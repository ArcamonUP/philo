/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 11:39:27 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/17 10:56:18 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

void	free_thread(pthread_t *tid, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		pthread_detach(tid[i]);
		i++;
	}
	free(tid);
}

void	free_mutex(t_philo data)
{
	int	i;

	i = 0;
	while (data.mutex[i])
	{
		pthread_mutex_destroy(data.mutex[i]);
		free(data.mutex[i]);
		i++;
	}
	free(data.mutex);
}
