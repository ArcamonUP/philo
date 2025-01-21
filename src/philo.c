/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:06:08 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/21 17:29:15 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void	*philo(void *arg)
{
	t_thread		*thread;
	struct timeval	tv;

	if (!arg)
		return (NULL);
	thread = arg;
	synchronize(thread);
	tv = thread->data->tv;
	while (1)
	{
		if (philo_took_fork(*thread, tv))
			return (free(arg), NULL);
		if (philo_eat(*thread, tv))
			return (free(arg), NULL);
		gettimeofday(&tv, NULL);
		if (philo_sleep(*thread, tv))
			return (free(arg), NULL);
	}
}

int	main(int ac, char **av)
{
	pthread_t		*tid;
	t_philo			data;
	int				i;

	//Faire tests de -1 a 10.
	//il faut parser tout : pas de char, bon nombres d'args. Il peut y en avoir 6
	// nb_eat : pas pris en compte pr l'instant
	//check si les pthread_create/init/mutex/gettime fail a chaque fois : ils peuvent fail
	//philo 4 410 200 200 : Doit fonctionner sans morts : pas le cas actuellement
	//Temps pour detacter la mort un peu trop long : a fix aussi
	//Regler le pb dans le main ci-dessous.
	tid = init(ac, av, &data);
	if (!tid)
		return (1);
	i = 0;
	while (i < data.num_total)
	{
		if (pthread_create(&tid[i], NULL, philo, cp_data(&data, i)) != 0)
		{
			(free_thread(tid, i + 1), free_mutex(data));
			return (write(2, "Error.\nFailed to create thread.\n", 32), 1);
		}
		i++;
	}
	pthread_mutex_lock(data.alive.mutex);
	data.alive.value = 1;
	gettimeofday(&data.tv, NULL);
	//check ici constamment si un truc est mort : faire le join que apres ca
	pthread_mutex_unlock(data.alive.mutex);
	i = 0;
	while (tid[i])
		pthread_join(tid[i++], NULL);
	return (free(tid), free_mutex(data), 0);
}
