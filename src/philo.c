/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:06:08 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/13 15:12:26 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "philo.h"
#include <stdlib.h>

void	*philo(void *arg)
{
	t_philo	*data;
	char	*str = "died";

	if (!arg)
		return (NULL);
	data = arg;
	while (1)
	{
		if (!print_move(data, str))
			return (free(arg), NULL);
	}
	return (free(arg), NULL);
}

int	main(int ac, char **av)
{
	pthread_t		*tid;
	t_philo			data;

	if (ac < 4 || ac > 5)
		return (write(2, "Error.\nWrong parameters.\n", 25), 1);
	tid = init(ac, av, &data);
	if (!tid)
		return (1);
	while (data.num - 1 < ft_atoi(av[1]))
	{
		pthread_create(&tid[data.num - 1], NULL, philo, copy_data(data));
		data.num++;
	}
	//boucle pr check si chaque process est fini : mutex
	pthread_join(tid[0], NULL);
	free_thread(tid, ft_atoi(av[1]));
	return (0);
}
