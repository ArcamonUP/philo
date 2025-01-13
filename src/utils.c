/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:50:41 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/13 15:12:39 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <stdio.h>

int	ft_atoi(const char *str)
{
	int	nb;
	int	i;
	int	result;

	i = 0;
	nb = 1;
	result = 0;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			nb = -nb;
		i++;
	}
	while (str[i] && '0' <= str[i] && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * nb);
}

int	ft_strncmp(const char *s1, const char *s2, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i] && i < n - 1)
		i++;
	if (i < n)
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	return (0);
}

int	print_move(t_philo *data, char *str)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	printf("%ld: %d %s\n", tv.tv_usec - data->tv.tv_usec, data->num, str);
	if (ft_strncmp(str, "died", 4) == 0)
		return (0);
	return (1);
}

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

t_philo	*copy_data(t_philo data)
{
	t_philo	*result;

	result = (t_philo *)malloc(sizeof(t_philo));
	if (!result)
		return (NULL);
	(*result) = data;
	return (result);
}
