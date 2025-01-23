/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:06:08 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/23 16:18:14 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

//Philo, contains the init/synchronize, and the routine.
void	*philo(void *arg)
{
	t_thread		*thread;
	struct timeval	tv;
	int				i;

	if (!arg)
		return (NULL);
	thread = (t_thread *)arg;
	if (synchronize(thread))
		return (NULL);
	tv = thread->data->tv;
	i = 0;
	while (1)
	{
		if (philo_took_fork(*thread, tv))
			return (NULL);
		if (philo_eat(*thread, tv))
			return (NULL);
		i++;
		if (i == thread->data->nb_eat)
			return (NULL);
		gettimeofday(&tv, NULL);
		if (philo_sleep_think(*thread, tv))
			return (NULL);
	}
}

//Main: It's the dispatcher, the first function that my program call
int	main(int ac, char **av)
{
	pthread_t		*tid;
	t_thread		**thread_data;
	t_philo			data;
	int				i;

	//check si les pthread_create/init/mutex/gettime fail a chaque fois : ils peuvent fail
	//philo 4 410 200 200 : Doit fonctionner sans morts
	tid = init(ac, av, &data, &thread_data);
	if (!tid)
		return (1);
	i = 0;
	while (i < data.num_total)
	{
		int y = 0;
		thread_data[i] = cp_data(&data, i);
		if (pthread_create(&tid[i], NULL, philo, thread_data[i]) != 0 || (y != 2))
			return (fail_process(data, thread_data, tid, i + 1));
		i++;
		y++;
		//bien sur : enlever le y apres
	}
	pthread_mutex_lock(data.alive.mutex);
	data.alive.value = 1;
	gettimeofday(&data.tv, NULL);
	pthread_mutex_unlock(data.alive.mutex);
	i = 0;
	while (tid[i])
		pthread_join(tid[i++], NULL);
	return (free(tid), free_mutex(data), free_t_data(thread_data), 0);
}
